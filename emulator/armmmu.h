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

#ifndef SIMIT_ARMMMU_H
#define SIMIT_ARMMMU_H

#include "armemul.h"
#include "tlb.h"


namespace simit {

#define ARMul_CP15_R5_ST_ALIGN  0x0001
#define ARMul_CP15_R5_IMPRE     0x0406
#define ARMul_CP15_R5_MMU_EXCPT 0x0400

/* Bits in the control register */
#define CONTROL_MMU             (1<<0)
#define CONTROL_ALIGN_FAULT     (1<<1)
#define CONTROL_CACHE           (1<<2)
#define CONTROL_DATA_CACHE      (1<<2)
#define CONTROL_WRITE_BUFFER    (1<<3)
#define CONTROL_BIG_ENDIAN      (1<<7)
#define CONTROL_SYSTEM          (1<<8)
#define CONTROL_ROM             (1<<9)
#define CONTROL_UNDEFINED            (1<<10)
#define CONTROL_BRANCH_PREDICT       (1<<11)
#define CONTROL_INSTRUCTION_CACHE    (1<<12)
#define CONTROL_VECTOR               (1<<13)
#define CONTROL_RR                   (1<<14)
#define CONTROL_L4                   (1<<15)



typedef enum mmu_size_t {
	MMU_BYTE = 0x1,
	MMU_HALF = 0x2,
	MMU_WORD = 0x4,
} mmu_size_t;

typedef enum mmu_access_t {
	MMU_WRITE = 0x0,
	MMU_READ = 0x8,
} mmu_access_t;

/* FS[3:0] in the fault status register: */
#ifndef SIMIT_TLB_H
typedef enum mmu_fault_t
{
	NO_FAULT = 0x0,
	ALIGNMENT_FAULT = 0x1,
	SECTION_TRANSLATION_FAULT = 0x5,
	PAGE_TRANSLATION_FAULT = 0x7,
	SECTION_DOMAIN_FAULT = 0x9,
	PAGE_DOMAIN_FAULT = 0xB,
	SECTION_PERMISSION_FAULT = 0xD,
	SUBPAGE_PERMISSION_FAULT = 0xF,
	/* this is for speed up checking access permisssion */	
	SLOW_ACCESS_CHECKING = 0x3,
	/* this is for jit self-mofifying code check*/	
	JIT_ALARM_FAULT = 0xff,
} mmu_fault_t;
#endif

typedef struct mmu_page_table_entry_t
{
	unsigned int read_tag;
	unsigned int write_tag;
	word_t phys_addr;
	byte_t *ptr;
} mmu_page_table_entry_t;

/* Register numbers in the MMU: */
typedef enum mmu_regnum_t
{
	MMU_ID = 0,
	MMU_CONTROL = 1,
	MMU_TRANSLATION_TABLE_BASE = 2,
	MMU_DOMAIN_ACCESS_CONTROL = 3,
	MMU_FAULT_STATUS = 5,
	MMU_FAULT_ADDRESS = 6,
	MMU_CACHE_OPS = 7,
	MMU_TLB_OPS = 8,
	MMU_CACHE_LOCKDOWN = 9,
	MMU_TLB_LOCKDOWN = 10,
	MMU_PID = 13,

	/*MMU_V4 */
	MMU_V4_CACHE_OPS = 7,
	MMU_V4_TLB_OPS = 8,

	/*MMU_V3 */
	MMU_V3_FLUSH_TLB = 5,
	MMU_V3_FLUSH_TLB_ENTRY = 6,
	MMU_V3_FLUSH_CACHE = 7,

	/*MMU Intel SA-1100 */
	MMU_SA_RB_OPS = 9,
	MMU_SA_DEBUG = 14,
	MMU_SA_CP15_R15 = 15,

	//chy 2003-08-24
	/*Intel xscale CP15 */
	XSCALE_CP15_CACHE_TYPE = 0,
	XSCALE_CP15_AUX_CONTROL = 1,
	XSCALE_CP15_COPRO_ACCESS = 15,

} mmu_regnum_t;


/*virt_addr exchange according to CP15.R13(process id virtul mapping)*/
#define MMU_PID_VA_MAP_MASK    0xfe000000

#define MMU_ITLB_SIZE 32
#define MMU_DTLB_SIZE 32


#define MMU_HASH_SIZE 256
#define MMU_HASH_INDEX(_a) ((_a>>12) & (MMU_HASH_SIZE - 1))

#define MMU_HASH_MASK 0xfffff000

class arm_mmu {

	public:

		/* constructor */
		arm_mmu (arm_emulator *emula);

		/* destructor */
		~arm_mmu ();

		/* reset */
		void reset ();


		mmu_fault_t read_byte (word_t virt_addr, byte_t  *data){

			mmu_page_table_entry_t * pte = d_table + MMU_HASH_INDEX(virt_addr);
						
			if (pte->read_tag == (virt_addr & mask_array[MMU_BYTE - 1])){
				DIRECT_READ_BYTE((pte->ptr+virt_addr),(*data));
				return NO_FAULT;
			}
			else
				return read_byte_slow(virt_addr,data);
		}

		mmu_fault_t read_hword (word_t virt_addr, hword_t *data){

			mmu_page_table_entry_t * pte = d_table + MMU_HASH_INDEX(virt_addr);
			
			if (pte->read_tag == (virt_addr & mask_array[MMU_HALF - 1])){
				DIRECT_READ_HALF_WORD((pte->ptr+virt_addr),(*data));
				return NO_FAULT;
			}
			else
				return read_hword_slow(virt_addr,data);
		}

		mmu_fault_t read_word (word_t virt_addr, word_t  *data){

			mmu_page_table_entry_t * pte = d_table + MMU_HASH_INDEX(virt_addr);
			
			if (pte->read_tag == (virt_addr & mask_array[MMU_WORD - 1])){
				DIRECT_READ_WORD((pte->ptr+virt_addr),(*data));
				return NO_FAULT;
			}
			else
				return read_word_slow(virt_addr,data);
		}

		mmu_fault_t write_byte (word_t virt_addr, byte_t  data){

			mmu_page_table_entry_t * pte = d_table + MMU_HASH_INDEX(virt_addr);
			
			if (pte->write_tag == (virt_addr & mask_array[MMU_BYTE - 1])){
				DIRECT_WRITE_BYTE((pte->ptr+virt_addr),data);
				return NO_FAULT;
			}
			else
				return write_byte_slow(virt_addr,data);
		}

		mmu_fault_t write_hword (word_t virt_addr, hword_t data) {

			mmu_page_table_entry_t * pte = d_table + MMU_HASH_INDEX(virt_addr);
			
			if (pte->write_tag == (virt_addr & mask_array[MMU_HALF - 1])){
				DIRECT_WRITE_HALF_WORD((pte->ptr+virt_addr),data);
				return NO_FAULT;
			}
			else
				return write_hword_slow(virt_addr,data);
		}

		mmu_fault_t write_word (word_t virt_addr, word_t  data) {

			mmu_page_table_entry_t * pte = d_table + MMU_HASH_INDEX(virt_addr);

			if (pte->write_tag == (virt_addr & mask_array[MMU_WORD - 1])){
				DIRECT_WRITE_WORD((pte->ptr+virt_addr),data);
				return NO_FAULT;
			}
			else
				return write_word_slow(virt_addr,data);
		}


		mmu_fault_t load_instr (word_t virt_addr, word_t  *instr) {

			mmu_page_table_entry_t * pte = i_table + MMU_HASH_INDEX(virt_addr);
			
			if (pte->read_tag == (virt_addr & MMU_HASH_MASK)){
				DIRECT_READ_WORD((pte->ptr+virt_addr),(*instr));
				return NO_FAULT;
			}
			else
				return load_instr_slow(virt_addr,instr);
		}
		
		/* translates virtual instruction addr into physical address 
			for jit directory */
		mmu_fault_t translate_instr_addr (word_t virt_addr, word_t *phys_addr, 
			byte_t **ptr){

			mmu_page_table_entry_t * pte = i_table + MMU_HASH_INDEX(virt_addr);

			if (pte->read_tag == (virt_addr & MMU_HASH_MASK)){
				*phys_addr = (pte->phys_addr + virt_addr);
				*ptr = (pte->ptr + virt_addr); 	
				return NO_FAULT;
			}
			else{
				*ptr = NULL;
				return translate_instr_addr_slow (virt_addr, phys_addr);
			}
		}

		/* The MMU(copro 15) is accessible via MCR and MRC operations */
		word_t mrc (word_t instr);
		void mcr (word_t instr, word_t value);
		
		/* evaluate all tlb entris access permision due to condition changeing*/
		void evaluate_access_all	(void);
		
		word_t high_vector(word_t vector) {
			if (control & CONTROL_VECTOR)
				return(vector + 0xffff0000); //for v4 high exception address
			else
				return(vector);	//for normal  address
		}

		void update_fsr_far (mmu_fault_t fault, word_t addr) {
			fault_status =  (fault | (last_domain << 4))& 0xFF;
			fault_address = addr;
		}
	

	
		/*XScale functions*/
	
		void set_xscale(void){	
			set_control(0);
			cache_type = 0xB1AA1AA;  //0000 1011 0001 1010 1010 0001 1010 1010
			aux_control = 0;
		}

		word_t xscale_mrc (word_t instr);
		void xscale_mcr (word_t instr, word_t value);
		
		void xscale_update_fsr_far (word_t fsr, word_t far){
			if (emu->is_xscale()) {
				fault_status = fsr & 0x6FF;
				fault_address = far;
			}
		}
			
		bool cp_access_allowed (unsigned cpnum) {
			if (emu->is_xscale())
				return (copro_access & (1 << cpnum));
			else
				return true;
		}	


	private:

		/* translates virtual instruction addr into physical address */
		mmu_fault_t translate_data_addr_slow (word_t virt_addr,
				mmu_access_t read, mmu_size_t s,word_t *phys_addr);
		
		/* data access routines */
		mmu_fault_t read_byte_slow    (word_t virt_addr, byte_t  *data);
		mmu_fault_t read_hword_slow   (word_t virt_addr, hword_t *data);
		mmu_fault_t read_word_slow    (word_t virt_addr, word_t  *data);
		mmu_fault_t write_byte_slow   (word_t virt_addr, byte_t  data);
		mmu_fault_t write_hword_slow  (word_t virt_addr, hword_t data);
		mmu_fault_t write_word_slow   (word_t virt_addr, word_t  data);

		/* translates virtual instruction addr into physical address */
		mmu_fault_t translate_instr_addr_slow (word_t virt_addr,
			word_t *phys_addr);
	
		/* load instruction uses itlb */
		mmu_fault_t load_instr_slow (word_t virt_addr, word_t *instr);
		

		/*retrive the page table entry from memory and save to designate TLB*/
		mmu_fault_t translation_walk (word_t virt_addr, tlb_entry_t *tlb_e);
		
		/*access permission calculating functions*/
		void evaluate_access_entry	(int fast_access, tlb_entry_t *tlb_e);
		mmu_fault_t check_access	(word_t virt_addr, tlb_entry_t *e,
									mmu_access_t read);
		int check_perms	(int ap, mmu_access_t rread) const;
		
		/*set the value of control register*/
		void set_control(word_t value);
		
		/*set the value of process_id register*/
		void set_process_id(word_t value);

		/*set the value of domain access control register*/
		void set_domain_access_control(word_t value){
			if (domain_access_control != value){
				domain_access_control = value;
				for(int i=0;i<16;i++){
					domain_access_array[i]=value & 3;
					value= value>>2;
				}
				evaluate_access_all();
			}
		}

		/*Fast Context Switch Extension (FCSE)*/
		word_t mmu_pid_va_map(word_t va) {
			if (process_id == 0)
				return va;
			else if (va & MMU_PID_VA_MAP_MASK)
				return va; 
			else 
				return va | process_id;
		}

		void invalidate_tlb(word_t opcode2,word_t CRm,word_t value);

		void add_mmu_page_entry(mmu_page_table_entry_t *x_table,
				tlb_entry_t *tlb_e,word_t phys_addr,word_t virt_addr);
		void remove_mmu_page_entry(mmu_page_table_entry_t *x_table,
				tlb_entry_t *tlb_e);
		

	protected:

		arm_emulator *emu;

		uint64_t fault_count;

		word_t control;
		word_t rs_flag;

		word_t domain_access_array[16];
		word_t domain_access_control;
		word_t fault_status;
		word_t fault_address;
		word_t process_id;
		word_t cache_locked_down;
		word_t tlb_locked_down;
		word_t last_domain;
		word_t translation_table_base;


		//chy 2003-08-24 for xscale
		word_t cache_type;	// 0
		word_t aux_control;	// 1
		word_t copro_access;	// 15

		/* tlbs */
		tlb<MMU_ITLB_SIZE> *i_tlb;
		tlb<MMU_DTLB_SIZE> *d_tlb;

		/* FAST LOOK UP TABLE*/
		mmu_page_table_entry_t	i_table[MMU_HASH_SIZE];
		mmu_page_table_entry_t	d_table[MMU_HASH_SIZE];
	
		tlb_entry_t dummy_entry;

		word_t mask_array[4];

	public:

		/* Below is a special interface created for compiled simulation in
		   system level. In compiled simulation, after a block of instruction
		   is translated into a DLL, the block must be marked so that
		   when a write to the block occurs, the simulator is notified so
		   that the DLL is unloaded. We define an interface for the
		   jit simulator to communicate with the MMU.
		*/

		/* call (*fcheck)(wchecker, addr) when loading an MMU cache entry,
		   if it returns true, meaning the address is compiled, then
		   set write_tag to an invalid value.
		   In write_XXX_slow, call (*falarm)(wchecker, addr) to notify
		   the simulator that an address is updated. If it returns true,
		   then the write_slow function should return a JIT_ALARM_FAULT
		   without updating the memory.

		   Note addr should be physical address.
		*/
		typedef bool (wcheck_t)(void *, arm_addr_t);
		typedef bool (walarm_t)(void *, arm_addr_t);

		void register_write_checker(void * e, wcheck_t * fc, walarm_t * fa) {
			wchecker = e;
			fcheck = fc;
			falarm = fa;
		}
	
		/* jit simulator calls this when a block is scheduled to be compiled */
		void invalidate_write_cache(arm_addr_t virt_addr) {
			mmu_page_table_entry_t * pte = d_table + MMU_HASH_INDEX(virt_addr);
			if (pte->write_tag == (virt_addr & MMU_HASH_MASK))
				pte->write_tag = 0xFFFFFFFF;
		}
	
	private:

		void *wchecker;
		wcheck_t *fcheck;
		walarm_t *falarm;

};

} /* namespace */

#endif /* SIMIT_ARMMMU_H */
