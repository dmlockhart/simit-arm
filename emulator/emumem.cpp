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

#include "emumem.h"

#include <cassert>
#include <csignal>
#include <cstring>
#include <cstdlib>

using namespace simit;

memory::memory(unsigned int def_perm, int sig)
{
	def_perm &= MEMORY_PAGE_PERM_MASK;

	/* if nonempty permission, then all pages are valid by default */
	if (def_perm) 
	{
		for (unsigned int ii=0; ii<MEMORY_PAGE_TABLE_SIZE; ii++)
			page_table[ii].flag = 
				MEMORY_PAGE_EMPTY | (def_perm << MEMORY_PAGE_PERM_SHIFT);
	}
	/* otherwise all pages are invalid by default */
	else 
	{
		for (unsigned int ii=0; ii<MEMORY_PAGE_TABLE_SIZE; ii++)
			page_table[ii].flag = MEMORY_PAGE_UNAVAIL;
	}

	page_count = 0;
	fault_sig = sig;
}

memory::~memory()
{
	for(unsigned int ii = 0; ii < MEMORY_PAGE_TABLE_SIZE; ii++)
	{
		memory_page_table_entry_t *pte = page_table + ii;
		if (page_allocated(pte)) 
			free_page(pte);
	}

}

memory::memory(const memory& mem)
{

	page_count = 0;
	fault_sig = mem.fault_sig;

	for(unsigned ii = 0; ii < MEMORY_PAGE_TABLE_SIZE; ii++)
	{
		page_table[ii].flag = mem.page_table[ii].flag;
		if (page_allocated(mem.page_table + ii))
		{
			page_table[ii].ptr = (byte_t *)malloc(MEMORY_PAGE_SIZE);
			if (page_table[ii].ptr == NULL)
			{
				report_fault(MEM_NOMEMORY_FAULT, 0);
				return;
			}

			memcpy(page_table[ii].ptr,
				mem.page_table[ii].ptr + (ii << MEMORY_PAGE_SIZE_BITS),
				MEMORY_PAGE_SIZE);
			page_table[ii].ptr -= ii << MEMORY_PAGE_SIZE_BITS;
			page_count++;
		}
	}
}

void memory::reset()
{
	for(unsigned int ii = 0; ii < MEMORY_PAGE_TABLE_SIZE; ii++)
	{
		memory_page_table_entry_t *pte = page_table + ii;
		if (page_allocated(pte)) 
		{
			free_page(pte);
		}
		else if (page_remapped(pte))
		{
			reinterpret_cast<memory_ext_interface *>(pte->ptr)->reset();
		}
	}
	page_count = 0;
}



unsigned int memory::get_page_permission(target_addr_t addr) const
{
	const memory_page_table_entry_t *pte = get_page(addr);
	unsigned int flag = pte->flag;
	if (flag & MEMORY_PAGE_SLOW)
		flag >>= MEMORY_PAGE_PERM_SHIFT;
	return (flag & MEMORY_PAGE_PERM_MASK);
}

void memory::set_page_permission(target_addr_t addr, unsigned int perm)
{
	memory_page_table_entry_t *pte = get_page(addr);

	// do nothing if the page is not available
	if (pte->flag & MEMORY_PAGE_UNAVAIL) return;

	// otherwise set permission
	perm &= MEMORY_PAGE_PERM_MASK;
	if (pte->flag & MEMORY_PAGE_SLOW) 
	{
		perm = perm << MEMORY_PAGE_PERM_SHIFT;        
		pte->flag &= ~(MEMORY_PAGE_PERM_MASK << MEMORY_PAGE_PERM_SHIFT);
		pte->flag |= perm;
	}
	else
	{
		pte->flag &= ~MEMORY_PAGE_PERM_MASK;
		pte->flag |= perm;
	}
}

void memory::set_region_permission(target_addr_t addr,
	unsigned int size, unsigned int perm)
{
	target_addr_t end_addr = /* avoid overflow */
		(addr + size) < addr ? (target_addr_t)-1 : (addr + size);

	addr = align_to_page_boundary(addr);
	while (addr < end_addr)
	{
		set_page_permission(addr, perm);
		addr += MEMORY_PAGE_SIZE;
		if (addr == 0) break;	 /* no wrap-around */
	}
}


void memory::remap_page(target_addr_t addr, 
	memory_ext_interface *mif, unsigned int perm)
{
	memory_page_table_entry_t *pte = get_page(addr);

	/* free space if the page is allocated some space */
	if (page_allocated(pte))
		free_page(pte);

	pte->flag = MEMORY_PAGE_REMAPPED; 
	pte->flag |= (perm & MEMORY_PAGE_PERM_MASK) << MEMORY_PAGE_PERM_SHIFT;
	pte->ptr = reinterpret_cast<byte_t *>(mif);
}

void memory::unmap_page(target_addr_t addr)
{
	memory_page_table_entry_t *pte = get_page(addr);
	if (pte->flag & MEMORY_PAGE_REMAPPED)
		pte->flag = MEMORY_PAGE_UNAVAIL;
}

void memory::remap_region(target_addr_t addr, unsigned int size,
	memory_ext_interface *mif, unsigned int perm)
{
	target_addr_t end_addr = /* avoid overflow */
		(addr + size) < addr ? (target_addr_t)-1 : (addr + size);

	addr = align_to_page_boundary(addr);
	while (addr < end_addr)
	{
		remap_page(addr, mif, perm);
		addr += MEMORY_PAGE_SIZE;
		if (addr == 0) break;	 /* no wrap-around */
	}
}

void memory::unmap_region(target_addr_t addr, unsigned int size)
{
	target_addr_t end_addr = /* avoid overflow */
		(addr + size) < addr ? (target_addr_t)-1 : (addr + size);

	addr = align_to_page_boundary(addr);
	while (addr < end_addr)
	{
		unmap_page(addr);
		addr += MEMORY_PAGE_SIZE;
		if (addr == 0) break;	 /* no wrap-around */
	}
}



void memory::mark_page_available(target_addr_t addr, unsigned int perm)
{
	memory_page_table_entry_t *pte = get_page(addr);
	perm &= MEMORY_PAGE_PERM_MASK;

	/* if already available, change the permission only */
	if (pte->flag & MEMORY_PAGE_UNAVAIL) 
		pte->flag = MEMORY_PAGE_EMPTY | (perm << MEMORY_PAGE_PERM_SHIFT);
	else
		set_page_permission(addr, perm);
}

void memory::mark_page_unavailable(target_addr_t addr)
{
	memory_page_table_entry_t *pte = get_page(addr);

	/* free space if the page is allocated some space */
	if (page_allocated(pte))
		free_page(pte);

	pte->flag = MEMORY_PAGE_UNAVAIL; 
}

void memory::mark_region_available(target_addr_t addr,
	unsigned int size, unsigned int perm)
{
	target_addr_t end_addr = /* avoid overflow */
		(addr + size) < addr ? (target_addr_t)-1 : (addr + size);

	addr = align_to_page_boundary(addr);
	while (addr < end_addr)
	{
		mark_page_available(addr, perm);
		addr += MEMORY_PAGE_SIZE;
		if (addr == 0) break;	 /* no wrap-around */
	}
}

void memory::mark_region_unavailable(target_addr_t addr, unsigned int size)
{
	target_addr_t end_addr = /* avoid overflow */
		(addr + size) < addr ? (target_addr_t)-1 : (addr + size);

	addr = align_to_page_boundary(addr);
	addr = align_to_page_boundary(addr);
	while (addr < end_addr)
	{
		mark_page_unavailable(addr);
		addr += MEMORY_PAGE_SIZE;
		if (addr == 0) break;	 /* no wrap-around */
	}
}



memory_fault_t memory::allocate_page(memory_page_table_entry_t *pte)
{
	assert(!page_allocated(pte));

	pte->ptr = (byte_t *)calloc(MEMORY_PAGE_SIZE, 1);
	if (pte->ptr==NULL)
		return MEM_NOMEMORY_FAULT;

	// inherit the permission bits if any
	pte->flag = (pte->flag >> MEMORY_PAGE_PERM_SHIFT) & MEMORY_PAGE_PERM_MASK;

	// subtract the starting address from the ptr so that it is easier
	// to compute the actual address, see translate() for clue
	pte->ptr -= (pte - page_table) << MEMORY_PAGE_SIZE_BITS;
	page_count++;
	return MEM_NO_FAULT;
}


void memory::free_page(memory_page_table_entry_t *pte)
{
	assert(page_allocated(pte));

	free (pte->ptr + ((pte - page_table) << MEMORY_PAGE_SIZE_BITS));
	pte->flag = MEMORY_PAGE_EMPTY | 
		((pte->flag & MEMORY_PAGE_PERM_MASK) << MEMORY_PAGE_PERM_SHIFT);

	page_count--;
}



memory_fault_t memory::read_block_within_page(void *buf, target_addr_t addr,
		unsigned int size, unsigned int *psize)
{
	memory_fault_t fault;
	memory_page_table_entry_t *pte = get_page(addr);

	*psize = 0;
	if ((fault = check_page_permission(pte, MEMORY_PAGE_READABLE)) !=
			MEM_NO_FAULT)
	{
		report_fault(fault, addr);
		return fault;
	}

	// I/O remapped space does not understand block operation
	if (pte->flag & MEMORY_PAGE_REMAPPED)
	{
		report_fault(MEM_BADADDR_FAULT, addr);
		return MEM_BADADDR_FAULT;
	}


	/* check if crossing page boundary */
	if ((addr >> MEMORY_PAGE_SIZE_BITS) !=
		((addr + size - 1) >> MEMORY_PAGE_SIZE_BITS))
		size = MEMORY_PAGE_SIZE - (addr & (MEMORY_PAGE_SIZE - 1));

	if ((pte->flag & MEMORY_PAGE_EMPTY) && (fault = allocate_page(pte)) !=
			MEM_NO_FAULT)
	{
		report_fault(fault, addr);
		return fault;
	}

	memcpy(buf, pte->ptr + addr, size);
	*psize = size;
	return MEM_NO_FAULT;
}

memory_fault_t memory::read_block(void *buf, target_addr_t addr,
		unsigned int size)
{
	memory_fault_t ft;
	unsigned int sz, len = 0;

	/* crossing the page boundary */
	while (len < size)
	{
		if ((ft = read_block_within_page(buf, addr+len, size-len, &sz)) !=
			MEM_NO_FAULT)
			return ft;

		len += sz;
		buf = reinterpret_cast<byte_t *>(buf) + sz;
	}
	return MEM_NO_FAULT;
}

memory_fault_t memory::write_block_within_page(void *buf, target_addr_t addr,
		unsigned int size, unsigned int *psize)
{
	memory_fault_t fault;
	memory_page_table_entry_t *pte = get_page(addr);

	*psize = 0;
	if ((fault = check_page_permission(pte, MEMORY_PAGE_WRITABLE)) !=
			MEM_NO_FAULT)
	{
		report_fault(fault, addr);
		return fault;
	}

	if (pte->flag & MEMORY_PAGE_REMAPPED)
	{
		report_fault(MEM_BADADDR_FAULT, addr);
		return MEM_BADADDR_FAULT;
	}

	/* check if crossing page boundary */
	if ((addr >> MEMORY_PAGE_SIZE_BITS) !=
		((addr + size - 1) >> MEMORY_PAGE_SIZE_BITS))
		size = MEMORY_PAGE_SIZE - (addr & (MEMORY_PAGE_SIZE - 1));

	if ((pte->flag & MEMORY_PAGE_EMPTY) && (fault = allocate_page(pte)) !=
			MEM_NO_FAULT)
	{
		report_fault(fault, addr);
		return fault;
	}

	memcpy(pte->ptr + addr, buf, size);
	*psize = size;
	return MEM_NO_FAULT;
}

memory_fault_t memory::write_block(void *buf, target_addr_t addr,
		unsigned int size)
{
	memory_fault_t ft;
	unsigned int sz, len = 0;

	/* crossing the page boundary */
	while (len < size)
	{
		if ((ft = write_block_within_page(buf, addr+len, size-len, &sz)) !=
			MEM_NO_FAULT)
			return ft;

		len += sz;
		buf = reinterpret_cast<byte_t *>(buf) + sz;
	}
	return MEM_NO_FAULT;
}

memory_fault_t memory::set_block_within_page(target_addr_t addr, byte_t value, 
		unsigned int size, unsigned int *psize)
{
	memory_fault_t fault;
	memory_page_table_entry_t *pte = get_page(addr);

	*psize = 0;
	if ((fault = check_page_permission(pte, MEMORY_PAGE_WRITABLE)) !=
			MEM_NO_FAULT)
	{
		report_fault(fault, addr);
		return fault;
	}

	if (pte->flag & MEMORY_PAGE_REMAPPED)
	{
		report_fault(MEM_BADADDR_FAULT, addr);
		return MEM_BADADDR_FAULT;
	}

	/* check if crossing page boundary */
	if ((addr >> MEMORY_PAGE_SIZE_BITS) !=
		((addr + size - 1) >> MEMORY_PAGE_SIZE_BITS))
		size = MEMORY_PAGE_SIZE - (addr & (MEMORY_PAGE_SIZE - 1));

	if ((pte->flag & MEMORY_PAGE_EMPTY) && (fault = allocate_page(pte)) !=
			MEM_NO_FAULT)
	{
		report_fault(fault, addr);
		return fault;
	}

	memset(pte->ptr + addr, value, size);
	*psize = size;
	return MEM_NO_FAULT;
}


memory_fault_t memory::set_block(target_addr_t addr, byte_t value,
		unsigned int size)
{
	memory_fault_t ft;
	unsigned int sz, len = 0;

	/* crossing the page boundary */
	while (len < size)
	{
		if ((ft = set_block_within_page(addr+len, value, size-len, &sz)) !=
				MEM_NO_FAULT)
			return ft;

		len += sz;
	}
	return MEM_NO_FAULT;
}


memory_fault_t memory::check_page_permission
	(const memory_page_table_entry_t *pte, unsigned int perm) const
{
	if (pte->flag & MEMORY_PAGE_UNAVAIL)
		return MEM_BADADDR_FAULT;

	unsigned int priv = pte->flag & MEMORY_PAGE_PERM_MASK;
	if (pte->flag & MEMORY_PAGE_AVAIL_SLOW) 
		priv = (pte->flag >> MEMORY_PAGE_PERM_SHIFT) & MEMORY_PAGE_PERM_MASK;

	perm &= ~priv;
	return (memory_fault_t)perm;
}

memory_fault_t memory::check_page_permission(target_addr_t addr,
		unsigned int perm) const
{
	return check_page_permission(get_page(addr), perm);
}



template <class T>
static memory_fault_t remap_read(memory_ext_interface *intf,
	target_addr_t addr, T *pval)
{
	return MEM_NO_FAULT;
}

#define REMAP_READ_INSTANTIATE(retyp) \
template <> \
memory_fault_t remap_read<retyp ## _t>(memory_ext_interface *intf, \
	target_addr_t addr, retyp ## _t *pval) \
{\
	return intf->read_ ## retyp (addr, pval); \
}

REMAP_READ_INSTANTIATE(byte)
REMAP_READ_INSTANTIATE(hword)
REMAP_READ_INSTANTIATE(word)
REMAP_READ_INSTANTIATE(dword)


template <class T> 
static memory_fault_t remap_write(memory_ext_interface *intf,
	target_addr_t addr, T val)
{
	return MEM_NO_FAULT;
}

#define REMAP_WRITE_INSTANTIATE(retyp) \
template <> \
memory_fault_t remap_write<retyp ## _t>(memory_ext_interface *intf, \
	target_addr_t addr, retyp ## _t val) \
{ \
	return intf->write_ ## retyp (addr, val); \
}


REMAP_WRITE_INSTANTIATE(byte)
REMAP_WRITE_INSTANTIATE(hword)
REMAP_WRITE_INSTANTIATE(word)
REMAP_WRITE_INSTANTIATE(dword)


template <class T>
memory_fault_t memory::read_slow_base(target_addr_t addr, T *pval)
{
	memory_fault_t fault;
	T value;

	// check page boundary
	if (sizeof(T) > 1 && cross_page_boundary(addr, sizeof(T)))
	{
		fault = read_block(reinterpret_cast<void *>(&value), addr, sizeof(T));
	}
	else
	{
		memory_page_table_entry_t *pte = get_page(addr);

		if ((fault = check_page_permission(pte, MEMORY_PAGE_READABLE)) !=
					MEM_NO_FAULT) 
		{
			*pval = 0;	// make pval deterministic so convenient to debug
			report_fault(fault, addr);
			return fault;	
		}

		// check storage allocation
		if (pte->flag & MEMORY_PAGE_EMPTY) 
		{
			if ((fault = allocate_page(pte)) != MEM_NO_FAULT)
			{
				report_fault(fault, addr);
				return fault;
			}
			value = *reinterpret_cast<T *>(pte->ptr + addr);
		}
		else //if (pte->flag & MEMORY_PAGE_REMAPPED) 
		{
			/* I/O should take care of endianness by itself */
			return remap_read<T>
				(reinterpret_cast<memory_ext_interface *>(pte->ptr),
					addr, pval);
		}
	}
#if WORDS_BIGENDIAN==TARGET_LITTLE_ENDIAN
	*pval = swap(value);
#else
	*pval = value;
#endif

	return fault;
}

template <class T>
memory_fault_t memory::write_slow_base(target_addr_t addr, T value)
{
	T val;

#if WORDS_BIGENDIAN==TARGET_LITTLE_ENDIAN
	val = swap(value);
#else
	val = value;
#endif

	// check page boundary
	if (sizeof(T) > 1 && cross_page_boundary(addr, sizeof(T)))
	{
		return write_block(&val, addr, sizeof(T));
	}
	else
	{
		memory_fault_t fault;
		memory_page_table_entry_t *pte = get_page(addr);
		if ((fault = check_page_permission(pte, MEMORY_PAGE_WRITABLE)) !=
					MEM_NO_FAULT)
		{
			report_fault(fault, addr);
			return fault;	
		}

		// check storage allocation
		if (pte->flag & MEMORY_PAGE_EMPTY)
		{
			if ((fault = allocate_page(pte)) != MEM_NO_FAULT)
			{
				report_fault(fault, addr);
				return fault;
			}
			*reinterpret_cast<T *>(pte->ptr + addr) = val;
			return MEM_NO_FAULT;
		}
		else //if (pte->flag & MEMORY_PAGE_REMAPPED) 
		{
			/* I/O should take care of endianness by itself */
			return remap_write<T>
				(reinterpret_cast<memory_ext_interface *>(pte->ptr),
					addr, value);
		}
	}
}

memory_fault_t memory::read_slow(target_addr_t addr, byte_t *pval)
{
	return read_slow_base<byte_t>(addr, pval);
}

memory_fault_t memory::write_slow(target_addr_t addr, byte_t value)
{
	return write_slow_base<byte_t>(addr, value);
}

memory_fault_t memory::read_slow(target_addr_t addr, hword_t *pval)
{
	return read_slow_base<hword_t>(addr, pval);
}

memory_fault_t memory::write_slow(target_addr_t addr, hword_t value)
{
	return write_slow_base<hword_t>(addr, value);
}

memory_fault_t memory::read_slow(target_addr_t addr, word_t *pval)
{
	return read_slow_base<word_t>(addr, pval);
}

memory_fault_t memory::write_slow(target_addr_t addr, word_t value)
{
	return write_slow_base<word_t>(addr, value);
}

memory_fault_t memory::read_slow(target_addr_t addr, dword_t *pval)
{
	return read_slow_base<dword_t>(addr, pval);
}

memory_fault_t memory::write_slow(target_addr_t addr, dword_t value)
{
	return write_slow_base<dword_t>(addr, value);
}



void memory::report_fault(memory_fault_t fault, target_addr_t faddr)
{
	last_fault = fault;
	last_faddr = faddr;
	if (fault_sig!=-1) raise(fault_sig);
}


#ifdef TEST_MEMORY
#include <cstring>
#include <iostream>

bool test(memory *mem)
{
	unsigned int i;
	bool ret = true;

	/* test half_word */
	hword_t hw;
	for (i=0; i<1024;i+=2)
	{
		if (mem->write_hword(i, (hword_t)i)) 
		{
			ret = false;
			std::cerr << "hword write failed at " << i << std::endl;
		}
	}

	for (i=0; i<1024;i+=2) 
	{
		byte_t b1, b2;
		if (mem->read_hword(i, &hw) || hw!=(hword_t)i ||
			mem->read_byte(i, &b1) || b1 != (byte_t)i ||
			mem->read_byte(i+1, &b2) || b2 != (byte_t)(i>>8)) 
		{
			ret = false;
			std::cerr << "hword test failed at " << i << ", get " << hw
				  << std::endl;
		}

	}

	/* test word */
	word_t w;
	for (i=0; i<1024;i+=4) 
	{
		if (mem->write_word(i, (word_t)i))
		{
			ret = false;
			std::cerr << "word write failed at " << i << std::endl;
		}
	}

	for (i=0; i<1024;i+=4) 
	{
		byte_t b1, b2;
		if (mem->read_word(i, &w) || w!=i ||
			mem->read_byte(i, &b1) || b1!=(byte_t)i ||
			mem->read_byte(i+3, &b2) || b2!=(byte_t)(i>>24))
		{
			ret = false;
			std::cerr << "word test failed at " << i << ", get " << w
				  << std::endl;
		}
	}

	/* test dword */
	dword_t dw;
	for (i=0; i<1024;i+=8)
	{
		if (mem->write_dword(i, ((dword_t)i<<32)|i))
		{
			ret = false;
			std::cerr << "dword write failed at " << i << std::endl;
		}
	}

	for (i=0; i<1024;i+=8) 
	{
		byte_t b1, b2;
		if (mem->read_dword(i, &dw) || dw!=(((dword_t)i<<32)|i) ||
			mem->read_byte(i, &b1) || b1!=(byte_t)i ||
			mem->read_byte(i+7, &b2) || b2!=(byte_t)(i>>24)) 
		{
			ret = false;
			std::cerr << "dword test failed at " << i << ", get " << dw 
				  << std::endl;
		}
	}

	/* alignment test at page boundary*/
	for (i=0; i<8; i++)
	{
		mem->write_word(MEMORY_PAGE_SIZE-i, 0xAABBCCDD);
		mem->read_word(MEMORY_PAGE_SIZE-i, &w);
		if (w != 0xAABBCCDD)
		{
			ret = false;
			std::cerr << "word alignment test failed at " << i << ", get " << w
				<< std::endl;
		}
	}


	/* alignment test at page boundary*/
	for (i=0; i<16; i++)
	{
		mem->write_dword(MEMORY_PAGE_SIZE-i, 0x66778899AABBCCDDULL);
		mem->read_dword(MEMORY_PAGE_SIZE-i, &dw);
		if (dw != 0x66778899AABBCCDDULL)
		{
			ret = false;
			std::cerr << "dword alignment test failed at " << i << ", get "
				<< dw << std::endl;
		}
	}

	/* alignment test at page boundary*/
	for (i=0; i<4; i++) 
	{
		mem->write_hword(MEMORY_PAGE_SIZE-i, 0x6677);
		mem->read_hword(MEMORY_PAGE_SIZE-i, &hw);
		if (hw != 0x6677) 
		{
			ret = false;
			std::cerr << "hword alignment test failed at " << i << ", get "
				<< hw << std::endl;
		}
	}

	/* test block */
	char hello[] = "hello world!\n";
	char bufff[sizeof(hello)];
	

	mem->write_block(hello, MEMORY_PAGE_SIZE-1, sizeof(hello));
	mem->read_block(bufff, MEMORY_PAGE_SIZE-1, sizeof(hello));

	for (i=0; i<sizeof(hello);i+=8)
		if (bufff[i]!=hello[i]) 
		{
			ret = false;
			std::cerr << "block test failed at " << i << ", get " << bufff[i]
				  << std::endl;
		}

	/* test copy constructor using word */
	for (i=0; i<1024;i+=4)
		mem->write_word(i, (word_t)i);

	memory mem_cpy(*mem);
	for (i=0; i<1024;i+=4) 
	{
		byte_t b1, b2;
		if (mem_cpy.read_word(i, &w) || w!=i ||
			mem_cpy.read_byte(i, &b1) || b1!=(byte_t)i ||
			mem_cpy.read_byte(i+3, &b2) || b2!=(byte_t)(i>>24)) 
		{
			ret = false;
			std::cerr << "copy test failed at " << i << ", get " << w
				  << std::endl;
		}
	}

	if (mem_cpy.get_page_count() != mem->get_page_count()) 
	{
		std::cerr << "page count mismatch after copying" << std::endl;
		ret = false;
	}

	return ret;
}

int main()
{
	memory amem;
	word_t w;
	printf(".");
	amem.read_word(0, &w);
	printf("%d", w);
	if (test(&amem))
		std::cerr << "memory test passed!" << std::endl;
	else
		std::cerr << "memory test failed!" << std::endl;
}
#endif

