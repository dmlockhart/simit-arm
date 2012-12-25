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

#ifndef __MAD_SYMBOL_HPP__
#define __MAD_SYMBOL_HPP__

#ifdef __GNUC__
#if __GNUC__ < 3
#include <hash_map.h>
namespace Sgi { using ::hash_map; using ::hash; }; // inherit globals
#else
#include <ext/hash_map>
#if __GNUC_MINOR__ == 0 && __GNU_C__ == 3
namespace Sgi = std;               // GCC 3.0
#else
namespace Sgi = ::__gnu_cxx;       // GCC 3.1 and later
#endif
#endif
#else      // ...  there are other compilers, right?
namespace Sgi = std;
#endif

#include <string>
#include <iostream>
#include <vector>

/**
 *  Namespace for utilities.
 */
namespace issgen {

	/** Symbol is unsigned integer.  */
	typedef unsigned int Symbol;
	
	/** Default table size to reserve initially.  */
	const unsigned int default_table_size=256;

	/** The NULL symbol.  */
	const Symbol SymNULL=0;

	/** Symbol table class.  */
	typedef class symbol_table {

		public:

			/**
			 *  Constructor 1.
			 */
			symbol_table();

			/**
			 *  Constructor 2.
			 *  @param size  initially reserved size of table.
			 */
			symbol_table(unsigned int size);

			/** Destructor. */
			~symbol_table();

			/**
			 *  Insert or find a string.
			 *  @param val  the string to insert.
			 *  @return  The symbol value of the string, must be >0.
			 */
			Symbol find_or_insert(const std::string& val);

			/**
			 *  Find a string in table.
			 *  @param val  the string to find.
			 *  @return  The symbol value of the string, SymNULL if not found.
			 */
			Symbol find(const std::string& val);

			/**
			 *  Get the value of a symbol.
			 *  @param key  the symbol value.
			 *  @return  The string to be found.
			 */
			const std::string& get_string(Symbol key) {
				return table[key];
			}

		private:

			/** Functor used by the hash class to compare string */
			struct strEql {
				bool operator()(const char* sz1, const char* sz2) {
					return strcmp(sz1,sz2) == 0;
				}
			};

			/* current end of the table */
			unsigned int index;

			/* storage for strings */
			std::vector<std::string> table;

			/* mapping from string to integer */
			typedef Sgi::hash_map<const char *, unsigned int,
				Sgi::hash<char *>, strEql> hash_map;
			hash_map hasher; 

			/* output function */
			friend std::ostream& operator << (std::ostream&, symbol_table&);

	} SymbolTable;

	/** Short cut to access the global symbol table */
	Symbol symbol_find_or_insert(const std::string& s);

	/** Short cut to access the global symbol table */
	Symbol symbol_find(const std::string& s);

	/** Short cut to access the global symbol table */
	const std::string& symbol_get_string(Symbol key);

	/** Dump a symbol table */
	std::ostream& operator << (std::ostream&, symbol_table&);
}

#endif
