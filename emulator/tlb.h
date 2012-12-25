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

#ifndef SIMIT_TLB_H
#define SIMIT_TLB_H

//#define RECORD_TLB_MISS			

#include <misc.h>

namespace simit {

typedef enum tlb_mapping_t
{
	TLB_INVALID    = 0,
	TLB_SMALLPAGE  = 1,
	TLB_LARGEPAGE  = 2,
	TLB_SECTION    = 3,
	TLB_ESMALLPAGE = 4,
	TLB_TINYPAGE   = 5
} tlb_mapping_t;


/* Permissions bits in a TLB entry:
 *
 *         31         12 11 10  9  8  7  6  5  4  3   2   1   0
 *          +-------------+-----+-----+-----+-----+---+---+-------+
 * Page:    |             | ap3 | ap2 | ap1 | ap0 | C | B |       |
 *          +-------------+-----+-----+-----+-----+---+---+-------+
 *
 *         31         12 11 10  9              4  3   2   1   0
 *          +-------------+-----+-----------------+---+---+-------+
 * Section: |             |  AP |                 | C | B |       |
 *          +-------------+-----+-----------------+---+---+-------+
 */

/*
section:
	section base address	[31:20]
	AP			- table 8-2, page 8-8
	domain
	C,B

page:
	page base address	[31:16] or [31:12]
	ap[3:0]
	domain (from L1)
	C,B
*/
//#define DEBUG_TLB



/* FS[3:0] in the fault status register: */
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

class tlb_entry_t
{
	public:
	word_t virt_addr;
	word_t phys_addr;
	word_t perms;
	word_t domain;
	word_t mask;
	word_t nmask;
	tlb_mapping_t mapping;

	mmu_fault_t read_fault;
	mmu_fault_t write_fault;
};


template <unsigned num> class tlb {

	public:
		/* constructor, num is number of entries */
		tlb () {
			reset();
		}

		/* destructor */
		~tlb() {
#ifdef RECORD_TLB_MISS			
			printf("tlb miss = %u \n",miss_count);
#endif
		}

		/* reset */
		void reset() {
			miss_count = 0;
			invalidate_all();
		}

		void invalidate_all () {
			
			for (unsigned i= 0; i < num; i++) {
				entries[i].mapping = TLB_INVALID;
				entries[i].mask = 0xFFFFFFFF;
				entries[i].virt_addr = 0xFFFFFFFF;
			}
			last_e= &entries[0];
			
			cycle = 0;
		}

		void invalidate_entry (word_t addr) {
			tlb_entry_t *e;
			e = search (addr);
			if (e) {
				e->mapping = TLB_INVALID;
				e->mask = 0xFFFFFFFF;
				e->virt_addr = 0xFFFFFFFF;
			}
		}

		/* search for an entry */
		tlb_entry_t *search (word_t virt_addr) {
			for (unsigned i = 0; i <num; i++) {
				if (entries[i].mapping != TLB_INVALID &&
					((virt_addr & entries[i].mask) == 
					(entries[i].virt_addr & entries[i].mask))) {
					return &entries[i];
				}
			}
#ifdef RECORD_TLB_MISS			
			miss_count++;
#endif
			return NULL;
		}
		

		/* update RB */
		tlb_entry_t *next () {
			last_e = entries + cycle;
			cycle = (cycle+1) % num;
			
			return last_e;
		}

		tlb_entry_t *get_entry(int index){
			return entries + index;
		}
	
	private:
	
		tlb_entry_t entries[num]; 
		tlb_entry_t *last_e;

		int cycle;	/* current tlb cycle, state for round robin policy */
		uint64_t miss_count;	/* miss count */
};


#define tlb_c_flag(tlb) \
	((tlb)->perms & 0x8)

#define tlb_b_flag(tlb) \
	((tlb)->perms & 0x4)

}


#endif	    /*SIMIT_TLB_H*/
