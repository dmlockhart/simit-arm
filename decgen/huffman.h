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

#ifndef _HUFFMAN_H_
#define _HUFFMAN_H_

#include <list>
#include <vector>
#include <functional>

typedef struct node_t {

	double prob;
	double height;
	node_t *left_child;	
	node_t *right_child;

} HuffmanTreeNode;

typedef class huffman_tree {


	public:
		huffman_tree(const std::vector<double>& probs);
		~huffman_tree();

		double getHeight() {return (root)?root->height:0.0;}

	private:
		std::vector<HuffmanTreeNode> *nodePool;
		int nodePtr;

		/* functor for sorting */
		class nodePtrCompare: public std::less <HuffmanTreeNode *> {
			public:
				bool operator()(HuffmanTreeNode *lhs, HuffmanTreeNode *rhs) {
					return lhs->prob < rhs->prob;
				}
		};

		std::list<HuffmanTreeNode *> nodeList;
		HuffmanTreeNode *root;

		HuffmanTreeNode *buildTree();

} HuffmanTree;

#endif
