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

#ifndef _DECODE_THEILING_H_
#define _DECODE_THEILING_H_

#include "decode_table.h"
#include <functional>

#define HEIGHT_THRESHOLD 0.1

typedef class decode_theiling_node DecodeTheilingNode;
class decode_theiling_node {

	public:

		/* constructors */
		decode_theiling_node(std::vector<DecodeEntry>& es,
			InstWord cum_mask) : cum_mask(cum_mask) {
			table = new DecodeTable(es);
		}

		decode_theiling_node(DecodeTable* t,
			InstWord cum_mask) : table(t), cum_mask(cum_mask) {}

		/* destructor*/
		~decode_theiling_node();

		/* decode the table */
		void decode();

		/* get average decoding height of the table */
		double getHeight() {return height;}

        /* get the length of the longest decoding path */
		unsigned getMaxHeight() {return maxHeight;}

		/* get the length of the shortest decoding path */
		unsigned getMinHeight() {return minHeight;}

        /* get the sum of sizes of all tables below the node */
		unsigned getTotalTableSize() {return totalTableSize;}

		/* get the total number of inner nodes under the node */
		unsigned getTotalNodeCount() {return totalNodeCount;}
		
		/* check wellformedness */
		bool isWellFormed() {return table->checkWellFormedness();}

		/* print the decoder in XML form */
		void emitXML(std::ostream& os, const std::string& indent=" ");

		/* print the decoder C code */
		void emitC(std::ostream& os);

	private:

		DecodeTable *table;

		/* decoding schemes */
		enum {
			DECODE_NULL,                                      
			DECODE_CHECK,                                      
			DECODE_TABLE                                      
		} scheme;   

		/* decoding information */
		InstWord tab_mask;
		InstWord cum_mask;	/* undecoded bits */

		std::vector<decode_theiling_node *> children;

		unsigned totalTableSize;
		unsigned totalNodeCount;
		unsigned maxHeight;
		unsigned minHeight;
		double height;

#if 0
		friend class nodePtrCompare: public std::less<DecodeTreeNode *> {
			public:
				bool operator()(class decode_theiling_node *lhs,
					class decode_theiling_node *rhs) {
					return lhs->table.getIndex() < rhs->table.getIndex();
				}
		};
#endif

		int label;

		void labelTableNode();
		void emitCStubs(std::ostream &os);
		void emitCStubDecs(std::ostream &os);
		void emitCArrays(std::ostream &os);
		void emitCCode(std::ostream &os, const std::string& indent=" ");
		void emitCall(std::ostream &os, const std::string& indent);

};

#endif
