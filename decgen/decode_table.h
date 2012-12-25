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

#ifndef _DECODE_TABLE_H_
#define _DECODE_TABLE_H_

#include "decode.h"
#include "huffman.h"

typedef class decode_table DecodeTable;

class decode_table {

	public:

		/* constructor */
		decode_table(std::vector<DecodeEntry>& es, int id=0);

		/* destructor */
		~decode_table() {};

		/* get the decoding entries */
		const std::vector<DecodeEntry>& getEntries() {return entries;}

		/* get the number of entries in the table */
		int getSize() const {return entries.size();}

		/* get the number of unique entries (labels) in the table */
		int getUSize() const {return usize;}

		/* get the entripy of the entries */
		double getEntropy() const {return entropy;}

		/* get the Huffman tree height of the entries */
		double getHTreeHeight() const {return hTreeHeight;}

		/* get the mask of the table
		 * 1's in mask marks the bits that we care about in decoding
		 */
		InstWord getMask() const {return mask;}

		/* get the common mask of the table
		 * 1's in the mask marks the bits that are cared by all entries
		 */
		InstWord getCMask() const {return cmask;}

		/* get the total probability of the entries */
		double getProb() const {return prob;}

		/* fix containing entries */
		bool fixEntries();

		/* check the wellness of the entries, i.e. no two patterns overlap */
		bool checkWellFormedness();

		/* get the index of the table
		 * index specifies the order of the table in its parent node
		 */
		unsigned int getIndex() const {return index;}

		/* both functions set the flag entry_split indicating if
		 * any entries split occurs
		 */
		std::vector<DecodeTable *> divide_by_pattern(InstWord mask,
			InstWord signature);
		std::vector<DecodeTable *> divide_by_table(InstWord mask,
			int bitcount, int rmb);

		/* use the theiling algorithm, divide by non-contiguous bits*/
		std::vector<DecodeTable *> divide_by_theiling_table(InstWord mask);

	private:

		friend std::ostream& operator << (std::ostream&, decode_table&);
		friend class decode_tree_node;
		friend class huffman_tree;

		/* entry probabilities are normalized */
		std::vector<DecodeEntry> entries;

		/* Shannon entropy */
		double entropy;

		/* huffman tree height */
		double hTreeHeight;

		InstWord mask;		/* significant bits mask */
		InstWord dmask;		/* constant bits mask */
		InstWord cmask;		/* common mask (conjunection of all masks) */
		InstWord signature;	/* value of significant bits */

		/* sum of probabilities of all entries before normalzaition */
		double prob;

		unsigned int index;
		unsigned int usize;

		InstWord bits_assemble(unsigned int w,
			std::vector<unsigned int>& bitones);
		std::vector<InstWord> split_pattern(unsigned int p, InstWord mask,
			InstWord signature);

};


#endif
