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

#ifndef _INST_TYPE_H_
#define _INST_TYPE_H_

#include <iostream>
#include <cstdlib>
#include "config.h"

/* width of the instruction word in bytes */
#ifndef INSTSIZE
#define INSTSIZE 4
#endif

#define INSTBITS (INSTSIZE*8)
#define WORDSIZE (INSTSIZE*2)

#if INSTSIZE<=SIZEOF_LONG_LONG

#if INSTSIZE>SIZEOF_LONG

/* instruction word data type */
typedef class inst_word_t {

  public:
	inst_word_t() : val(0u) {}
	inst_word_t(unsigned int c) : val(c) {}
	inst_word_t(unsigned long c) : val(c) {}
	inst_word_t(unsigned long long c) : val(c) {}
	inst_word_t(const char *str) {val = strtoull(str, NULL, 0);}

	inst_word_t& operator &= (const inst_word_t& r) {
		this->val &= r.val;
		return *this;
	}

	inst_word_t& operator |= (const inst_word_t& r) {
		this->val |= r.val;
		return *this;
	}

	inst_word_t& operator ^= (const inst_word_t& r) {
		this->val ^= r.val;
		return *this;
	}

	inst_word_t& operator <<= (const unsigned int sbits) {
		this->val <<= sbits;
		return *this;
	}

	unsigned int pop_count() {
		return pcount((unsigned int)val) + pcount((unsigned int)(val>>32));
	}

	unsigned int ucast() const {return (unsigned int)val;}

	bool bit_n(unsigned n) {return (val>>n)&1;}

  private:

	unsigned long long val;

	unsigned int pcount(unsigned int x) {
		x = (x & 0x55555555) + ((x >> 1) & 0x55555555);
		x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
		x = (x & 0x0F0F0F0F) + ((x >> 4) & 0x0F0F0F0F);
		x = (x & 0x00FF00FF) + ((x >> 8) & 0x00FF00FF);
		x = (x & 0x0000FFFF) + ((x >> 16) & 0x0000FFFF);
		return x;
	}

	friend inst_word_t operator & (const inst_word_t& x, const inst_word_t& y);
	friend inst_word_t operator | (const inst_word_t& x, const inst_word_t& y);
	friend inst_word_t operator ^ (const inst_word_t& x, const inst_word_t& y);
	friend inst_word_t operator << (const inst_word_t& x, unsigned int s);
	friend inst_word_t operator >> (const inst_word_t& x, unsigned int s);
	friend bool operator == (const inst_word_t& x, const inst_word_t& y);
	friend bool operator != (const inst_word_t& x, const inst_word_t& y);
	friend inst_word_t operator ~ (const inst_word_t& x);
	friend std::ostream& operator << (std::ostream& os, const inst_word_t& w);

} InstWord;

inline InstWord power2(unsigned int n) {
	return InstWord(1ull<<n);
}

inline InstWord n_ones(unsigned int n) {
	/* for n>=INSTBITS, the shift operator will take it as n mod 32*/
	if (n>=INSTBITS) return InstWord(-1ull);
	else return InstWord((1ull<<n)-1);
}

#else

/* instruction word data type */
typedef class inst_word_t {

  public:
	inst_word_t() : val(0u) {}
	inst_word_t(unsigned int c) : val(c) {}
	inst_word_t(unsigned long c) : val(c) {}
	inst_word_t(const char *str) {val = std::strtoul(str, NULL, 0);}

	inst_word_t& operator &= (const inst_word_t& r) {
		this->val &= r.val;
		return *this;
	}

	inst_word_t& operator |= (const inst_word_t& r) {
		this->val |= r.val;
		return *this;
	}

	inst_word_t& operator ^= (const inst_word_t& r) {
		this->val ^= r.val;
		return *this;
	}

	inst_word_t& operator <<= (const unsigned int sbits) {
		this->val <<= sbits;
		return *this;
	}

	unsigned int pop_count() {
		unsigned int x;
		x = (val & 0x55555555) + ((val >> 1) & 0x55555555);
		x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
		x = (x & 0x0F0F0F0F) + ((x >> 4) & 0x0F0F0F0F);
		x = (x & 0x00FF00FF) + ((x >> 8) & 0x00FF00FF);
		x = (x & 0x0000FFFF) + ((x >> 16) & 0x0000FFFF);
		return x;
	}

	unsigned int ucast() const {return (unsigned int)val;}

	bool bit_n(unsigned n) {return (val>>n)&1;}

  private:

	unsigned long val;

	friend inst_word_t operator & (const inst_word_t& x, const inst_word_t& y);
	friend inst_word_t operator | (const inst_word_t& x, const inst_word_t& y);
	friend inst_word_t operator ^ (const inst_word_t& x, const inst_word_t& y);
	friend inst_word_t operator << (const inst_word_t& x, unsigned int s);
	friend inst_word_t operator >> (const inst_word_t& x, unsigned int s);
	friend bool operator == (const inst_word_t& x, const inst_word_t& y);
	friend bool operator != (const inst_word_t& x, const inst_word_t& y);
	friend inst_word_t operator ~ (const inst_word_t& x);
	friend std::ostream& operator << (std::ostream& os, const inst_word_t& w);

} InstWord;

inline InstWord power2(unsigned int n) {
	return InstWord(1u<<n);
}

inline InstWord n_ones(unsigned int n) {
	/* for n>=INSTBITS, the shift operator will take it as n mod 32*/
	if (n>=INSTBITS) return InstWord(-1u);
	else return InstWord((1u<<n)-1);
}

#endif

inline InstWord operator & (const InstWord& x, const InstWord& y) {
	return InstWord(x.val & y.val);
}

inline InstWord operator | (const InstWord& x, const InstWord& y) {
	return InstWord(x.val | y.val);
}

inline InstWord operator ^ (const InstWord& x, const InstWord& y) {
	return InstWord(x.val ^ y.val);
}

inline InstWord operator << (const InstWord& x, unsigned int sbits) {
	return InstWord(x.val << sbits);	
}

inline InstWord operator >> (const InstWord &x, unsigned int sbits) {
	return InstWord(x.val >> sbits);	
}

inline bool operator == (const InstWord& x, const InstWord& y) {
	return x.val==y.val;
}

inline bool operator != (const InstWord& x, const InstWord& y) {
	return x.val!=y.val;
}

inline InstWord operator ~ (const InstWord& x) {
	return InstWord(~x.val);
}

inline std::ostream& operator << (std::ostream& os, const InstWord& word)
{
	os << word.val;
	return os;
}

#else 

#include <bitset>

/* instruction word data type */
typedef struct word_t {

	word_t(unsigned int c);
	word_t(unsigned long c);
	word_t(const char *str);

	std::bitset<INSTBITS> val;
} InstWord;

#error "Sorry the case when instruction word is wider than host long integer size is not supported yet."

#endif

#endif
