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
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <cassert>
#include "decode_tree.h"

#define DEBUG 0

using namespace std;

extern bool do_onebit;

DecodeTreeNode::~decode_tree_node()
{
	delete table;
	/* delete children */
	vector<DecodeTreeNode *>::iterator it;
	for (it=children.begin(); it!=children.end(); it++)
		delete *it;
}

/* main decoding entrance */
void DecodeTreeNode::decode(double minUtilization)
{
	this->gamma = minUtilization;

	InstWord mask = this->table->getMask();

	this->totalTableSize = 0;
	this->totalNodeCount = 0;

	/* check if this is a trivial node */
	if (this->table->getUSize()<=1 || this->table->getHTreeHeight()==0.0) {
		this->scheme = DECODE_NULL;
		this->height = 0.0;
		this->maxHeight = 0;
		this->minHeight = 0;
		return;
	}

#if DEBUG>0
	cout << *(this->table);
#endif

	/* first, see if any pattern works well
	 * starting from empty, we grow a pattern bit by bit
	 */
	double minPatternHeight = 1e+200;
	double maxPatternGain = -1e+200;
	double minPatternRatio = 1.0;
	InstWord minPatternSignature = 0u;
	InstWord minPatternMask = 0u;

	/* 1 bit case is symmetric, both positive and negative signature
	 * will generate the same height, we need to try out more bits from both */
	unsigned int numBits = 0;
	bool moveOn = true;
	while (moveOn) {

		moveOn = false;
		double maxGain = -1e+200;
		double minRatio = 0.0;
		double minHeight = 0.0;
		InstWord minMask;
		InstWord minSignature;

		for (unsigned int bitPos=0; bitPos<INSTBITS; bitPos++) {

			InstWord theBit = power2(bitPos);

			/* if the bit is already within current pattern, or
			 * if the bit is a don't care for the table, we skip
			 */
			if ((theBit&minPatternMask)!=0u || (theBit&mask)==0u) continue;

			/* when numBits = 0, 1 bit pattern, symmetric, test only one case */
			/* when numBits = 1, 2 bit pattern, test four cases */
			/* when numBits > 1, test only two cases */
			int ntry = (numBits==0)?1:(numBits==1)?4:2;
			for (int k=0; k<ntry; k++) {

				InstWord currentMask = minPatternMask|theBit;
				InstWord currentSignature = minPatternSignature;

				/* two bit pattern has four cases */
				if (numBits==1) {
					switch (k) {
						case 0:
							currentSignature = 0u; break;
						case 1:
							currentSignature = theBit; break;
						case 2:
							currentSignature = minPatternMask; break;
						case 3:
							currentSignature = currentMask; break;
						default: /*no such case */ break;
					}
				}
				else {
					if (k) currentSignature |= theBit;
				}

				/* try to split the table based on the pattern */
				vector<DecodeTable *> subs = this->table->divide_by_pattern
					(currentMask, currentSignature);

				/* check if the pattern is useless */
				if (subs[1]->getSize()==0 || subs[0]->getSize()==0) {
					delete subs[0];
					delete subs[1];
					continue;
				}

				/* calc. the new average decoding height */
				double height = subs[0]->getProb()*subs[0]->getHTreeHeight() +
					subs[1]->getProb()*subs[1]->getHTreeHeight() + 1;

				/* calc. the utilization ratio as 1/mr (see paper) */
				double totalSize = 1;
				totalSize += subs[0]->getUSize()?(subs[0]->getUSize()-1):0;
				totalSize += subs[1]->getUSize()?(subs[1]->getUSize()-1):0;
				double uratio = (double)(this->table->getUSize()-1)/totalSize;

				/* calc. the gain as the negation of Hc (see paper) */
				double gain = - height;
				gain += minUtilization*(log(uratio)/log(2.0));
				if (gain>maxGain && uratio>MINUTILRATE) {
					maxGain = gain;
					minMask = currentMask;
					minSignature = currentSignature;
					minHeight = height;
					minRatio = uratio;
#if 0
					cout << "***" << minMask << "***"; 
					cout << "***" << minSignature << "***";
					cout << "***" << minHeight << "***";
					cout << "***" << minRatio << "***" << endl;
#endif
				}

				delete subs[0];
				delete subs[1];
			}
		}

		/* check if worthwhile to grow the new bit */
		if (maxGain>maxPatternGain) {
			minPatternHeight = minHeight;
			minPatternSignature = minSignature;
			minPatternMask = minMask;
			minPatternRatio = minRatio;
			maxPatternGain = maxGain;
			if (minPatternHeight!=1.0) moveOn = true;
		}

#if DEBUG>1
		cout << dec << numBits << " bit patterns: "
			<< " minHeight "
			<< minPatternHeight
			<< " minMask "
			<< PADDEDHEX << minPatternMask
			<< " minSignature "
			<< PADDEDHEX << minPatternSignature
			<< " uratio " << minRatio
			<< endl;
#endif
		numBits++;

		if (do_onebit) break;
	}

	/* next, see how table works */
	unsigned int minTableShift = 0, minTableBits = 0;
	double minTableHeight = minPatternHeight;
	double minTableRatio = 1.0;
	double maxTableGain =  -1e+200;
	InstWord minTableMask;

	/* table has at least 4 entries */
	numBits = 1;
	moveOn = true;
	while (moveOn && (!do_onebit)) {

		moveOn = false;
		numBits++;

		double minHeight = 1e+200;
		double maxGain = -1e+200;
		double minRatio =0.0;
		InstWord minMask;
		unsigned int minShift =0;

		InstWord tableMask = n_ones(numBits);
		for (unsigned int tableShift=0; tableShift<=INSTBITS-numBits;
			tableShift++) {

			InstWord msk = tableMask<<tableShift;

			/* if msk contains don't care bits, skip */
			if ((msk&mask)!=msk) continue;
#if 0
			cout << "***" << msk << "***" << endl;
#endif

			/* split into 2^numBits tables */
			vector<DecodeTable *> subs = this->table->divide_by_table(msk,
				numBits, tableShift);

			/* calc. table size
			 * calc. average decoding height
			 */
			unsigned int totalSize = 1;
			double height = 1.0;
			vector<DecodeTable *>::iterator it;
			for (it=subs.begin(); it!=subs.end(); it++) {
				height += (*it)->getProb()*(*it)->getHTreeHeight();
				totalSize += (*it)->getUSize()?(*it)->getUSize():1;
				delete *it;
			}

			/* calc. the utilization ratio as 1/mr */
			double uratio = (double)(this->table->getUSize()-1)/totalSize;

			/* calc. the gain as -Hc */
			double gain = - height;
			gain += minUtilization*(log(uratio)/log(2.0));

			if (gain>maxGain && uratio>MINUTILRATE) {
				maxGain = gain;
				minHeight = height;
				minMask = msk;
				minShift = tableShift;
				minRatio = uratio;
			}
		}

#if DEBUG>1
		cout << dec << numBits << " bit tables :"
			<< " minHeight " << minHeight
			<< " minMask " << PADDEDHEX << minMask
			<< " uratio " << minRatio
			<< endl;
#endif
		if (maxGain>maxTableGain) {
			minTableHeight = minHeight;
			minTableMask = minMask;
			minTableShift = minShift;
			minTableBits = numBits;
			minTableRatio = minRatio;
			maxTableGain = maxGain;
			if (minTableHeight!=1.0) moveOn = true;
		}
	}

	/* now decode recursively */
	if (maxPatternGain>=maxTableGain) {
		this->scheme = DECODE_PATTERN;
#if DEBUG>0
		cout << "decode by pattern: " 
			<< " mask " << PADDEDHEX << minPatternMask
			<< " signature " << PADDEDHEX << minPatternSignature
			<< endl;
#endif
	} else {
		this->scheme = DECODE_TABLE;
#if DEBUG>0
		cout << "decode by table: "
			<< " mask " << PADDEDHEX << minTableMask
			<< endl;
#endif
	}

	if (this->scheme==DECODE_PATTERN) {	/* pattern decode */

		vector<DecodeTable *> subs = this->table->divide_by_pattern
			(minPatternMask, minPatternSignature);

		DecodeTreeNode *t1 = new DecodeTreeNode(subs[0]);
		DecodeTreeNode *t2 = new DecodeTreeNode(subs[1]);

		t1->decode(minUtilization);
		t2->decode(minUtilization);

		this->totalTableSize = t1->totalTableSize + t2->totalTableSize;
		this->totalNodeCount = t1->totalNodeCount + t2->totalNodeCount + 1; 
		this->children.push_back(t1);
		this->children.push_back(t2);

		this->height = t1->getHeight()*subs[0]->getProb() +
			t2->getHeight()*subs[1]->getProb() + 1;
		this->maxHeight = 1 + (t1->getMaxHeight()>t2->getMaxHeight()?
			t1->getMaxHeight():t2->getMaxHeight());
		this->minHeight = 1 + (t1->getMinHeight()<t2->getMinHeight()?
			t1->getMinHeight():t2->getMinHeight());

		this->pat_mask = minPatternMask;
		this->pat_signature = minPatternSignature;

	}
	else { /* table decode */ 

		vector<DecodeTable *> subs = this->table->divide_by_table(minTableMask,
			minTableBits, minTableShift);
		this->totalTableSize = 1<<minTableBits; //subs.size();
		this->totalNodeCount = 1;
		this->height = 1.0;
		this->maxHeight = 0;
		this->minHeight = (unsigned)-1;

		vector<DecodeTable *>::iterator it;
		for (it=subs.begin(); it!=subs.end(); it++) {
			DecodeTreeNode *t1 = new DecodeTreeNode(*it);
			t1->decode(minUtilization);
			this->totalTableSize += t1->totalTableSize;
			this->totalNodeCount += t1->totalNodeCount;
			this->children.push_back(t1);
			this->height += t1->getHeight()*(*it)->getProb();
			if (t1->getMaxHeight()>this->maxHeight)
				this->maxHeight = t1->getMaxHeight();
			if (t1->getMinHeight()<this->minHeight)
				this->minHeight = t1->getMinHeight();
		}

		this->maxHeight += 1;
		this->minHeight += 1;
		this->tab_mask= minTableMask;                      
		this->tab_shift = minTableShift;
	}

}

void DecodeTreeNode::emitXML(ostream& os, const string &indent)
{
	os << indent << "<table index=\"" << this->table->getIndex() 
		<< "\">" << endl;
	os << indent << "  <size>"<< this->table->getSize() << "</size>" << endl;

	switch (scheme) {
		case DECODE_NULL: {
			const vector<DecodeEntry>& entries = this->table->getEntries();
			vector<DecodeEntry>::const_iterator it;
			for (it=entries.begin(); it!=entries.end(); it++) {
				os << indent << "  <entry>"
					<< (*it).label << "  </entry>" << endl;
			}
			break;
		}
		case DECODE_PATTERN: {
			os << indent << "  <decode_pattern mask=\""
				<< PADDEDHEX << this->pat_mask
				<< "\" pattern=\""
				<< PADDEDHEX << this->pat_signature << "\"/>" << endl;
			break;
		}
		case DECODE_TABLE: {
			os << indent << "  <decode_table mask=\""
				<< PADDEDHEX << this->tab_mask
				<< "\"/>" << endl;
			break;
		}
	}

	vector<DecodeTreeNode *>::iterator it;
	for (it=children.begin(); it!=children.end(); it++)
		(*it)->emitXML(os, indent+"  ");

	os << indent <<  "  </table>" << endl;
}

static int static_label = 0;

void DecodeTreeNode::labelTableNode()
{
	switch(scheme) {
		case DECODE_TABLE: 
			this->label = static_label++;
		case DECODE_PATTERN: {
				vector<DecodeTreeNode *>::iterator it;
				for (it=children.begin(); it!=children.end(); it++) {
					(*it)->labelTableNode();
				}
			}
		default: break;
	}
}

void DecodeTreeNode::emitCStubDecs(ostream &os)
{
	switch(scheme) {
		case DECODE_TABLE: {
			vector<DecodeTreeNode *>::iterator it;
			for (it=children.begin(); it!=children.end(); it++) {
				if ((*it)->scheme!=DECODE_NULL)
					os << "_STUB_DEC(stub_" << dec << this->label
						<< "_" << (*it)->table->getIndex() << ")" << endl;
			}
		}
		case DECODE_PATTERN: {
			vector<DecodeTreeNode *>::iterator it;
			for (it=children.begin(); it!=children.end(); it++) {
				(*it)->emitCStubDecs(os);
			}
		}
		default: break;
	}
}

void DecodeTreeNode::emitCArrays(ostream &os)
{
	switch(scheme) {
		case DECODE_TABLE: {

			os << "/*" << endl << *(this->table);
			os << "Table Mask=" << PADDEDHEX
				<< this->tab_mask << "*/" << endl << endl;
			os << "_TABLE_DEF_BEGIN(table_" << dec << this->label
				<< ", " << children.size() << ")" << endl;

			vector<DecodeTreeNode *>::iterator it;
			for (it=children.begin(); it!=children.end(); it++) {
				if ((*it)->scheme!=DECODE_NULL) {
					os << "  _STUB_NAME(stub_" << dec << this->label
						<< "_" << (*it)->table->getIndex() << ")," << endl;
				}
				else if (((*it)->table)->getSize()>0) {
					DecodeTable *table = (*it)->table;
					os << "  _FUNC_NAME("
						<< (table->getEntries().front()).label
						<< ")," << endl;
				}
				else os << "  _FUNC_DEFAULT," << endl;
			}
			os << "_TABLE_DEF_END" << endl << endl;
		}
		case DECODE_PATTERN: {
			vector<DecodeTreeNode *>::iterator it;
			for (it=children.begin(); it!=children.end(); it++)
				(*it)->emitCArrays(os);
			break;
		}
		default: break;
	}
}

void DecodeTreeNode::emitCCode(ostream &os, const string& indent)
{
	switch(scheme) {
		case DECODE_TABLE:
			os << indent << "_TABLE_JUMP(table_" << dec << this->label
				<< ", " << PADDEDHEX << (this->tab_mask>>this->tab_shift)
				<< ", " << dec << this->tab_shift << ");" << endl;
			break;

		case DECODE_PATTERN:

			os << indent << "if _PATTERN_TRUE("
				<< PADDEDHEX << this->pat_mask << ", "
				<< PADDEDHEX << this->pat_signature << ") {" << endl;

			this->children[0]->emitCCode(os, indent+"  ");
			os << indent << "} else {" << endl;
			this->children[1]->emitCCode(os, indent+"  ");
			os << indent << "}" << endl;
			break;

		case DECODE_NULL:
			assert(this->table->getSize());
			os << indent << "_FUNC_CALL(" 
				<< (this->table->getEntries().front()).label
				<< ");" << endl;
		default: break;
	}
}

void DecodeTreeNode::emitCStubs(ostream &os)
{
	switch(scheme) {
		case DECODE_TABLE: {

			vector<DecodeTreeNode *>::iterator it;
			for (it=children.begin(); it!=children.end(); it++) {
				if ((*it)->scheme!=DECODE_NULL) {
					os << "_STUB_ENTRY(stub_" << dec << this->label
						<< "_" << (*it)->table->getIndex() << ")" << endl
						<< "{" << endl;
					(*it)->emitCCode(os);
					os << "}" << endl << endl;
				}
			}
		}
		case DECODE_PATTERN: {
				vector<DecodeTreeNode *>::iterator it;
				for (it=children.begin(); it!=children.end(); it++)
					(*it)->emitCStubs(os);
				break;
			}
		default:
			break;
	}
}

void DecodeTreeNode::emitC(ostream &os)
{
	/* first label all tables */
	labelTableNode();

	/* preamble of the file */
	os << "/*" << endl
		<< "Binary decoder synthesized by " PACKAGE " version " VERSION
	    << endl << endl;

	os << "Input statistics " << endl;
	os << "Total entries  : " << this->table->getEntries().size() << endl;
	os << "Unique labels  : " << this->table->getUSize() << endl;
	os << "Shannon entropy     : " << this->table->getEntropy() << endl;
	os << "Huffman tree height : " << this->table->getHTreeHeight() << endl;

	os << endl;
	os << "Decoder characteristics " << endl;
	os << "Gamma          : " << this->gamma << endl;
	os << "1 bit only     : " << do_onebit << endl;
	os << "Average height : " << this->height << endl;   
	os << "Maximum height : " << this->maxHeight << endl;   
	os << "Minimum height : " << this->minHeight << endl;   
	os << "Table entries  : " << this->totalTableSize << endl;
	os << "Total nodes    : " << this->totalNodeCount << endl;
	os << "*/" << endl << endl;

	/* include the C macro definition file */
	//os << "/* Include the C macro definition file */" << endl;
	//os << "#include \"autodecoder.h\"" << endl << endl;

	/* then emit the stubs function names */
	emitCStubDecs(os);
	os << endl;

	/* then emit the arrays */
	emitCArrays(os);
	os << endl;

	/* the emit the stubs functions */
	emitCStubs(os);
	os << endl;

	/* finally emit the decode routine */
	os << "_MAIN_DECODE_ENTRY" << endl << "{" << endl;
	emitCCode(os);
	os << "}" << endl;

	os << "/* End of decoder */" << endl;
}
