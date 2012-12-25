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
#include "decode_theiling.h"

#define DEBUG 0

using namespace std;

extern bool do_onebit;

DecodeTheilingNode::~decode_theiling_node()
{
	delete table;
	/* delete children */
	vector<DecodeTheilingNode *>::iterator it;
	for (it=children.begin(); it!=children.end(); it++)
		delete *it;
}

/* main decoding entrance */
void DecodeTheilingNode::decode()
{

	InstWord cmask = this->table->getCMask();

	this->totalTableSize = 0;
	this->totalNodeCount = 0;

	/* check if this is a trivial node */
	if (this->table->getUSize()<=1 || this->table->getHTreeHeight()==0.0) {
		if (this->table->getSize()==1 &&
			(this->cum_mask&this->table->getEntries().front().mask)!=0u) {
			this->scheme = DECODE_CHECK;
			this->height = 1.0;
			this->maxHeight = 1;
			this->minHeight = 1;
		}
		else {
			if (this->table->getSize()>0 &&
			(this->cum_mask&this->table->getEntries().front().mask)!=0u) {
				cerr << "Warning: decoder not safe!" << endl;
			}
			this->scheme = DECODE_NULL;
			this->height = 0.0;
			this->maxHeight = 0;
			this->minHeight = 0;
		}
		return;
	}

#if DEBUG>0
	cout << *(this->table);
	cout << "cum_mask " << this->cum_mask << endl;
#endif

	if (cmask==0u) {
		cerr << "Fatal: deadlock occurred!\n" << endl;
		exit (1);
	}


	vector<DecodeTable *> subs =
		this->table->divide_by_theiling_table(cmask);
	this->totalTableSize = 1<<cmask.pop_count();//subs.size();
	this->totalNodeCount = 1;
	this->height = 1.0;
	this->maxHeight = 0;
	this->minHeight = (unsigned)-1;

	vector<DecodeTable *>::iterator it;
	for (it=subs.begin(); it!=subs.end(); it++) {
		DecodeTheilingNode *t1 = new DecodeTheilingNode(*it,
			this->cum_mask & (~cmask));
		t1->decode();
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
	this->tab_mask= cmask;                      
	this->scheme = DECODE_TABLE;

}

void DecodeTheilingNode::emitXML(ostream& os, const string &indent)
{
	os << indent << "<table index=\"" << this->table->getIndex() 
		<< "\">" << endl;
	os << indent << "  <size>"<< this->table->getSize() << "</size>" << endl;

	switch (scheme) {
		case DECODE_CHECK:
		case DECODE_NULL: {
			const vector<DecodeEntry>& entries = this->table->getEntries();
			vector<DecodeEntry>::const_iterator it;
			for (it=entries.begin(); it!=entries.end(); it++) {
				os << indent << "  <entry>"
					<< (*it).label << "  </entry>" << endl;
			}
			break;
		}
		case DECODE_TABLE: {
			os << indent << "  <decode_table mask=\""
				<< PADDEDHEX << this->tab_mask
				<< "\"/>" << endl;
			break;
		}
	}

	vector<DecodeTheilingNode *>::iterator it;
	for (it=children.begin(); it!=children.end(); it++)
		(*it)->emitXML(os, indent+"  ");

	os << indent <<  "  </table>" << endl;
}

static int static_label = 0;

void DecodeTheilingNode::labelTableNode()
{
	switch(scheme) {
		case DECODE_TABLE: {
			this->label = static_label++;
			vector<DecodeTheilingNode *>::iterator it;
			for (it=children.begin(); it!=children.end(); it++) {
				(*it)->labelTableNode();
			}
		}
		default: break;
	}
}

void DecodeTheilingNode::emitCStubDecs(ostream &os)
{
	switch(scheme) {
		case DECODE_TABLE: {
			vector<DecodeTheilingNode *>::iterator it;
			for (it=children.begin(); it!=children.end(); it++) {
				if ((*it)->scheme!=DECODE_NULL)
					os << "_STUB_DEC(stub_" << dec << this->label
						<< "_" << (*it)->table->getIndex() << ")" << endl;
			}
			for (it=children.begin(); it!=children.end(); it++) 
				(*it)->emitCStubDecs(os);
		}
		default: break;
	}
}

void DecodeTheilingNode::emitCArrays(ostream &os)
{
	switch(scheme) {
		case DECODE_TABLE: {

			os << "/*" << endl << *(this->table);
			os << "Table Mask=" << PADDEDHEX
				<< this->tab_mask << "*/" << endl << endl;
			os << "_TABLE_DEF_BEGIN(table_" << dec << this->label
				<< ", " << children.size() << ")" << endl;

			vector<DecodeTheilingNode *>::iterator it;
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
			for (it=children.begin(); it!=children.end(); it++)
				(*it)->emitCArrays(os);                       
		}
		default: break;
	}
}

static vector<pair<unsigned int, unsigned int> > break_mask(InstWord x)
{
	bool flag=false;
	unsigned int from=0, len=0, cumlen=0;
	vector<pair<unsigned int, unsigned int> > ret;

	for (unsigned int i=0;i<INSTBITS;i++) {
		if ((x&power2(i))!=0u) {
			if (flag) len++;
			else {
				from = i;
				len = 1;
				flag = true;
			}
		}
		else if (flag) {
			flag = false;
			ret.push_back(pair<unsigned int, unsigned int>
					(((1u<<len)-1)<<cumlen, from-cumlen));	
			cumlen += len;
		}
	}
	if (flag) {
		ret.push_back(pair<unsigned int, unsigned int>
					(((1u<<len)-1)<<cumlen, from-cumlen));	
	}
	return ret;
}

void DecodeTheilingNode::emitCall(ostream &os, const string& indent)
{
	switch(scheme) {
		case DECODE_NULL:
			assert(this->table->getSize());
			os << indent << "_FUNC_CALL(" 
				<< (this->table->getEntries().front()).label
				<< ");" << endl;
			break;
		case DECODE_CHECK: {
			InstWord m = this->cum_mask&
					this->table->getEntries().front().mask;
			os << indent << "if _PATTERN_TRUE("
				<< PADDEDHEX << m << "," << PADDEDHEX << (m&
                   (this->table->getEntries().front()).signature)
				<< ") {" << endl
				<< indent << "  _FUNC_CALL(" 
				<< (this->table->getEntries().front()).label
				<< ");" << endl
				<< indent << "} else _FUNC_CALL(default);" << endl;
			}
			break;
		default: {
			vector<pair<unsigned int, unsigned int> > ms =
				break_mask(this->tab_mask);
			assert(ms.size());
			os << indent << "_TABLE_JUMP" << ms.size() 
				<<	"(table_" << dec << this->label;
		    for (unsigned int i=0;i<ms.size();i++)
				os << ", " << ms[i].first << ", " << ms[i].second;
			os << "); /* " << PADDEDHEX << this->tab_mask << " */" << endl;
			}
			break;
	}
}

void DecodeTheilingNode::emitCCode(ostream &os, const string& indent)
{
	switch(scheme) {
		case DECODE_TABLE: {
			if (this->tab_mask.pop_count()==1) {
				os << indent << "if _PATTERN_TRUE("
					<< PADDEDHEX << this->tab_mask << ", "
					<< PADDEDHEX << this->tab_mask << ") {" << endl;
				children[1]->emitCall(os, indent);
				os << indent << "} else {" << endl;
				children[0]->emitCall(os, indent);
				os << indent << "}" 
					<< "/* " << PADDEDHEX << this->tab_mask << " */" << endl;
			}
			else {
				os << "/* " << this->tab_mask.pop_count() << "*/" << endl;
			emitCall(os, indent);
			}
			break;
		}
		case DECODE_NULL:
			assert(this->table->getSize());
			emitCall(os, indent);
#if 0
			os << indent << "FUNC_CALL(" 
				<< (this->table->getEntries().front()).label
				<< ");" << endl;
#endif
			break;

		case DECODE_CHECK: {
			emitCall(os, indent);
			break;
		}

		default: break;
	}
}

void DecodeTheilingNode::emitCStubs(ostream &os)
{
	switch(scheme) {
		case DECODE_TABLE: {

			vector<DecodeTheilingNode *>::iterator it;
			for (it=children.begin(); it!=children.end(); it++) {
				if ((*it)->scheme!=DECODE_NULL) {
					os << "_STUB_ENTRY(stub_" << dec << this->label
						<< "_" << (*it)->table->getIndex() << ")" << endl
						<< "{" << endl;
					(*it)->emitCCode(os);
					os << "}" << endl << endl;
				}
			}
			for (it=children.begin(); it!=children.end(); it++)
				(*it)->emitCStubs(os);
			break;
		}
#if 0
		case DECODE_CHECK: {
			os << "**STUB_ENTRY(stub_" << dec << this->label
				<< "_" << this->table->getIndex() << ")" << endl
				<< "{" << endl;
			this->emitCCode(os);
			os << "}" << endl << endl;
		}
#endif
						   
		default:
			break;
	}
}

void DecodeTheilingNode::emitC(ostream &os)
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
	os << "Using Theiling algorithm" << endl;
	os << "Decoder characteristics " << endl;
	os << "Average height : " << this->height << endl;   
    os << "Maximum height : " << this->maxHeight << endl;
    os << "Minimum height : " << this->minHeight << endl;
	os << "Table entries  : " << this->totalTableSize << endl;
	os << "Total nodes    : " << this->totalNodeCount << endl;
	os << "*/" << endl << endl;

	/* include the C macro definition file */
	os << "/* Include the C macro definition file */" << endl;
	os << "#include \"autodecoder.h\"" << endl << endl;

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
