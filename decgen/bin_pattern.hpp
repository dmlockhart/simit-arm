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


#ifndef __MAD_BIN_PATTERN_HPP__
#define __MAD_BIN_PATTERN_HPP__

#include <iostream>
#include <string>
#include <cassert>
#include <list>
#include "config.h"

/** binary pattern class. */
typedef class binary_pattern {

	public:

		/** Constructor. */
		binary_pattern(const std::string& d);

		/** Constructor. */
		binary_pattern(unsigned len);

		/** Copy constructor. */
		binary_pattern(const binary_pattern& pat);

		/** Destructor. */
		~binary_pattern();

		/** Assignment operator */
		binary_pattern& operator = (const binary_pattern& pat);

		/** Get the length of the pattern. */
		unsigned get_length() const {return len;}

		/** Distance between patterns (number of conflicting bits) */
		unsigned distance(const binary_pattern& pat) const;

		bool mergeable(const binary_pattern& pat) const;

		/** Check if this pattern contains the other. */
		bool contain(const binary_pattern& pat) const;

		/** Overlap is weaker than contains. */
		bool overlap(const binary_pattern& pat) const;

#if 0
		/** return a difference vector A-B. */
		std::vector<binary_pattern> diff(const binary_pattern& pat) const;

		/** Get the mask of the pattern, in binary format. */
		std::string get_mask() const;

		/** Get the signature of the pattern, in binary format. */
		std::string get_signature() const;
#endif
		/** Get the string representing the pattern, in '0', '1', '-' */
		std::string get_str() const;

		/** Get the mask of the pattern, in hexadecimal w/o 0x. */
		std::string get_hex_mask() const;

		/** Get the signature of the pattern, in hexadecimal w/o 0x. */
		std::string get_hex_signature() const;

		/** Output the pattern. */
		std::ostream& print(std::ostream& os) const;

	private:

		/* pointer to array of masks and signatures (mask first) 
		 * data[0]'s LSB is the lowest bit
		 */
		unsigned *data; 

		/* length of the pattern */
		unsigned len;

	friend const binary_pattern operator +
			(const binary_pattern& x, const binary_pattern& y);

	friend const binary_pattern operator ^
			(const binary_pattern& x, const binary_pattern& y);

	friend bool operator ==
			(const binary_pattern& x, const binary_pattern& y);

	friend bool operator !=
			(const binary_pattern& x, const binary_pattern& y);

	friend void inverse(std::list<binary_pattern>& pats,
						std::list<binary_pattern>& result);

	friend void exclude_cube(std::list<binary_pattern>& pats,
						const binary_pattern& cube);


} BinPattern;

/** Concatenate two patterns. 
 *  @param x The high order bits.
 *  @param y The low order bits.
 */
const binary_pattern operator +
		(const binary_pattern& x, const binary_pattern& y);

/** Consensus of two patterns. */
const binary_pattern operator ^
		(const binary_pattern& x, const binary_pattern& y);

/** The equality operator */
bool operator == (const BinPattern& x, const BinPattern& y);

/** The inequality operator */
bool operator != (const BinPattern& x, const BinPattern& y);

/** Inverion of the boolean space.
 *  @param pats   The original space.
 *  @param result The inverted space.
 */
void inverse(std::list<binary_pattern>& pats,
		std::list<binary_pattern>& result);

/** Exclude a cube from a list of cubes.
 *  @param pats The cubes to work on.
 *  @param cube The cube to exclude.
 */
void exclude_cube(std::list<binary_pattern>& pats,
		const binary_pattern& cube);

/** Output the pattern. */
std::ostream& operator << (std::ostream& os, const BinPattern& x);


#endif
