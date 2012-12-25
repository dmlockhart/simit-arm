/*************************************************************************
    Copyright (C) 2002 - 2007 Wei Qin
    See file COPYING for more information.

    This program is free software; you can redistribute it and/or modify    
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*************************************************************************/
/*
    armmmu.c - Memory Management Unit emulation.
    ARMulator extensions for the ARM7100 family.
    Copyright (C) 1999  Ben Williamson

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#include "armmmu.h"
#include "arm_io.h"

#include <cassert>
#include <cstring>
#include <iostream>
#include <csignal>

/*Macro defines for MMU state*/
#define MMU_CTL       (control)

#define MMU_Enabled   (control & CONTROL_MMU)
#define MMU_Disabled  (!(MMU_Enabled))

#define MMU_Aligned   (control & CONTROL_ALIGN_FAULT)

#define MMU_ICacheEnabled  (MMU_CTL & CONTROL_INSTRUCTION_CACHE)
#define MMU_ICacheDisabled (!(MMU_ICacheDisabled))

#define MMU_DCacheEnabled  (MMU_CTL & CONTROL_DATA_CACHE)
#define MMU_DCacheDisabled (!(MMU_DCacheEnabled))

#define MMU_CacheEnabled   (MMU_CTL & CONTROL_CACHE)
#define MMU_CacheDisabled  (!(MMU_CacheEnabled))

#define MMU_WBEnabled      (MMU_CTL & CONTROL_WRITE_BUFFER)
#define MMU_WBDisabled     (!(MMU_WBEnabled))

#define FLUSH_MMU_PAGE_TABLE(a_) \
	memset(a_, 0xff, (sizeof(mmu_page_table_entry_t) * MMU_HASH_SIZE))

#define FLUSH_MMU_PAGE_ENTRY(a_) \
	memset(a_, 0xff, sizeof(mmu_page_table_entry_t))

using namespace simit;

static word_t tlb_masks[] = {
	0x00000000,		/* TLB_INVALID */
	0xFFFFF000,		/* TLB_SMALLPAGE */
	0xFFFF0000,		/* TLB_LARGEPAGE */
	0xFFF00000,		/* TLB_SECTION */
	0xFFFFF000,		/* TLB_ESMALLPAGE, have TEX attirbute, only for XScale */
	0xFFFFFC00		/* TLB_TINYPAGE */
};
		
static word_t tlb_va_to_pa(tlb_entry_t * tlb_e, word_t va) {
	/*	
	The non-optimized function should be like this code. For more 
	information see the code at the bottom of translation_walk function.
	
	return (tlb_e->phys_addr & tlb_e->mask) | (va & tlb_e->nmask);
	*/

	return tlb_e->phys_addr + va;
}

arm_mmu::arm_mmu (arm_emulator *emula) : emu(emula) 
{

	i_tlb = new tlb<MMU_ITLB_SIZE>;
	d_tlb = new tlb<MMU_DTLB_SIZE>;

	/* suppress valgrind complaints */
	control = 0;
	rs_flag = 0;
	domain_access_control = 0;
	
	set_control(0x70);
    set_domain_access_control( 0xDEADC0DE);

    translation_table_base = 0xDEADC0DE;
    fault_status = 0;
    fault_address = 0;
    process_id = 0;
	

	fault_count = 0;

	dummy_entry.mapping = TLB_SMALLPAGE;
	dummy_entry.read_fault = NO_FAULT;
	dummy_entry.write_fault = NO_FAULT;
	
	register_write_checker(NULL,NULL,NULL);

}

arm_mmu::~arm_mmu () {
	delete i_tlb;
	delete d_tlb;
}

void arm_mmu::reset () {
	i_tlb->reset();
	d_tlb->reset();
}

		
/*set the value of control register*/
void arm_mmu::set_control(word_t value) {
	
	if( (control &1) != (value & 1)) {
		FLUSH_MMU_PAGE_TABLE(i_table);		
		FLUSH_MMU_PAGE_TABLE(d_table);		
	}	
			
	if(rs_flag !=( (value >> 7) & 6) ) {
		rs_flag =( (value >> 7) & 6);
		evaluate_access_all();
	}
			
	if (MMU_Aligned) {
		mask_array[MMU_BYTE - 1] = MMU_HASH_MASK;
		mask_array[MMU_HALF - 1] = 0xFFFFF001;
		mask_array[MMU_WORD - 1] = 0xFFFFF003;
	}
	else{
		mask_array[MMU_BYTE - 1] = MMU_HASH_MASK;
		mask_array[MMU_HALF - 1] = MMU_HASH_MASK;
		mask_array[MMU_WORD - 1] = MMU_HASH_MASK;
	}
	
	control = value;
}

void arm_mmu::set_process_id (word_t value)
{
	value &= MMU_PID_VA_MAP_MASK;
	if (process_id != value) {

		process_id = value;
		int i;

		for(i=0; i < MMU_HASH_SIZE; i++)
			if ( ( i_table[i].phys_addr != 0xFFFFFFFF) &&
				 ( (i_table[i].read_tag & MMU_PID_VA_MAP_MASK) == 0 ||
					(i_table[i].write_tag & MMU_PID_VA_MAP_MASK) == 0 ))
				FLUSH_MMU_PAGE_ENTRY(&i_table[i]);

		for(i=0; i < MMU_HASH_SIZE; i++)
			if ( ( d_table[i].phys_addr != 0xFFFFFFFF) &&
				 ( (d_table[i].read_tag & MMU_PID_VA_MAP_MASK) == 0 ||
					(d_table[i].write_tag & MMU_PID_VA_MAP_MASK) == 0 ))
				FLUSH_MMU_PAGE_ENTRY(&d_table[i]);
				
	}
}
	
void arm_mmu::add_mmu_page_entry(mmu_page_table_entry_t *x_table,
	tlb_entry_t *tlb_e,word_t phys_addr,word_t virt_addr)
{
	byte_t *ptr = emu->mem->translate(phys_addr); 
	if (ptr != NULL && tlb_e->mapping != TLB_TINYPAGE) {
		mmu_page_table_entry_t *pte = x_table + MMU_HASH_INDEX(virt_addr);
		
		if (tlb_e->read_fault == NO_FAULT)
			pte->read_tag = (virt_addr & MMU_HASH_MASK);
		else 
			pte->read_tag = 0xffffffff;
		

		if (x_table == d_table) {
			if (tlb_e->write_fault == NO_FAULT)
				pte->write_tag = (virt_addr & MMU_HASH_MASK);
			else 
				pte->write_tag = 0xffffffff;

			if(fcheck && fcheck(wchecker, phys_addr))
				pte->write_tag = 0xffffffff;
		}

		pte->phys_addr = (phys_addr & MMU_HASH_MASK) - (virt_addr & MMU_HASH_MASK);
		pte->ptr =	ptr + (phys_addr & MMU_HASH_MASK); //get host address 
		pte->ptr -= (virt_addr & MMU_HASH_MASK); //prepare to converse virtual address to host address
		
	}
}
		
void arm_mmu::remove_mmu_page_entry(mmu_page_table_entry_t *x_table,
	tlb_entry_t *tlb_e)
{
	FLUSH_MMU_PAGE_ENTRY(x_table + MMU_HASH_INDEX(tlb_e->virt_addr));
	if (tlb_e->mapping == TLB_SECTION || tlb_e->mapping == TLB_LARGEPAGE)
		FLUSH_MMU_PAGE_TABLE(x_table);		
	/*
		for(int i=0; i < MMU_HASH_SIZE; i++)
			if ( x_table[i].phys_addr != 0xFFFFFFFF)
				if ( ( (x_table[i].read_tag & tlb_e->mask) == tlb_e->virt_addr)
				||( (x_table[i].write_tag & tlb_e->mask) == tlb_e->virt_addr) )   
				FLUSH_MMU_PAGE_ENTRY(x_table + i);
	*/			
}
		
#ifdef RECORD_TLB_MISS			
#define D_TRANS(_a, _e) \
	if ((_e=d_tlb->search(_a))==NULL) {\
		_e = d_tlb->next();\
		remove_mmu_page_entry(d_table,_e);\
		fault = translation_walk(_a, _e); \
	} \
	else \
		fault = NO_FAULT;
		
#define I_TRANS(_a, _e) \
	if ((_e=i_tlb->search(_a))==NULL) {\
		_e = i_tlb->next();\
		remove_mmu_page_entry(i_table,_e);\
		fault = translation_walk(_a, _e); \
	} \
	else \
		fault = NO_FAULT;

#else

#define D_TRANS(_a, _e) \
	if ((_e=d_tlb->search(_a))==NULL) {\
		_e = d_tlb->next(),	fault = translation_walk(_a, _e); \
	} \
	else \
		fault = NO_FAULT;
		
#define I_TRANS(_a, _e) \
	if ((_e=i_tlb->search(_a))==NULL) {\
		_e = i_tlb->next(),	fault = translation_walk(_a, _e); \
	} \
	else \
		fault = NO_FAULT;
#endif

mmu_fault_t arm_mmu::translate_data_addr_slow (word_t virt_addr,
	mmu_access_t read, mmu_size_t size, word_t *phys_addr)
{
	mmu_fault_t fault;
	tlb_entry_t *entry;

	word_t va = mmu_pid_va_map (virt_addr);
	
	if (MMU_Disabled) {
		*phys_addr = va;
		
		/*Cache Translation Process*/
		add_mmu_page_entry(d_table ,&dummy_entry,*phys_addr,virt_addr);
	
		return NO_FAULT;
	}

	if (MMU_Aligned && (va & (size-1))) {
		return ALIGNMENT_FAULT;
	}

	/*Search for maching TLB entry*/
	D_TRANS(va, entry);
	if (fault) {/*Translation fault*/
		return fault;
	}

	/*Check access permission*/
	fault = (read == MMU_READ)? entry->read_fault : entry->write_fault;
	if (fault) {
		fault = check_access(va, entry, read);
		if(fault) {/*Access fault*/
			return fault;
		}
	}

	/*Convert virtual address to physical address*/
	*phys_addr = tlb_va_to_pa (entry, va);
	
	/*Cache Translation Process*/
	add_mmu_page_entry(d_table,entry,*phys_addr,virt_addr);
	
	return NO_FAULT;
}
	



mmu_fault_t arm_mmu::read_byte_slow (word_t virt_addr, byte_t * data)
{
	mmu_fault_t fault;
    word_t pa;

	fault = translate_data_addr_slow (virt_addr, MMU_READ, MMU_BYTE, &pa);
	if (fault) {
		update_fsr_far (fault,virt_addr);
		return fault;
	}

	MEM_READ_BYTE(pa, data);
	return NO_FAULT;

}

mmu_fault_t arm_mmu::read_hword_slow (word_t virt_addr, hword_t *data)
{
	mmu_fault_t fault;
    word_t pa;

	fault = translate_data_addr_slow (virt_addr, MMU_READ, MMU_HALF, &pa);
	if (fault) { 
		update_fsr_far (fault,virt_addr);
		return fault;
	}

	MEM_READ_HALF_WORD(pa, data);
	return NO_FAULT;

}


mmu_fault_t arm_mmu::read_word_slow	(word_t virt_addr, word_t * data)
{
	mmu_fault_t fault;
    word_t pa;

	fault = translate_data_addr_slow (virt_addr, MMU_READ, MMU_WORD, &pa);
	if (fault) {
		update_fsr_far (fault,virt_addr);
		return fault;
	}

	MEM_READ_WORD(pa, data);
	return NO_FAULT;

}

mmu_fault_t arm_mmu::write_byte_slow	(word_t virt_addr, byte_t data)
{
	mmu_fault_t fault;
    word_t pa;

	fault = translate_data_addr_slow (virt_addr, MMU_WRITE, MMU_BYTE, &pa);
	if (fault) { 
		update_fsr_far (fault,virt_addr);
		return fault;
	}

	if(falarm && falarm(wchecker, pa)) {
		return JIT_ALARM_FAULT;
	}
	
	MEM_WRITE_BYTE(pa, data);
	return NO_FAULT;

}

mmu_fault_t arm_mmu::write_hword_slow	(word_t virt_addr, hword_t data)
{
	mmu_fault_t fault;
    word_t pa;

	fault = translate_data_addr_slow (virt_addr, MMU_WRITE, MMU_HALF, &pa);
	if (fault) { 
		update_fsr_far (fault,virt_addr);
		return fault;
	}

	if(falarm && falarm(wchecker, pa)) {
		return JIT_ALARM_FAULT;
	}
	
	MEM_WRITE_HALF_WORD(pa, data);
	return NO_FAULT;

}

mmu_fault_t arm_mmu::write_word_slow	(word_t virt_addr, word_t data)
{
	mmu_fault_t fault;
    word_t pa;

	fault = translate_data_addr_slow (virt_addr, MMU_WRITE, MMU_WORD, &pa);
	if (fault) { 
		update_fsr_far (fault,virt_addr);
		return fault;
	}

	if(falarm && falarm(wchecker, pa)) {
		return JIT_ALARM_FAULT;
	}
	
	MEM_WRITE_WORD (pa, data);
	return NO_FAULT;

}

mmu_fault_t arm_mmu::translate_instr_addr_slow (word_t virt_addr, word_t *phys_addr)
{
	mmu_fault_t fault;
    tlb_entry_t *entry;
 
	word_t va = mmu_pid_va_map (virt_addr);
	
	if (MMU_Disabled) {
		*phys_addr = va;
		add_mmu_page_entry(i_table,&dummy_entry,*phys_addr,virt_addr);
		return NO_FAULT;
	}

	/*Search for maching TLB entry*/
	I_TRANS(va, entry);
	if (fault) {/*Translation fault*/
		return fault;
	}

	/*Check read access permission*/
	if (entry->read_fault) {
		fault = check_access(va, entry, MMU_READ);
		if(fault) {/*access fault*/
			return fault;
		}
	}

	/*Convert virtual address to physical address*/
	*phys_addr = tlb_va_to_pa (entry, va);

	/*Cache Translation Process*/
	add_mmu_page_entry(i_table,entry,*phys_addr,virt_addr);

	return NO_FAULT;
}

mmu_fault_t arm_mmu::load_instr_slow (word_t virt_addr, word_t * instr)
{
	mmu_fault_t fault;
    word_t pa;

	fault = translate_instr_addr_slow (virt_addr, &pa);
	if (fault) return fault;

	MEM_READ_WORD(pa, instr);
	return NO_FAULT;

}

word_t arm_mmu::mrc (word_t instr)
{
	word_t data;
	unsigned creg = (instr>>16)&15;
	//unsigned opcode_2 = (instr>>5)&7;
	//unsigned CRm = (instr>>0)&15;
	
	switch (creg) {
		case MMU_ID:
			data =  emu->get_cpu_id();
			break;
		case MMU_CONTROL:
			data =  control;
			break;
		case MMU_TRANSLATION_TABLE_BASE:
			data = translation_table_base;
			break;
		case MMU_DOMAIN_ACCESS_CONTROL:
			data = domain_access_control;
			break;
		case MMU_FAULT_STATUS:
			data = fault_status;
			break;
		case MMU_FAULT_ADDRESS:
			data = fault_address;
			break;
		case MMU_PID:
			data = process_id;
			break; //no break in Skyeye
		default:
			data = 0;
			fprintf (stderr, "cp15_mrc read UNKNOWN - reg %d, pc 0x\n", creg);
			exit (-1);
			break;
	}
	return data;
}

void arm_mmu::invalidate_tlb(word_t opcode_2,word_t CRm,word_t value) {
			
	if (opcode_2 == 0 && CRm == 0x7) {
		FLUSH_MMU_PAGE_TABLE(i_table);		
		FLUSH_MMU_PAGE_TABLE(d_table);		
		i_tlb->invalidate_all ();
		d_tlb->invalidate_all ();
	}
	else if (opcode_2 == 0 && CRm == 0x5) {
		FLUSH_MMU_PAGE_TABLE(i_table);		
		i_tlb->invalidate_all ();
	}
	else if (opcode_2 == 1 && CRm == 0x5) {
		FLUSH_MMU_PAGE_TABLE(i_table);		
		i_tlb->invalidate_entry (value);
	}
	else if (opcode_2 == 0 && CRm == 0x6) {
		FLUSH_MMU_PAGE_TABLE(d_table);		
		d_tlb->invalidate_all ();
	}
	else if (opcode_2 == 1 && CRm == 0x6) {
		FLUSH_MMU_PAGE_TABLE(d_table);		
		d_tlb->invalidate_entry (value);
	}
	else {
		fprintf (stderr,"Unknow OPC_2 = 0x%x CRm = 0x%x,Reg[15] 0x\n",
				opcode_2, CRm);
		exit (-1);
	}
}

void arm_mmu::mcr(word_t instr, word_t value)
{
	unsigned opcode_2 = (instr>>5)&7;
	unsigned CRm = (instr>>0)&15;
	unsigned creg = (instr>>16)&15;

	switch (creg) {
		case MMU_CONTROL:
			set_control((value | 0x70) & 0xFFFD);
			break;
		case MMU_TRANSLATION_TABLE_BASE:
			translation_table_base = value & 0xFFFFC000;
			FLUSH_MMU_PAGE_TABLE(i_table);		
			FLUSH_MMU_PAGE_TABLE(d_table);		
			
			break;
		case MMU_DOMAIN_ACCESS_CONTROL:
			set_domain_access_control(value);
			break;
		case MMU_FAULT_STATUS:
			fault_status = value & 0xFF;
			break;
		case MMU_FAULT_ADDRESS:
			fault_address = value;
			break;
		case MMU_CACHE_OPS:
			/*cache is not implemented*/
			//xscale_cp15_cache_ops (state, instr, value);
			break;
		case MMU_TLB_OPS:
			invalidate_tlb(opcode_2,CRm,value);		
			break;

		case MMU_SA_RB_OPS:
			// no read buffer
			break;
		case MMU_SA_DEBUG:
			break;
		case MMU_SA_CP15_R15:
			break;

		case MMU_PID:
			set_process_id(value);
			break;

		default:
			fprintf(stderr,
				"cp15_mcr wrote UNKNOWN - reg %d, reg15 0x\n", creg);
			break;
	}
}


int arm_mmu::check_perms (int ap, mmu_access_t read) const
{

	static const byte_t perms[] = {
	//	  !u    !u    !u    !u	
		0, 0, 0, 0, 0, 0, 0, 0,		//ap == 0
		0, 0, 0, 1, 1, 1, 1, 1,		//ap == 0 read
		0, 1, 0, 1, 0, 1, 0, 1,		//ap == 1	
		0, 1, 0, 1, 0, 1, 0, 1,		//ap == 1 read
		0, 1, 0, 1, 0, 1, 0, 1,		//ap == 2 
		1, 1, 1, 1, 1, 1, 1, 1,		//ap == 2 read
		1, 1, 1, 1, 1, 1, 1, 1,		//ap == 3
		1, 1, 1, 1, 1, 1, 1, 1		//ap == 3 read
	//				r  r  r	 r
	//        s  s        s  s
	};
	
			/* ap:read:rs_flag:user */
	int	pack = ap | read | rs_flag | emu->is_privilege_mode();
	return perms[pack];
	
}

mmu_fault_t arm_mmu::check_access (word_t virt_addr, tlb_entry_t * tlb_e,
	mmu_access_t read)
{
	int access;
	access = domain_access_array[tlb_e->domain];
	if(access == 1) {
		/* client access - check perms */
		int subpage, ap;

		switch (tlb_e->mapping) {
		case TLB_ESMALLPAGE:	//xj
			ap = tlb_e->perms  & 48;
			break;

		case TLB_TINYPAGE:
			ap = tlb_e->perms  & 48;
			break;

		case TLB_SMALLPAGE:
			ap = (tlb_e->perms >> ((virt_addr >> 9) & 6)) & 48;
			break;
		case TLB_LARGEPAGE:
			ap = (tlb_e->perms >> ((virt_addr >> 13) & 6)) & 48;
			break;
		case TLB_SECTION:
			ap = (tlb_e->perms >> 6 ) & 48;
			break;
		default:
			assert (0);
			subpage = 0;	/* cleans a warning */
		}

		if (!check_perms (ap, read)) {
			last_domain= tlb_e->domain;
			if (tlb_e->mapping == TLB_SECTION) {
				return SECTION_PERMISSION_FAULT;
			}
			else {
				return SUBPAGE_PERMISSION_FAULT;
			}
		}
		return NO_FAULT;
	}
	if (access == 3) {			/* access == 3 */
		/* manager access - don't check perms */
		return NO_FAULT;
	}
//	if ((access == 0) || (access == 2)) {
		/* It's unclear from the documentation whether this
		   should always raise a section domain fault, or if
		   it should be a page domain fault in the case of an
		   L1 that describes a page table.  In the ARM710T
		   datasheets, "Figure 8-9: Sequence for checking faults"
		   seems to indicate the former, while "Table 8-4: Priority
		   encoding of fault status" gives a value for FS[3210] in
		   the event of a domain fault for a page.  Hmm. */
		return SECTION_DOMAIN_FAULT;
//	}
}

mmu_fault_t arm_mmu::translation_walk (word_t virt_addr, tlb_entry_t *tlb_e)
{
	/* walk the translation tables */
	int ap[4];
	int fast_access = 1;
	word_t L1addr, L1desc;
	tlb_entry_t entry = *tlb_e;
		
	L1addr = translation_table_base & 0xFFFFC000;
	L1addr = (L1addr | (virt_addr >> 18)) & ~3;
	MEM_READ_WORD (L1addr, &L1desc);

	switch (L1desc & 3) {
	case 0:
		//SBZ
		{
			return PAGE_TRANSLATION_FAULT;
		}
	case 3:
		/* fine page table */
		{
			word_t L2addr, L2desc;

			L2addr = L1desc & 0xFFFFF000;
			L2addr = (L2addr | ((virt_addr & 0x000FFC00) >> 8)) & ~3;
			MEM_READ_WORD (L2addr, &L2desc);

			entry.virt_addr = virt_addr;
			entry.phys_addr = L2desc;
			entry.perms = L2desc & 0x00000FFC;
			entry.domain = (L1desc >> 5) & 0x0000000F;
			ap[3]=(entry.perms>>10) & 3;
			ap[2]=(entry.perms>>8) & 3;
			ap[1]=(entry.perms>>6) & 3;
			ap[0]=(entry.perms>>4) & 3;
			switch (L2desc & 3) {
				case 0:
					last_domain = entry.domain;
					return PAGE_TRANSLATION_FAULT;
				case 3:
					entry.mapping = TLB_TINYPAGE;
					break;
				case 1:
					entry.mapping = TLB_LARGEPAGE;
					/*check every sub-page ap-bit in order to group into 
					one ap-bit*/
					if(ap[3] != ap[2] || ap[2]!=ap[1] || ap[1] != ap[0] ) {
						fast_access =0;	
					}
					break;
				case 2:
					entry.mapping = TLB_SMALLPAGE;
					/*check every sub-page ap-bit in order to group into 
					one ap-bit*/
					if(ap[3] != ap[2] || ap[2]!=ap[1] || ap[1] != ap[0] ) {
						fast_access =0;	
					}
					break;
			}
		}
		break;
	case 1:
		/* coarse page table */
		{
			word_t L2addr, L2desc;

			L2addr = L1desc & 0xFFFFFC00;
			L2addr = (L2addr | ((virt_addr & 0x000FF000) >> 10)) & ~3;
			MEM_READ_WORD (L2addr, &L2desc);

			entry.virt_addr = virt_addr;
			entry.phys_addr = L2desc;
			entry.perms = L2desc & 0x00000FFC;
			entry.domain = (L1desc >> 5) & 0x0000000F;
			ap[3]=(entry.perms>>10) & 3;
			ap[2]=(entry.perms>>8) & 3;
			ap[1]=(entry.perms>>6) & 3;
			ap[0]=(entry.perms>>4) & 3;
			switch (L2desc & 3) {
				case 0:
					last_domain = entry.domain;
					return PAGE_TRANSLATION_FAULT;
				case 3:
					if (emu->is_xscale()) {
						entry.mapping = TLB_ESMALLPAGE;	//xj
						break;
					}
					else {
						last_domain = entry.domain;
						return PAGE_TRANSLATION_FAULT;
					}
				case 1:
					entry.mapping = TLB_LARGEPAGE;
					/*check every sub-page ap-bit in order to group into 
					one ap-bit*/
					if(ap[3] != ap[2] || ap[2]!=ap[1] || ap[1] != ap[0] ) {
						fast_access =0;	
					}
					break;
				case 2:
					entry.mapping = TLB_SMALLPAGE;
					/*check every sub-page ap-bit in order to group into 
					one ap-bit*/
					if(ap[3] != ap[2] || ap[2]!=ap[1] || ap[1] != ap[0] ) {
						fast_access =0;	
					}
					break;
				}
		}
		break;
	case 2:
		/* section */
		entry.virt_addr = virt_addr;
		entry.phys_addr = L1desc;
		entry.perms = L1desc & 0x00000C0C;
		entry.domain = (L1desc >> 5) & 0x0000000F;
		entry.mapping = TLB_SECTION;
		break;
	}

	entry.mask = tlb_masks[entry.mapping];  
	/*	
	Masking bits for both physical address and virtual address consumes 
	more time. The better way is to subtract physical page-base with 
	virtual page-base before hand and the time to convert address will 
	be less 
	
	The non-optimized function should be like this code.
	
	entry.nmask = ~tlb_masks[entry.mapping];  
	entry.phys_addr &= entry.mask;
	*/
	entry.phys_addr = (entry.phys_addr & entry.mask) -
						(entry.virt_addr & entry.mask); 

	/*
	Compute the memory access permision of this tlb entry(page entry)
	for both read access and write access
	*/
	evaluate_access_entry(fast_access,&entry);

	/* commit the change */
	*tlb_e = entry;
	return NO_FAULT;
}

void arm_mmu::evaluate_access_all(void)
{ 
	
	int fast_access;
	tlb_entry_t *tlb_e;
	
	
	FLUSH_MMU_PAGE_TABLE(i_table);		
	FLUSH_MMU_PAGE_TABLE(d_table);		
			
	/*Compute the memory access permision of all entries in ITLB*/
	for(int i=0;i< MMU_ITLB_SIZE;i++) {
		tlb_e =i_tlb->get_entry(i);
		fast_access =1;
	
		if (tlb_e->mapping == TLB_INVALID) continue;
		else if (tlb_e->read_fault == SLOW_ACCESS_CHECKING) fast_access = 0;
		
		evaluate_access_entry(fast_access,tlb_e);
	}
	
	/*Compute the memory access permision of all entries in ITLB*/
	for(int i=0;i< MMU_DTLB_SIZE ;i++) {
		tlb_e =d_tlb->get_entry(i);
		fast_access =1;
	
		if (tlb_e->mapping == TLB_INVALID) continue;
		else if (tlb_e->read_fault == SLOW_ACCESS_CHECKING) fast_access = 0;
		
		evaluate_access_entry(fast_access,tlb_e);
	}
}

void arm_mmu::evaluate_access_entry (int fast_access,tlb_entry_t *tlb_e)
{

	if (tlb_e->mapping == TLB_INVALID) return;

	if (fast_access) {
	/*fast access checking means that this page has one ap-bit 
	  or same ap-bit for each sub-page*/
		tlb_e->read_fault = check_access(0,tlb_e,MMU_READ);
		tlb_e->write_fault = check_access(0,tlb_e,MMU_WRITE);
	}
	else{
	/*slow access checking means that this page has difference
	  ap-bit for each sub-page*/
		tlb_e->read_fault = SLOW_ACCESS_CHECKING;
		tlb_e->write_fault = SLOW_ACCESS_CHECKING;
				
	}
}



word_t arm_mmu::xscale_mrc (word_t instr)
{
	word_t data;
	unsigned creg = (instr>>16)&15;
	unsigned opcode_2 = (instr>>5)&7;
	//unsigned CRm = (instr>>0)&15;
	
	switch (creg) {
		case MMU_ID:
			data = (opcode_2 ? cache_type : emu->get_cpu_id());
			break;
		case MMU_CONTROL:
			data = (opcode_2 ? aux_control : control);
			break;
		case MMU_TRANSLATION_TABLE_BASE:
			data = translation_table_base;
			break;
		case MMU_DOMAIN_ACCESS_CONTROL:
			data = domain_access_control;
			break;
		case MMU_FAULT_STATUS:
			data = fault_status;
			break;
		case MMU_FAULT_ADDRESS:
			data = fault_address;
			break;
		case MMU_PID:
			data = process_id;
			break; //no break in Skyeye
		case XSCALE_CP15_COPRO_ACCESS:
			//printf("xscale cp15 read coprocessor access\n");
			data = copro_access;
			break;
		default:
			data = 0;
			fprintf (stderr, "cp15_mrc read UNKNOWN - reg %d, pc 0x\n", creg);
			exit (-1);
			break;
	}
	return data;
}

void arm_mmu::xscale_mcr(word_t instr, word_t value)
{
	unsigned opcode_2 = (instr>>5)&7;
	unsigned CRm = (instr>>0)&15;
	unsigned creg = (instr>>16)&15;

	switch (creg) {
		case MMU_CONTROL:
			set_control(opcode_2 ? (value & 0x33) : (value & 0x3FFF));
			break;
		case MMU_TRANSLATION_TABLE_BASE:
			translation_table_base = value & 0xFFFFC000;
			FLUSH_MMU_PAGE_TABLE(i_table);		
			FLUSH_MMU_PAGE_TABLE(d_table);		
			
			break;
		case MMU_DOMAIN_ACCESS_CONTROL:
			set_domain_access_control(value);
			break;
		case MMU_FAULT_STATUS:
			fault_status = value & 0x6FF;
			break;
		case MMU_FAULT_ADDRESS:
			fault_address = value;
			break;
		case MMU_CACHE_OPS:
			/*cache is not implemented*/
			//xscale_cp15_cache_ops (state, instr, value);
			break;
		case MMU_TLB_OPS:
			invalidate_tlb(opcode_2,CRm,value);		
			break;

		case MMU_PID:
			set_process_id(value);
			break;
		case XSCALE_CP15_COPRO_ACCESS:
			//printf("xscale cp15 write coprocessor access  val 0x %x\n",value);
			copro_access = value & 0x3ff;
			break;
		default:
			fprintf(stderr,
				"cp15_mcr wrote UNKNOWN - reg %d, reg15 0x\n", creg);
			break;
	}
}
