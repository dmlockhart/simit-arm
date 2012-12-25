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

#ifndef _DECODE_H_
#define _DECODE_H_

#include <list>
#include <vector>
#include <string>
#include <functional>
#include <iostream>

#include "inst_type.h"

/* label data type */
typedef struct dg_label_t {

	/* constructor */
	dg_label_t(const std::string& n) : name(n) {}

	/* member */
	std::string name;

	friend std::ostream& operator << (std::ostream&, const dg_label_t&);

} Label;

inline bool operator== (const Label& lhs, const Label& rhs) {
	return lhs.name == rhs.name;
}

/* decoding entry data type */
typedef struct entry_t {

	/* constructor */
	entry_t();
	entry_t(const char *m, const char *s, const char *l, double p) :
		mask(m), signature(s),label(l),prob(p) {}

	/* members */
	InstWord mask;
	InstWord signature;
	Label label;	
	double prob;

	friend std::ostream& operator << (std::ostream&, const entry_t&);

} DecodeEntry;


inline bool operator< (const DecodeEntry& lhs, const DecodeEntry &rhs) {
	return lhs.label.name < rhs.label.name;
}

#define PADDEDHEX "0x" << hex << setw(WORDSIZE) << setfill('0')
#define MINUTILRATE 0.1

#endif
