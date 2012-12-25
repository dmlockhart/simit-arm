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

#ifndef _DECODE_TREE_H_
#define _DECODE_TREE_H_

#include "decode_table.h"
#include <functional>

#define HEIGHT_THRESHOLD 0.1

typedef class decode_tree_node DecodeTreeNode;
class decode_tree_node {

	public:

		/* constructors */
		decode_tree_node(std::vector<DecodeEntry>& es) {
			table = new DecodeTable(es);
		}

		decode_tree_node(DecodeTable* t) : table(t) {}

		/* destructor*/
		~decode_tree_node();

		/* decode the table */
		void decode(double minUtilization);

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

		/* fix the decoding entries */
		bool fixEntries() {return table->fixEntries();}

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
			DECODE_PATTERN,
			DECODE_TABLE
		} scheme;

		/* decoding information, supplement to scheme */
		InstWord pat_mask;
		InstWord pat_signature;

		InstWord tab_mask;
		unsigned int tab_shift;

		std::vector<decode_tree_node *> children;

		unsigned totalTableSize;	/* # of table entries */
		unsigned totalNodeCount;	/* # of internal nodes */
		unsigned maxHeight;			/* worst case height */
		unsigned minHeight;			/* best case height */
		double height;				/* averate height */

		double gamma;

#if 0
		friend class nodePtrCompare: public std::less<DecodeTreeNode *> {
			public:
				bool operator()(class decode_tree_node *lhs,
					class decode_tree_node *rhs) {
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

};

#endif
