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

#include <functional>
#include "huffman.h"

using namespace std;

HuffmanTree::huffman_tree(const vector<double>& probs) : nodePtr(0)
{
	this->nodePool = new vector<HuffmanTreeNode>(probs.size()*2);

	vector<double>::const_iterator it;
	for (it = probs.begin(); it != probs.end(); it++) {

		/* exclude those with 0 probability ones,
		they should not take space in the tree */
		if ((*it)>0.0) { 
			HuffmanTreeNode *n = &(*(this->nodePool))[nodePtr++];
			n->prob = *it;
			n->height = 0.0;
			this->nodeList.push_back(n);

		}
	}

	this->nodeList.sort(nodePtrCompare());
	this->root = buildTree();
}

HuffmanTree::~huffman_tree()
{
	delete nodePool;
}

HuffmanTreeNode *HuffmanTree::buildTree()
{
	while (!this->nodeList.empty()) {

		HuffmanTreeNode *c1 = *(this->nodeList.begin());
		this->nodeList.pop_front();
		if (this->nodeList.empty())
			return c1;

		HuffmanTreeNode *c2 = *(this->nodeList.begin());
		this->nodeList.pop_front();

		HuffmanTreeNode *n = &(*(this->nodePool))[nodePtr++];
		n->prob = c1->prob + c2->prob;
		n->height = n->prob + c1->height + c2->height;
		n->left_child = c1;
		n->right_child = c2;

		list<HuffmanTreeNode *>::iterator it;
		for (it = this->nodeList.begin(); it != this->nodeList.end(); it++)
			if (nodePtrCompare()(n, *it)) break;

		this->nodeList.insert(it, n);
	}

	return NULL;
}

