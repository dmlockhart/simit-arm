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
#ifndef DECOMP_SERVER_H
#define DECOMP_SERVER_H

#include <misc.h>

namespace simit {

class decomp_server {


  public:

	/* constructor */
	decomp_server(bool,  uint32_t, bool);

	/* destructor */
	~decomp_server();

	/* to run the server */
	uint32_t run(const char *path);

	uint32_t get_count() const {return decomp_count;}

  private:

	bool compile_block(const byte_t *pblk, uint32_t size, unsigned ind,
		bool user_level, const char *path);

	/* emulator */
	bool verbose;
	uint32_t portnum;
	bool tolink;
	uint32_t decomp_count;


};


    
}


#endif
