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

#include "symbol.hpp"

using std::vector;
using std::string;
using std::ostream;

using namespace issgen;

static SymbolTable mad_symbol_table;

symbol_table::symbol_table() :
	index(1), table(), hasher()
{
	table.reserve(default_table_size);
	table.push_back("dummy");
}

symbol_table::symbol_table(unsigned int size) :
	index(1), table(), hasher()
{
	table.reserve(size);
	table.push_back("dummy");
}

symbol_table::~symbol_table()
{
}

Symbol symbol_table::find_or_insert(const string& key)
{
	hash_map::iterator it = hasher.find(key.c_str());
	if (it==hasher.end())
	{
		table.push_back(key);
		hasher[key.c_str()] = index;
		return index++;
	}
	else {
		return it->second;
	}
}

Symbol symbol_table::find(const string& key)
{
	hash_map::iterator it = hasher.find(key.c_str());
	if (it==hasher.end()) return 0;
	else return it->second;
}

Symbol issgen::symbol_find_or_insert(const string& s)
{
	return mad_symbol_table.find_or_insert(s); 
}

Symbol issgen::symbol_find(const string& s)
{
	return mad_symbol_table.find(s);
}                                                         
                                                              
const string& issgen::symbol_get_string(Symbol key)
{
	return mad_symbol_table.get_string(key);
}

ostream& issgen::operator << (ostream &os, symbol_table& table)
{
	for (unsigned int i=1; i<table.index; i++)
		os << i << " " << table.table[i] << std::endl;
	return os;
}

#ifdef __TEST_SYMBOL__

int main()
{
	SymbolTable table;

	table.find_or_insert("Wei");
	table.find_or_insert("Qin");
	table.find_or_insert("");
	table.find_or_insert("is");
	table.find_or_insert("a");
	table.find_or_insert("good");
	table.find_or_insert("guy");
	table.find_or_insert("!");
	table.find_or_insert("\n");
	table.find_or_insert("wei");
	table.find_or_insert("wei");
	table.find_or_insert("wei");

	std::cout << table;
	std::cout << table.find("hello") << table.find("wei") << std::endl;

	return 0;
}

#endif
