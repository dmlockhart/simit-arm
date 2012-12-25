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


#include <cstdlib>
#include <fstream>
#include <stdio.h>

#include "decode.h"
#include "huffman.h"
#include "decode_tree.h"
#include "decode_theiling.h"

using std::vector;

extern int yyparse();
extern bool compute_undef(vector<DecodeEntry> &);

using namespace std;

vector<DecodeEntry> entries;
FILE *dec_in;
bool do_onebit = false;

void usage(char *cname)
{
	cerr << "Usage: " << cname << " [options] file" << endl
		<< "Options:" << endl
		<< "  --help, -h     Display this information" << endl
		<< "  -v             Display version information" << endl
		<< "  -o <file>      Place the output C code into <file>" << endl	
		<< "  -x <file>      Generate xml output into <file>" << endl
		<< "  -g <value>     Set gamma as <value>, default is 0.5" << endl
		<< "  -1bit          Use only 1 bit decoding" << endl
		<< "  -theiling      Use the Theiling algorithm" << endl
		<< "  -i             Compute undefined opcode space" << endl	
		<< endl;
	exit(1);
}

void version()
{
	cerr << PACKAGE " version " VERSION << endl;
}

void error(const char *s1, const char *s2="")
{
	cerr << s1 << ' ' << s2 << endl;
	exit(1);
}

int main(int argc, char *argv[])
{
	double gamma = 0.5;
	char *ifname = NULL;
	char *cfname = NULL;	/* output C file */
	char *xfname = NULL;	/* output xml file */
	bool theiling = false;
	bool invert = false;

	int i;

	/* search command line options */
	for (i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-o")==0) cfname = argv[++i];
		else if (strcmp(argv[i], "-i")==0) invert = true;
		else if (strcmp(argv[i], "-x")==0) xfname = argv[++i];
		else if (strcmp(argv[i], "-g")==0) gamma= atof(argv[++i]);
		else if (strcmp(argv[i], "-1bit")==0) do_onebit=true;
		else if (strcmp(argv[i], "-theiling")==0) theiling=true;
		else if (strcmp(argv[i], "-h")==0) usage(argv[0]);
		else if (strcmp(argv[i], "--help")==0) usage(argv[0]);
		else if (strcmp(argv[i], "-v")==0) version();
		else if (argv[i][0]=='-') error("Invalid argument",argv[i]);
		else break;
	}

	if (i==argc) exit(0);

	/* open input file
	 * I have to use old C stuff since flex is not quite up-to-date
	 * with ISO C++. Newer flex exists, but not widely deployed.
	 */ 
	ifname = argv[i];
	dec_in = fopen(ifname, "rt");
	if (dec_in==NULL) {
		error("Cannot open input file", ifname);
	}

	/* parse the input file */
	if (yyparse()) {
		fclose(dec_in);
		error("Error parsing ", ifname);
	}

	fclose(dec_in);

	if (invert)
		compute_undef(entries);

	/* open output file, if any */
	ofstream dec_out;
	if (cfname) {
		dec_out.open(cfname);
		if (!dec_out.is_open()) {
			error("Cannot open output file", cfname);
		}
	}

	if (theiling == false) {

		DecodeTreeNode dtree(entries);

		if (!dtree.fixEntries() || !dtree.fixEntries() || !dtree.isWellFormed())
			error("Entries not wellformed", "");

		dtree.decode(gamma);
		if (dec_out.is_open()) {
			dtree.emitC(dec_out);
			dec_out.close();
		}
		else dtree.emitC(cout);

		if (xfname) {
			ofstream xml_out(xfname);
			if (!xml_out.is_open()) {
				error("Cannot open output file", xfname);
			}
			dtree.emitXML(xml_out);
			xml_out.close();
		}
	}
	else {

		DecodeTheilingNode dth(entries, n_ones(INSTBITS));

		if (!dth.isWellFormed())
			error("Entries not wellformed", "");

		dth.decode();
		if (dec_out.is_open()) {
			dth.emitC(dec_out);
			dec_out.close();
		}
		else dth.emitC(cout);

		if (xfname) {
			ofstream xml_out(xfname);
			if (!xml_out.is_open()) {
				error("Cannot open output file", xfname);
			}
			dth.emitC(xml_out);
			xml_out.close();
		}
	}

	return 0;
}
