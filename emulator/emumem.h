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

#ifndef SIMIT_MEMORY_H
#define SIMIT_MEMORY_H

#include <config.h>
#include <misc.h>

namespace simit {

/* This file implements a 32-bit memory emulator.
   It uses a single-level software page table to translate
   the input address to the allocated storage address.
   One can set read/write permissions for individual pages.
   The emulator supports an extension interface, allowing one
   to supply interface-functions for an address range. This is useful
   to remap memory addresses to I/O.

   Limitations: 
   1. Permission and I/O remapping are only controllable at the
   page level. One can reduce the size of the page to get finer control,
   but the size of the page table will grow. By default, a page is 64KB.
   The page table has 64K entries. 
   2. Remapped pages do not accept block operations.
*/

/* Each page table entry contains two elements, a pointer to the allocated
   storage (or the user supplied interface object), and a flag word indicating
   the protection property of the page. The meanings of the bits of the
   flag are explained below.

   bit --> meaning

   0   --> normal but empty page, meaning storage not allocated yet
   1   --> remapped memory, meaning external interfaces is defined
   2   --> bad address, bus error

   4   --> normal page, readable
   5   --> normal page, writable
   6   --> normal page, executable, not used since hardware doesnot care

   4, 5, 6 can be combined.

   Bit 0, 1, 2 are exclusive. If they are all clear, the page
   is normal and has allocated storage space.
   IF ANY OF THESE BITS IS SET, THEN 4, 5, 6 WILL BE STEALTHILY
   SHIFTED BY 12 BITS TO THE LEFT TO BE HIDDEN.
   The reason to hide these bits is for better performance.
   For the special cases when one of those bits are set,
   a test of the permission bits will fail (since they are hidden),
   and a slow but full-featured access function is called.
   For the normal case, a test of these bits will succeed so that a
   fast access is performed.
*/

#undef CHECK_PAGE_BOUNDARY

const unsigned int MEMORY_PAGE_EMPTY      = 1;
const unsigned int MEMORY_PAGE_REMAPPED   = 2;
const unsigned int MEMORY_PAGE_UNAVAIL    = 4;
const unsigned int MEMORY_PAGE_READABLE   = 16;
const unsigned int MEMORY_PAGE_WRITABLE   = 32;
const unsigned int MEMORY_PAGE_EXECUTABLE = 64; // not really used

/* internal macros, user should not use these */
#define MEMORY_PAGE_PERM_SHIFT 12
#define MEMORY_PAGE_PERM_MASK  \
	(MEMORY_PAGE_READABLE | MEMORY_PAGE_WRITABLE | MEMORY_PAGE_EXECUTABLE)
#define MEMORY_PAGE_SLOW \
	(MEMORY_PAGE_EMPTY | MEMORY_PAGE_REMAPPED | MEMORY_PAGE_UNAVAIL)
#define MEMORY_PAGE_AVAIL_SLOW \
	(MEMORY_PAGE_EMPTY | MEMORY_PAGE_REMAPPED)

/* these are memory paging parameter, default page size = 64KB */
const unsigned int MEMORY_PAGE_SIZE_BITS  = 16;
const unsigned int MEMORY_PAGE_SIZE       = 1 << MEMORY_PAGE_SIZE_BITS;
const unsigned int MEMORY_PAGE_TABLE_BITS = 
	sizeof(target_addr_t) * 8 - MEMORY_PAGE_SIZE_BITS;
const unsigned int MEMORY_PAGE_TABLE_SIZE = 1 << MEMORY_PAGE_TABLE_BITS;


#define MEMORY_MAX_ADDR  ((target_addr_t)(-1))
#define MEMORY_MIN_ADDR  0

/* These are the bits in fault code */
typedef enum {
	MEM_NO_FAULT                = 0,
	MEM_READ_PERM_FAULT         = MEMORY_PAGE_READABLE, //16
	MEM_WRITE_PERM_FAULT        = MEMORY_PAGE_WRITABLE, //32
	MEM_EXEC_PERM_FAULT         = MEMORY_PAGE_EXECUTABLE, //64
	MEM_BADADDR_FAULT           = MEMORY_PAGE_UNAVAIL, //4
	MEM_MISALIGN_FAULT          = 128,
	MEM_NOMEMORY_FAULT          = 256, //out of memory
} memory_fault_t;

/* an abstract class as page remapping interface */
class memory_ext_interface {

  public:

	virtual ~memory_ext_interface() {}

	virtual void reset() = 0;

	/* Didn't use overloading in interface to avoid bugs */
	virtual memory_fault_t read_byte(target_addr_t addr, byte_t *pval)  = 0;
	virtual memory_fault_t write_byte(target_addr_t addr, byte_t value) = 0;
	
	virtual memory_fault_t read_hword(target_addr_t addr, hword_t *pval)  = 0;
	virtual	memory_fault_t write_hword(target_addr_t addr, hword_t value) = 0;

	virtual memory_fault_t read_word(target_addr_t addr, word_t *pval)  = 0;
	virtual memory_fault_t write_word(target_addr_t addr, word_t value) = 0;

	virtual	memory_fault_t read_dword(target_addr_t addr, dword_t *pval)  = 0;
	virtual	memory_fault_t write_dword(target_addr_t addr, dword_t value) = 0;
	
};


class memory
{
	public:
		/* constructor, 
		   def_perm - sets the default permission of all pages.
		   sig      - raise signal on fault, -1 means do nothing
		 */
		memory(unsigned int def_perm = MEMORY_PAGE_PERM_MASK, int sig = -1);

		/* destructor */
		~memory();

		/* copy constructor, not implemented for fast mode*/
		memory(const memory&);

		/* free all memory pages, but preserve remapping and permissions */
		void reset();


		/*******************************************************************
			Permission/remapping control for address ranges.
		*******************************************************************/

		/* does nothing if region is unavailable */
		void set_region_permission(target_addr_t addr,
						unsigned int size, unsigned int perm);

		/* remap and unmap a region */
		void remap_region(target_addr_t addr, unsigned int size,
						memory_ext_interface *mif, unsigned int perm);
		void unmap_region(target_addr_t addr, unsigned int size);

		/* mark a region as available or unavailable */
		void mark_region_available(target_addr_t addr,
						unsigned int size, unsigned int perm);
		void mark_region_unavailable(target_addr_t addr, unsigned int size);


		/*******************************************************************
			Primary access interface functions.
		*******************************************************************/

		memory_fault_t read_byte(target_addr_t addr, byte_t *pval)
		{
			return read_base<byte_t>(addr, pval);
		}

		memory_fault_t write_byte(target_addr_t addr, byte_t value)
		{
			return write_base<byte_t>(addr, value);
		}

		memory_fault_t read_word(target_addr_t addr, word_t *pval)
		{
			return read_base<word_t>(addr, pval);
		}

		memory_fault_t write_word(target_addr_t addr, word_t value)
		{
			return write_base<word_t>(addr, value);
		}

		memory_fault_t read_hword(target_addr_t addr, hword_t *pval)
		{
			return read_base<hword_t>(addr, pval);
		}

		memory_fault_t write_hword(target_addr_t addr, hword_t value)
		{
			return write_base<hword_t>(addr, value);
		}

		memory_fault_t read_dword(target_addr_t addr, dword_t *pval)
		{
			return read_base<dword_t>(addr, pval);
		}

		memory_fault_t write_dword(target_addr_t addr, dword_t value)
		{
			return write_base<dword_t>(addr, value);
		}


		/*******************************************************************
			Quick read access interface functions.
			These do not return the error values. Compared to the
			standard functions, these are often faster since gcc can
			pass the read value in register, not in memory.
		*******************************************************************/

		word_t read_word(target_addr_t addr)
		{
			return read_base<word_t>(addr);
		}

		hword_t read_hword(target_addr_t addr)
		{
			return read_base<hword_t>(addr);
		}

		byte_t read_byte(target_addr_t addr)
		{
			return read_base<byte_t>(addr);
		}

		dword_t read_dword(target_addr_t addr)
		{
			return read_base<dword_t>(addr);
		}

		/*******************************************************************
			Block interface functions, used by the loader only.
		*******************************************************************/

		/* Block operations, when a fault occurs, the actual transferred
           bytes is less than size. The transferred count can be obtained
		   by calculating the difference between last_faddr and addr.
		 */
		memory_fault_t read_block(void *buf, target_addr_t addr,
				unsigned int size);
		memory_fault_t write_block(void *buf, target_addr_t addr,
				unsigned int size);
		memory_fault_t set_block(target_addr_t addr, byte_t value,
				unsigned int size);


		/* return the number of pages allocated */
		unsigned int get_page_count() const {return page_count;}

		/* set the fault reporting signal, -1 means do nothing */
		void set_signal(int sig) {fault_sig = sig;}

		/* get the latest fault information */
		memory_fault_t get_last_fault() const {return last_fault;}
		target_addr_t  get_last_faddr() const {return last_faddr;}

	private:

		/* a page table entry */
		typedef struct memory_page_table_entry_t
		{
			unsigned int flag;
			byte_t *ptr;
		} memory_page_table_entry_t;

		// get the permission bits of a page
		unsigned int get_page_permission(target_addr_t addr) const; 
		void set_page_permission(target_addr_t addr, unsigned int perm);

		// mark a page as available or unavailable
		void mark_page_available(target_addr_t addr, unsigned int perm);
		void mark_page_unavailable(target_addr_t addr);

		// remap and unmap a page, page is unavailable after successful unmap
		void remap_page(target_addr_t addr,
						memory_ext_interface *mif, unsigned int perm);
		void unmap_page(target_addr_t addr);


		/* test if an address crosses page boundary, size < MEM_PAGE_SIZE */
		bool cross_page_boundary(target_addr_t addr, unsigned int size) const
		{
			return (addr & (MEMORY_PAGE_SIZE - 1)) > (MEMORY_PAGE_SIZE - size);
		}

		/* some useful utilities */
		target_addr_t align_to_page_boundary(target_addr_t addr) const
		{
			return addr & ~(MEMORY_PAGE_SIZE-1);
		}

		byte_t swap(byte_t val) const
		{
			return val;
		}

		hword_t swap(hword_t val) const
		{
			return (val>>8) | (val<<8);
		}

		word_t swap(word_t val) const
		{
			return (val>>24) | ((val>>8)&0xFF00) |
				   ((val&0xFF00)<<8) | (val<<24) ;
		}

		dword_t swap(dword_t val) const
		{
			return ((dword_t)swap((word_t)val)<<32) |
				    (dword_t)swap((word_t)(val>>32));
		}

		/* storage allocated for a normal page */
		bool page_allocated(const memory_page_table_entry_t *pte) const 
		{
			return ((pte->flag & MEMORY_PAGE_SLOW)==0);
		}

		/* storage allocated for a normal page */
		bool page_allocated(target_addr_t addr) const 
		{
			target_addr_t page_index = addr >> MEMORY_PAGE_SIZE_BITS;
			return ((page_table[page_index].flag & MEMORY_PAGE_SLOW)==0);
		}

		/* page remapped */
		bool page_remapped(const memory_page_table_entry_t *pte) const 
		{
			return (pte->flag & MEMORY_PAGE_REMAPPED);
		}

		/* page remapped */
		bool page_remapped(target_addr_t addr) const 
		{
			target_addr_t page_index = addr >> MEMORY_PAGE_SIZE_BITS;
			return (page_table[page_index].flag & MEMORY_PAGE_REMAPPED);
		}

		const memory_page_table_entry_t *get_page(target_addr_t addr) const
		{
			target_addr_t page_index = addr >> MEMORY_PAGE_SIZE_BITS;
			return page_table + page_index;
		}

		memory_page_table_entry_t *get_page(target_addr_t addr)
		{
			target_addr_t page_index = addr >> MEMORY_PAGE_SIZE_BITS;
			return page_table + page_index;
		}

		template<class T>
		T read_base(target_addr_t addr)
		{
#ifdef CHECK_PAGE_BOUNDARY
			/* if crossing page boundary */
			if (sizeof(T) > 1 && cross_page_boundary(addr, sizeof(T)))
				return read_slow<T>(addr);
#endif

			const memory_page_table_entry_t *pte = get_page(addr);
			/* if empty (not allocated), remapped, bad, or not readable */
			if (!(pte->flag & MEMORY_PAGE_READABLE))
				return read_slow<T>(addr);

#if WORDS_BIGENDIAN==TARGET_LITTLE_ENDIAN
			return swap(*reinterpret_cast<T *>(pte->ptr + addr));
#else
			return *reinterpret_cast<T*>(pte->ptr + addr);
#endif
		}

		template<class T>
		memory_fault_t read_base(target_addr_t addr, T *pval)
		{
#ifdef CHECK_PAGE_BOUNDARY
			/* if crossing page boundary */
			if (sizeof(T) > 1 && cross_page_boundary(addr, sizeof(T)))
				return read_slow(addr, pval);
#endif

			const memory_page_table_entry_t *pte = get_page(addr);
			/* if empty (not allocated), remapped, bad, or not readable */
			if (!(pte->flag & MEMORY_PAGE_READABLE))
				return read_slow(addr, pval);

#if WORDS_BIGENDIAN==TARGET_LITTLE_ENDIAN
			*pval = swap(*reinterpret_cast<T *>(pte->ptr + addr));
#else
			*pval = *reinterpret_cast<T*>(pte->ptr + addr);
#endif
			return MEM_NO_FAULT;
		}

		template<class T>
		memory_fault_t write_base(target_addr_t addr, T value)
		{
#ifdef CHECK_PAGE_BOUNDARY
			/* if crossing page boundary */
			if (cross_page_boundary(addr, sizeof(T)))
				return write_slow(addr, value);
#endif

			const memory_page_table_entry_t *pte = get_page(addr);
			/* if empty (not allocated), remapped, bad, or not writable */
			if (!(pte->flag & MEMORY_PAGE_WRITABLE))
				return write_slow(addr, value);

#if WORDS_BIGENDIAN==TARGET_LITTLE_ENDIAN
			*reinterpret_cast<T *>(pte->ptr + addr) = swap(value);
#else
			*reinterpret_cast<T *>(pte->ptr + addr) = value;
#endif
			return MEM_NO_FAULT;
		}

		/* slow but full featured memory acecss interface
		 * overloading is safer here since these are private
		 */
		memory_fault_t write_slow(target_addr_t addr, dword_t value);
		memory_fault_t write_slow(target_addr_t addr, word_t  value);
		memory_fault_t write_slow(target_addr_t addr, hword_t value);
		memory_fault_t write_slow(target_addr_t addr, byte_t  value);

		memory_fault_t read_slow(target_addr_t addr, dword_t *pval);
		memory_fault_t read_slow(target_addr_t addr, word_t  *pval);
		memory_fault_t read_slow(target_addr_t addr, hword_t *pval);
		memory_fault_t read_slow(target_addr_t addr, byte_t  *pval);

		/* these are the basis of the above _slow functions,
		 * their implementation is hidden in emumem.cpp file
		 */
		template <class T>
		memory_fault_t read_slow_base(target_addr_t addr, T *pval);

		template <class T>
		memory_fault_t write_slow_base(target_addr_t addr, T val);

		template <class T>
		T read_slow(target_addr_t addr)
		{
			T val;
			read_slow(addr, &val);
			return val;
		}

		/* simple block access functions */
		memory_fault_t read_block_within_page(void *buf,
			target_addr_t addr, unsigned int size, unsigned *psize);
		memory_fault_t write_block_within_page(void *buf,
			target_addr_t addr, unsigned int size, unsigned *psize);
		memory_fault_t set_block_within_page(target_addr_t addr,
			byte_t value, unsigned int size, unsigned *psize);

		/* allocate and free space for a page
		 * when memory runs out, allocate_page returns NOMEMORY_FAULT
		 */
		memory_fault_t allocate_page(memory_page_table_entry_t *pte);
		void free_page(memory_page_table_entry_t *pte);

		/* returns non-zero if there is a permission fault */
		memory_fault_t check_page_permission(target_addr_t addr,
				unsigned int perm) const;

		memory_fault_t check_page_permission
				(const memory_page_table_entry_t *pte,
				unsigned int perm) const;

		/* Address translation, NULL if remapped or error.
		 * The function is created for MMU's use.
		 * The return value does not point to the actually storage.
		 * Instead, ptr+addr is the actual address.
		 * See allocate page for details.
		 */
		byte_t *translate(target_addr_t addr)
		{
			memory_fault_t fault;
			memory_page_table_entry_t *pte = get_page(addr);

			/* if normal */
			if (page_allocated(pte)) return pte->ptr;

			/* normal page without space */
			if (pte->flag & MEMORY_PAGE_EMPTY) 
			{
				if ((fault = allocate_page(pte)) != MEM_NO_FAULT)
					report_fault(fault, addr);

				return pte->ptr; //ptr is NULL when allocate fails
			}

			/* IO remap, bad address */
			return NULL;
		}

		/* report a fault, raise a signal it fault_sig is not -1 */
		void report_fault(memory_fault_t f, target_addr_t addr);

		/* page table */
		memory_page_table_entry_t page_table[MEMORY_PAGE_TABLE_SIZE];

		/* stats information */
		unsigned int page_count;	

		/* signal on fault */
		int fault_sig;

		/* stores the information of the last fault*/
		memory_fault_t last_fault;
		target_addr_t last_faddr;

		friend class arm_mmu;

};

} /* namespace simit */

#endif
