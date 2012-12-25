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

#include <math.h>
#include <stdlib.h>
#include <functional>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cassert>
#include "decode_table.h"

using namespace std;

DecodeTable::decode_table(vector<DecodeEntry>& es, int id) :
	entries(es), entropy(0.0), mask(0u), prob(0.0), 
	index(id), usize(0)
{
	cmask = n_ones(INSTBITS);

	/* sort entries according to names so that we can count usize easily*/
	sort(this->entries.begin(), this->entries.end());

	/* calculate sum proprobabilities, masks and signatures */
	vector<DecodeEntry>::iterator it;
	for (it=this->entries.begin(); it!=this->entries.end(); it++) {
		this->prob += (*it).prob;
		this->mask |= (*it).mask;
		this->cmask &= (*it).mask;
		this->signature = (*it).signature;
	}

	dmask = cmask;
	/* normalize probability and fix dmask */
	for (it=this->entries.begin(); it!=this->entries.end(); it++)  {
		this->dmask ^= this->dmask&(this->signature^(*it).signature);
		(*it).prob /= this->prob;
	}

	/* constant bits should be don't cares */
	this->mask ^= this->dmask;
	this->cmask ^= this->dmask;
	this->signature &= this->mask | this->dmask;

	/* calculate the probability vector for entropy purpose */
	vector<double> vprob;

	/* calculate the number of unique labels and each label's probability */
	it = this->entries.begin();
	while (it!=this->entries.end()) {

		double prob_accu = (*it).prob;
		vector<DecodeEntry>::iterator nit=it+1;
		while (nit!=this->entries.end() && (*nit).label==(*it).label) {
			prob_accu += (*nit).prob;
			nit++;
		}

		usize++;
		if (prob_accu>0.0) vprob.push_back(prob_accu);
		it = nit;
	}

	/* calculate entropy */
	vector<double>::iterator dit;
	for (dit=vprob.begin(); dit!=vprob.end(); dit++)
		this->entropy -= (*dit)*log(*dit);

	this->entropy /= log(2.0);

	/* calculate Huffman tree */
	HuffmanTree htree(vprob);
	this->hTreeHeight = htree.getHeight();
}

bool DecodeTable::fixEntries()
{
	/* first sort the entries, bigger ones first */
	vector<DecodeEntry>::iterator it;
	for (it=entries.begin(); it!=entries.end(); it++) {

		vector<DecodeEntry>::iterator it1;
		for (it1=it+1; it1!=entries.end();) {

			/* check if two entries overlap */
			InstWord mask = (*it).mask & (*it1).mask;
			InstWord diff = (*it).signature ^ (*it1).signature;

			if ((mask & diff) == 0u) {
				if ((*it).label==(*it1).label) {

					/* in this case, move the bigger one ahead and
					 *  delete the small one */
					
					/* it's mask is smaller => it is bigger */
					if (mask==(*it).mask) {
						cerr << "Remove redundant pattern, between " << endl;
						cerr << (*it1) << endl << (*it) << endl;
						it1 = entries.erase(it1);
					}
					else if (mask==(*it1).mask) { /* swap the order */
						cerr << "Remove redundant pattern, between " << endl;
						cerr << (*it) << endl << (*it1) << endl;
						(*it) = (*it1);
						it1 = entries.erase(it1);
					}
					/* Otherwise two just overlap , does not contain. */
					it1++;
				}
				else {

					if ((*it).mask==(*it1).mask) {
						cerr << "Patterns completely overlap, between " << endl;
						cerr << (*it) << endl << (*it1) << endl;
						return false;
					}
					/* it1's mask is smaller => it1 is bigger, swap */
					else if (mask==(*it1).mask) {
						DecodeEntry temp = *it;
						(*it) = (*it1);
						(*it1) = temp;
					}
					/* neither one is bigger than the other */
					else if (mask!=(*it).mask) {
						cerr << "Patterns overlap, not comparable, between "
							  << endl;
						cerr << (*it) << endl << (*it1) << endl;
						return false;
					}
					/* otherwise do nothing */

					it1++;
				}
			}
			else it1++;
		}
	}

	vector<DecodeEntry> new_entries;

	for (it=entries.begin(); it!=entries.end(); it++) {


		bool split_flag = false;
		vector<DecodeEntry>::iterator it1;
		for (it1=it+1; it1!=entries.end(); it1++) {

			/* check if two entries overlap */
			InstWord mask = (*it).mask & (*it1).mask;
			InstWord diff = (*it).signature ^ (*it1).signature;

			if ((mask & diff) == 0u) {
				   
				/* redundancy must have been cleared */
				if ((*it).label==(*it1).label) continue;

			   	/* must be bigger one first after sorting*/
				assert((*it).mask!=(*it1).mask);
				assert(mask==(*it).mask);

				//cerr << "split " <<endl << (*it) << endl << (*it1) << endl;

				InstWord odds = (mask ^ (*it1).mask);
				DecodeEntry tmp = *it;
				tmp.prob = (*it).prob/odds.pop_count();

				for (InstWord i=1u; i!=0u; i<<=1u) {

					if ((odds & i)!=0u) {
						tmp.mask |= i;

						if ((i&((*it1).signature^tmp.signature))==0u)
							tmp.signature ^= i;

						new_entries.push_back(tmp);					
						tmp.signature ^= i;				   
						tmp.mask ^= i;
					}
				}
				split_flag = true;
				break;
			}
		}
		if (!split_flag) new_entries.push_back(*it);
	}
	entries = new_entries;

	return true;
}

bool DecodeTable::checkWellFormedness() 
{
	bool ret = true;
	vector<DecodeEntry>::iterator it;
	for (it=this->entries.begin(); it!=this->entries.end(); it++) {
		vector<DecodeEntry>::iterator it1;
		for (it1=it+1; it1!=this->entries.end(); it1++) {
			/* check if two entries overlap */
			InstWord mask = (*it).mask & (*it1).mask;
			InstWord diff = (*it).signature ^ (*it1).signature;
			if ((mask & diff) == 0u && !((*it).label==(*it1).label)) {
				cerr << "Patterns overlap, between " << endl;
				cerr << (*it) << endl << (*it1) << endl;
				ret = false;
			}
		}
	}
	return ret;
}

vector<DecodeTable *> DecodeTable::divide_by_pattern(InstWord msk,
	InstWord signature)
{
	vector<DecodeTable *> ret;

	/* see if already decode bit or insignificant bit */
	assert((msk&this->dmask)==0u);
	assert((msk&this->mask)==msk);

	vector<DecodeEntry> v0, v1;
	vector<DecodeEntry>::iterator it;
	for (it=this->entries.begin(); it!=this->entries.end(); it++) {

		if (((*it).mask&msk)==msk) { /*completely overlap*/
			if ((msk&signature)==(msk&(*it).signature)) /*good -> v0*/
				v0.push_back(*it);
			else /* bad -> v1 */
				v1.push_back(*it);
		}
		else {	/*partially overlap*/

			DecodeEntry tmp = *it;
			if ((msk&tmp.mask&(signature^tmp.signature))==0u) { /*split*/

				InstWord odds = (msk ^ tmp.mask) & msk;
				assert(odds!=0u);	/* odds==0u means completely overlap */

				for (InstWord i=1u; i!=0u; i<<=1u) {
					if ((odds & i)!=0u) {
						tmp.mask |= i;
						if ((i&(signature^tmp.signature))==0u)
							tmp.signature ^= i;
						tmp.prob /= 2.0;
						v1.push_back(tmp);
						tmp.signature ^= i;
					}
				}
				v0.push_back(tmp);
			}
			else {
				v1.push_back(tmp);
			}
			
		}
	}

	DecodeTable *tmp1=new DecodeTable(v0, 0);
	DecodeTable *tmp2=new DecodeTable(v1, 1);
	ret.push_back(tmp1);
	ret.push_back(tmp2);
	return ret;
}

/* split the table into 2^bitcount small ones according to bits
 * from rmb to rmb+bitcount-1, here msk=n_ones(bitcount)<<rmb
 */
vector<DecodeTable *> DecodeTable::divide_by_table(InstWord msk,
	int bitcount, int rmb)
{
	vector<DecodeTable *> ret;

	/* see if any already decoded bit */
	assert((msk&this->dmask)==0u);

	vector<vector<DecodeEntry> > v(1<<bitcount);
	vector<DecodeEntry>::iterator it;
	for (it=this->entries.begin(); it!=this->entries.end(); it++) {

		DecodeEntry tmp = *it;
		vector<InstWord> patterns = split_pattern(bitcount, tmp.mask>>rmb,
			tmp.signature>>rmb);

		tmp.mask |= msk;
		tmp.prob /= patterns.size();

		vector<InstWord>::iterator pit;
		for (pit=patterns.begin(); pit!=patterns.end(); pit++) {
			tmp.signature = (tmp.signature & (~msk)) | ((*pit)<<rmb);
			v[pit->ucast()].push_back(tmp);
		}
	}

	for (int j=0; j<(1<<bitcount); j++) {
		DecodeTable *tmp=new DecodeTable(v[j], j);
		ret.push_back(tmp);
	}

	return ret;
}

unsigned getval(InstWord inst, vector<unsigned int>& bitones)
{
	unsigned int ret = 0;
	vector<unsigned int>::reverse_iterator it;
	for (it = bitones.rbegin(); it!=bitones.rend(); it++)
			if ((inst & (1u<<(*it)))==0u) ret = ret<<1;
			else ret = (ret<<1)+1;
//	cout << "getval returns" << ret << " " << hex << inst << " " << bitones[0] << endl;
	return ret;

}

vector<DecodeTable *> DecodeTable::divide_by_theiling_table(InstWord msk)
{
	vector<DecodeTable *> ret;

	/* see if any already decoded bit */
	assert((msk&this->dmask)==0u);

	/* first we count the bit of 1s in msk, those are the ones to split */
	unsigned int i;
	vector<unsigned int> bitones;
	for (i=0; i<INSTBITS; i++) {
		if ((power2(i)&msk)!=0u)
			bitones.push_back(i);
	}

//	cout << "number of 1 bits " << bitones.size() << endl;
	vector<vector<DecodeEntry> > v(1<<bitones.size());
	vector<DecodeEntry>::iterator it;
	for (it=this->entries.begin(); it!=this->entries.end(); it++) {
		DecodeEntry tmp = *it;
		v[getval(tmp.signature & msk, bitones)].push_back(tmp);

	}

	for (int j=0; j<(1<<bitones.size()); j++) {
		DecodeTable *tmp=new DecodeTable(v[j], j);
		ret.push_back(tmp);
	}
	return ret;
}

/* generate a signature of value w, the signature is distributed into
 * the bits indicated by bitzeros
 */
InstWord DecodeTable::bits_assemble(unsigned int w,
	vector<unsigned int>& bitzeros)
{
	InstWord j=1u, ret=0u;
	vector<unsigned int>::iterator it;
	for (it=bitzeros.begin(); it!=bitzeros.end(); it++, j<<=1) {
		if ((w&j)!=0u) ret |= 1u<<*it;
	}
	return ret;
}

/* split a pattern into multiple patterns */
vector<InstWord> DecodeTable::split_pattern(unsigned int power,
	InstWord mask, InstWord signature)
{
	vector<InstWord> ret;
	InstWord msk = n_ones(power)&mask;

	/* first we count the bit of 0s in msk, those are the ones to split */
	unsigned int i;
	vector<unsigned int> bitzeros;
	for (i=0; i<power; i++) {
		if ((power2(i)&msk)==0u)
			bitzeros.push_back(i);
	}

	/* split to 2^bitzeros ones */
	for (i=0; i<(1u<<bitzeros.size()); i++) {
		ret.push_back((signature&msk)|bits_assemble(i,bitzeros));
	}
	return ret;
}

ostream& operator << (ostream& os, DecodeTable& table)
{
	os << dec << "Table begin: index=" << table.getIndex()
		<< " size=" << table.entries.size()
		<< endl;
	os << hex << "Mask=" << PADDEDHEX << table.mask
		<< "\tDMask=" << PADDEDHEX << table.dmask
		<< "\tCMask=" << PADDEDHEX << table.cmask
		<< "\tSig=" << PADDEDHEX << table.signature << endl;
	os << "Prob=" << setprecision(9) << table.prob
		<< "\tEntropy=" << setprecision(9) << table.entropy 
		<< "\tHTreeHeight=" << setprecision(9) << table.hTreeHeight << endl;
	os << "-------------------------------------------------------------------"
		<< endl;

	vector<DecodeEntry>::iterator it;
	for (it=table.entries.begin(); it!=table.entries.end(); it++)
		os << (*it) << endl;

	os << "-------------------------------------------------------------------"
		<< endl;
	return os;
}


