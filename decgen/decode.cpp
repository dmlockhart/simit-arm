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

#include <iostream>
#include <iomanip>
#include "decode.h"

using namespace std;

ostream& operator << (ostream& os, const Label& val)
{
	os << val.name;
	return os;
}

ostream& operator << (ostream& os, const DecodeEntry& entry)
{
	os << "mask=" << PADDEDHEX << entry.mask
		<< "\tsig=" << PADDEDHEX << entry.signature
		<< "\tprob=" << setprecision(9) << entry.prob
		<< "\tname=" << entry.label;
	return os;
}
