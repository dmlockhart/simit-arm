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

#include "bin_pattern.hpp"
#include <cstring>

using std::ostream;
using std::string;
using std::list;

#define BITS_PER_UNSIGNED  (sizeof(unsigned)*8)
#define BYTES_PER_UNSIGNED (sizeof(unsigned))

static unsigned pop_count(unsigned val)
{
	static unsigned char pop_count_table[] = {
		0, 1, 1, 2, 1, 2, 2, 3,
		1, 2, 2, 3, 2, 3, 3, 4,
		1, 2, 2, 3, 2, 3, 3, 4,
		2, 3, 3, 4, 3, 4, 4, 5,
		1, 2, 2, 3, 2, 3, 3, 4,
		2, 3, 3, 4, 3, 4, 4, 5,
		2, 3, 3, 4, 3, 4, 4, 5,
		3, 4, 4, 5, 4, 5, 5, 6,
		1, 2, 2, 3, 2, 3, 3, 4,
		2, 3, 3, 4, 3, 4, 4, 5,
		2, 3, 3, 4, 3, 4, 4, 5,
		3, 4, 4, 5, 4, 5, 5, 6,
		2, 3, 3, 4, 3, 4, 4, 5,
		3, 4, 4, 5, 4, 5, 5, 6,
		3, 4, 4, 5, 4, 5, 5, 6,
		4, 5, 5, 6, 5, 6, 6, 7,
		1, 2, 2, 3, 2, 3, 3, 4,
		2, 3, 3, 4, 3, 4, 4, 5,
		2, 3, 3, 4, 3, 4, 4, 5,
		3, 4, 4, 5, 4, 5, 5, 6,
		2, 3, 3, 4, 3, 4, 4, 5,
		3, 4, 4, 5, 4, 5, 5, 6,
		3, 4, 4, 5, 4, 5, 5, 6,
		4, 5, 5, 6, 5, 6, 6, 7,
		2, 3, 3, 4, 3, 4, 4, 5,
		3, 4, 4, 5, 4, 5, 5, 6,
		3, 4, 4, 5, 4, 5, 5, 6,
		4, 5, 5, 6, 5, 6, 6, 7,
		3, 4, 4, 5, 4, 5, 5, 6,
		4, 5, 5, 6, 5, 6, 6, 7,
		4, 5, 5, 6, 5, 6, 6, 7,
		5, 6, 6, 7, 6, 7, 7, 8,
	};

	unsigned count = 0;
	for (unsigned ii=0; ii<BYTES_PER_UNSIGNED; ii++) {
		count += pop_count_table[(unsigned char)val];
		val >>= 8;
	}
	return count;
}

binary_pattern::binary_pattern(const string& str)
{
	len = str.length();

	/* allocate space for the data */
	unsigned wlen = (len+BITS_PER_UNSIGNED-1)/BITS_PER_UNSIGNED;
	data = new unsigned[wlen*2];

	/* reset the data to zero */
	memset(data, 0, wlen*2*BYTES_PER_UNSIGNED);

	const char *cstr = str.c_str();
	for (unsigned ii=0; ii<len; ii++) {

		unsigned wpos = ii/BITS_PER_UNSIGNED;
		unsigned bpos = ii%BITS_PER_UNSIGNED;

		if (cstr[len-ii-1]=='1') {
			data[wpos] |= 1<<bpos;
			data[wlen+wpos]  |= 1<<bpos;
		}
		else if (cstr[len-ii-1]=='0') {
			data[wpos] |= 1<<bpos;
		}
	}
}

binary_pattern::binary_pattern(unsigned length) : len(length)
{
	/* allocate space for the data */
	unsigned wlen = (len+BITS_PER_UNSIGNED-1)/BITS_PER_UNSIGNED;
	data = new unsigned[wlen*2];

	/* reset the data to zero */
	memset(data, 0, wlen*2*BYTES_PER_UNSIGNED);
}

binary_pattern::binary_pattern(const binary_pattern& pat)
{
	len = pat.len;

	/* allocate space for the data */
	unsigned wlen = (len+BITS_PER_UNSIGNED-1)/BITS_PER_UNSIGNED;
	data = new unsigned[wlen*2];

	/* copy the data */
	memcpy(data, pat.data, wlen*2*BYTES_PER_UNSIGNED);
}

binary_pattern::~binary_pattern()
{
	delete [] data;
}

binary_pattern& binary_pattern::operator = (const binary_pattern& pat)
{
	if (len==pat.len) {
		unsigned wlen = (len+BITS_PER_UNSIGNED-1)/BITS_PER_UNSIGNED;
		memcpy(data, pat.data, wlen*2*BYTES_PER_UNSIGNED);
	}
	else {

		delete [] data;

		len = pat.len;

		/* allocate space for the data */
		unsigned wlen = (len+BITS_PER_UNSIGNED-1)/BITS_PER_UNSIGNED;
		data = new unsigned[wlen*2];

		/* copy the data */
		memcpy(data, pat.data, wlen*2*BYTES_PER_UNSIGNED);
	}
	return *this;
}

bool binary_pattern::overlap(const binary_pattern &pat) const
{
	assert(len==pat.len);

	/* allocate space for the data */
	unsigned wlen = (len+BITS_PER_UNSIGNED-1)/BITS_PER_UNSIGNED;

	/* check all words */
	for (unsigned ii=0; ii<wlen; ii++) {

		/* consider bits that are significant for both */
		unsigned mask = data[ii] & pat.data[ii];
		unsigned sig  = data[wlen+ii] ^ pat.data[wlen+ii];

		/* see if there is conflict */
		if (mask & sig) return false;
	}

	return true;
}

bool binary_pattern::contain(const binary_pattern& pat) const
{
	assert(len==pat.len);

	/* allocate space for the data */
	unsigned wlen = (len+BITS_PER_UNSIGNED-1)/BITS_PER_UNSIGNED;

	/* check all words */
	for (unsigned ii=0; ii<wlen; ii++) {

		/* see if mask1 is smaller all mask2 */
		if ((data[ii] & pat.data[ii]) != data[ii]) return false;

		/* if there is difference in value, see if significant */
		unsigned sig  = data[wlen+ii] ^ pat.data[wlen+ii];
		if (data[ii] & sig) return false;
	}

	return true;
}

unsigned binary_pattern::distance(const binary_pattern& pat) const
{
	assert(len==pat.len);

	/* allocate space for the data */
	unsigned wlen = (len+BITS_PER_UNSIGNED-1)/BITS_PER_UNSIGNED;
	unsigned dist = 0;

	/* check all words */
	for (unsigned ii=0; ii<wlen; ii++) {

		/* consider bits that are significant for both */
		unsigned mask = data[ii] & pat.data[ii];
		unsigned sig  = data[wlen+ii] ^ pat.data[wlen+ii];

		/* see if there is conflict */
		dist += pop_count(mask & sig);
	}

	return dist;
}

bool binary_pattern::mergeable(const binary_pattern& pat) const
{
	assert(len==pat.len);

	/* allocate space for the data */
	unsigned wlen = (len+BITS_PER_UNSIGNED-1)/BITS_PER_UNSIGNED;
	unsigned dist = 0;
	unsigned overflow1 = 0, overflow2 = 0;

	/* check all words */
	for (unsigned ii=0; ii<wlen; ii++) {

		/* consider bits that are significant for both */
		unsigned mask = data[ii] & pat.data[ii];
		unsigned sig  = data[wlen+ii] ^ pat.data[wlen+ii];

		overflow1 |= data[ii] & ~mask;
		overflow2 |= pat.data[ii] & ~mask;

		/* see if there is conflict */
		dist += pop_count(mask & sig);
	}
	if (overflow1 && overflow2) return false;
	return dist==1;
}

#if 0
list<BinPattern> binary_pattern::diff(const binary_pattern& x) const
{
	assert(this->contain(x));

	list<BinPattern> ret;
	const char *str1 = this->data.c_str();
	const char *str2 = x.data.c_str();

	for (unsigned int i = 0; i<this->get_length(); i++) {

		if (str1[i]=='-' && str2[i]=='0') {
			string s(str1);
			s[i] = '1';
			ret.push_back(s);
		}
		else if (str1[i]=='-' && str2[i]=='1') {
			string s(str1);
			s[i] = '0';
			ret.push_back(s);
		}
	}
	return ret;
}

string binary_pattern::get_mask() const
{
	string ret = data;
	for (unsigned int i=0; i<data.length(); i++) {
		if (ret[i]=='-') ret[i] = '0';
		else ret[i] = '1';
	}

	return ret;
}

string binary_pattern::get_signature() const
{
	string ret = data;
	for (unsigned int i=0; i<data.length(); i++) {
		if (ret[i]=='-') ret[i] = '0';
	}
	return ret;
}
#endif

#define hex_char(_v) ((_v<10)?(_v+'0'):(_v+'A'-10))

string binary_pattern::get_str() const
{
	string ret(len, '-');

	unsigned wlen  = (len+BITS_PER_UNSIGNED-1)/BITS_PER_UNSIGNED;
	for (unsigned ii=0; ii<len; ii++) {

		unsigned wpos = ii/BITS_PER_UNSIGNED;
		unsigned bpos = ii%BITS_PER_UNSIGNED;

		if (data[wpos] & (1<<bpos)) {

			if (data[wpos+wlen] & (1<<bpos))
				ret[len-ii-1] = '1';
			else
				ret[len-ii-1] = '0';
		}
	}

	return ret;
}

string binary_pattern::get_hex_mask() const
{
	/* length of the pattern in hexidecimal */
	unsigned hlen  = (len+3)/4;
	string ret(hlen, '0');

	for (unsigned ii=0; ii<len; ii+=4, hlen--) {

		unsigned wpos = ii/BITS_PER_UNSIGNED;
		unsigned bpos = ii%BITS_PER_UNSIGNED;
		unsigned val = (data[wpos]>>bpos) & 15;

		ret[hlen-1] = hex_char(val);
	}

	return ret;
}

string binary_pattern::get_hex_signature() const
{
	/* length of the pattern in hexidecimal */
	unsigned hlen  = (len+3)/4;
	string ret(hlen, '0');

	unsigned wlen  = (len+BITS_PER_UNSIGNED-1)/BITS_PER_UNSIGNED;
	for (unsigned ii=0; ii<len; ii+=4, hlen--) {

		unsigned wpos = ii/BITS_PER_UNSIGNED;
		unsigned bpos = ii%BITS_PER_UNSIGNED;
		unsigned val = (data[wpos+wlen]>>bpos) & 15;

		ret[hlen-1] = hex_char(val);
	}

	return ret;
}

ostream& binary_pattern::print(ostream& os) const
{
	return os << this->get_str();
}

const BinPattern operator + (const BinPattern& x, const BinPattern& y)
{
	/* get the total length */
	unsigned len = x.len + y.len;

	BinPattern result(len);

	/* copy the data from the low order bits*/
	unsigned wlen  = (len+BITS_PER_UNSIGNED-1)/BITS_PER_UNSIGNED;
	unsigned ywlen = (y.len+BITS_PER_UNSIGNED-1)/BITS_PER_UNSIGNED;
	unsigned xwlen = (x.len+BITS_PER_UNSIGNED-1)/BITS_PER_UNSIGNED;

	/* copy over the LSBs */
	memcpy(result.data, y.data, ywlen*BYTES_PER_UNSIGNED);
	memcpy(result.data+wlen, y.data+ywlen, ywlen*BYTES_PER_UNSIGNED);

	if (y.len%BITS_PER_UNSIGNED==0) {

		memcpy(result.data+ywlen, x.data, xwlen*BYTES_PER_UNSIGNED);
		memcpy(result.data+ywlen+wlen, x.data+xwlen, xwlen*BYTES_PER_UNSIGNED);
	}
	else {
		/* the position for the lsb from x*/
		unsigned bpos = y.len%BITS_PER_UNSIGNED;
		for (unsigned ii=0; ii<xwlen; ii++) {

			result.data[ywlen-1+ii] |= x.data[ii] << bpos;
			result.data[wlen+ywlen-1+ii] |= x.data[ii+xwlen] << bpos;

			if (ywlen+ii<wlen) {
				result.data[ywlen+ii] |=
					x.data[ii] >> (BITS_PER_UNSIGNED-bpos);
				result.data[wlen+ywlen+ii] |=
					x.data[ii+xwlen] >> (BITS_PER_UNSIGNED-bpos);
			}
		}
	}

	return result;
}

const BinPattern operator ^ (const BinPattern& x, const BinPattern& y)
{
	assert(x.len == y.len);

	BinPattern result(x.len);

	unsigned wlen  = (x.len+BITS_PER_UNSIGNED-1)/BITS_PER_UNSIGNED;
	for (unsigned ii=0; ii<wlen; ii++) {

		/* consider bits that are significant for both */
		unsigned mask = x.data[ii] & y.data[ii];
		unsigned sig  = x.data[wlen+ii] ^ y.data[wlen+ii];
		unsigned diff = mask & sig;

		result.data[ii] = (x.data[ii] | y.data[ii]) & (~diff);
		result.data[ii+wlen] = (x.data[wlen+ii] | y.data[wlen+ii]) & (~diff);
	}
	return result;
}

bool operator == (const BinPattern& x, const BinPattern& y)
{
	assert(x.len == y.len);

	unsigned wlen  = (x.len+BITS_PER_UNSIGNED-1)/BITS_PER_UNSIGNED;
	for (unsigned ii=0; ii<wlen; ii++) {
		if (x.data[ii]!=y.data[ii]) return false;
		if (x.data[ii+wlen]!=y.data[ii+wlen]) return false;
	}
	return true;
}

bool operator != (const BinPattern& x, const BinPattern& y)
{
	return !(x==y);
}

ostream& operator << (ostream& os, const BinPattern& x)
{
	return os << x.get_str();
}

static void SCC_minimal(list<binary_pattern>& pats)
{
	list<binary_pattern>::iterator iit, jjt;
	for (iit=pats.begin(); iit!=pats.end();) {

		bool del = false;
		for (jjt=iit, jjt++; jjt!=pats.end();) {

			if ((*iit).contain(*jjt)) {
				jjt = pats.erase(jjt);
			}
			else if ((*jjt).contain(*iit)) {
				del = true;
				iit = pats.erase(iit);
				break;
			}
			else {
				jjt++;
			}
		}
		if (!del) iit++;
	}
}

static void iterated_merge(list<binary_pattern>& pats)
{
	bool change = true;

	while (change) {

		change = false;
		/* remove nonessential ones first */
		SCC_minimal(pats);

		list<binary_pattern> new_pats;

		list<binary_pattern>::iterator iit, jjt;
		for (iit=pats.begin(); iit!=pats.end();) {

			bool del = false;
			for (jjt=iit, jjt++; jjt!=pats.end();) {

				/* see if possible to merge */
				if ((*iit).mergeable(*jjt)) {

					change = true;
					/* merge and push it to the back of the back */
					new_pats.push_back((*iit)^(*jjt));

					/* see if the result contains its sources */
					if (new_pats.back().contain(*jjt)) {
						jjt = pats.erase(jjt);
					}
					else if (new_pats.back().contain(*iit)) {
						del = true;
						iit = pats.erase(iit);
						break;
					}
					else {
						assert(0);
						jjt++;
					}
				}
				else jjt++;
			}
			if (!del) iit++;
		}
		pats.insert(pats.end(), new_pats.begin(), new_pats.end());
	}
}

void exclude_cube(list<binary_pattern>& pats, const binary_pattern& x)
{
	assert(pats.size()>0);
	unsigned len = pats.front().get_length();
	unsigned wlen = (len+BITS_PER_UNSIGNED-1)/BITS_PER_UNSIGNED;

	list<binary_pattern>::iterator iit;
	for (iit=pats.begin(); iit!=pats.end();) {

		/* the simplest case, remove iit */
		if (x.contain(*iit)) {
			iit = pats.erase(iit);
		}
		else if ((*iit).overlap(x)) {

			for (unsigned ii=0; ii<wlen; ii++) {

				unsigned diff = ~(*iit).data[ii] & x.data[ii];
				unsigned inv  = ~x.data[wlen+ii];

				for (unsigned jj=0; diff; jj++, diff>>=1) {

					if (diff&1) {
						BinPattern new_pat = *iit;
						new_pat.data[ii] |= 1<<jj;
						new_pat.data[ii+wlen] |= inv & (1<<jj);
						pats.push_back(new_pat);
					}
				}
			}
			iit = pats.erase(iit);
		}
		else iit++;
	}
}

/* get the inverse of the cubes */
void inverse(list<binary_pattern>& pats, list<binary_pattern>& result)
{
	/* simplify the space first */
	iterated_merge(pats);

	assert(pats.size()>0);
	unsigned len = pats.front().len;

	/* the full space */
	result.push_back(binary_pattern(len));

	unsigned ii=0;
	list<binary_pattern>::iterator iit;
	for (iit=pats.begin(); iit!=pats.end(); iit++, ii++) {
		exclude_cube(result, *iit);
		if (ii==5) {
			SCC_minimal(result);
			ii=0;
		}
	}

	/* simplify the final result */
	iterated_merge(result);
}
