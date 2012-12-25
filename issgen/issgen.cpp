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

#include "issgen.hpp"

#include <fstream>
#include <list>
#include <vector>
#include <cassert>
#include <iomanip>
#include <cstring>

using namespace issgen;
using std::string;
using std::ifstream;
using std::ofstream;
using std::vector;
using std::pair;
using std::map;
using std::list;

// global constant symbols
Symbol sym_condition;
Symbol sym_vtarget;
Symbol sym_ctarget;
Symbol sym_execute;
Symbol sym_iswrite;
Symbol sym_pc;
Symbol sym_inst;
Symbol sym_root;

extern isa_prog *isa_parse_wrapper(ifstream *infile, const char *fname);



/* bincode related methods */

bool bin_ident::type_check(const FIELD_ENV *fenv, const VAR_ENV *venv,
	OP_ENV *oenv, Symbol opname, unsigned off)
{
	offset = off;
	if (fenv->find(data)!=fenv->end()) {
		type  = BIN_FIELD;
		width = (*fenv->find(data)).second;
		return true;
	}
	else if (oenv->find(data)!=oenv->end()) {
		type  = BIN_OPER;
		oper = (*oenv->find(data)).second;
		if (oper->type_check(fenv, venv, oenv)) {
			width = oper->get_width();
			count *= oper->get_count();
			return true;
		}
		return false;
	}
	fprintf(stderr, "Unknown field %s in operation %s\n",
		symbol_get_string(data).c_str(),
		symbol_get_string(opname).c_str());
	return false;
}

string bin_ident::get_opcode(unsigned index) const
{
	assert(index<count);

	if (type==BIN_FIELD) return std::string(width, '-');
	return oper->get_opcode(index);
		
}

string bin_ident::get_qualified_name(unsigned index) const
{
	assert(index<count);

	if (type==BIN_FIELD) return std::string();
	return oper->get_qualified_name(index);
}

void bin_ident::get_used_vars(unsigned index, VAR_ENV *vtab) const
{
	assert(index<count);

	if (type==BIN_OPER)
		oper->get_used_vars(index, vtab);
}

bin_literal::bin_literal(const char *str) 
{
	unsigned ii, len;
	len = strlen(str);
	for (ii=0; ii<len; ii++) {
		if (str[ii]=='0' || str[ii]=='1' || str[ii]=='-') {
			data.push_back(str[ii]);
		}
	}
	width = data.length();
	count = 1;
}


/* oper methods */

bool oper::type_check(const FIELD_ENV *fenv, const VAR_ENV *venv, OP_ENV *oenv)
{
	if (checked) 
		return true;

	if (incheck) {
		fprintf(stderr, "Cyclic reference detected involving op %s.\n",
			symbol_get_string(name).c_str());
		return false;
	}

	incheck = true;

	// check the opcode patterns first
	unsigned offset = 0;
	count = 1;

	// check the opcode
	vector<class bin_code *>::reverse_iterator rbit;
	for (rbit=pat->rbegin(); rbit!=pat->rend(); rbit++) {

		if (!(*rbit)->type_check(fenv, venv, oenv, name, offset))
			return false;

		/* if this is not literal, enter into table */
		if ((*rbit)->is_identifier())
			field_table[(*rbit)->get_name()] = 
				dynamic_cast<class bin_ident *>(*rbit);

		offset += (*rbit)->get_width();
		count *= (*rbit)->get_count();
	}

	width = offset;

	/* enter PC into the table */
	field_table[sym_pc] = NULL;
	field_table[sym_inst] = NULL;

	// then need to check the contents
	OP_CON_ENV::iterator cit;
	for (cit=contents->begin(); cit!=contents->end(); cit++) {

		// check if the name of the content is recognized
		if ((*cit).first==sym_condition || (*cit).first==sym_vtarget ||
			(*cit).first==sym_ctarget || (*cit).first==sym_execute ||
			(*cit).first==sym_iswrite) {

			if (!check_content((*cit).first, venv, oenv))
				return false;
		}
		else {
			fprintf(stderr, "Unknown attribute %s in operation %s\n",
				symbol_get_string((*cit).first).c_str(),
				symbol_get_string(name).c_str());
		}
	}

	checked = true;
	incheck = false;

	return true;
}

static bool str_contain_word(const char *str, const char *needle)
{
	const char *ptr = str;
	int len = strlen(needle);

	while (*ptr) {

		char *match = strstr(ptr, needle);
		if (match==NULL) break;

		/* we check what is before the bword */
		if (ptr<match) {
			/* alphabet, number, _ means this is a subword, no good */
			if (isalnum(*(match-1)) || *(match-1)=='_') {
				ptr = match+len;
				continue;
			}
		}

		/* we then check what is after the word */
		/* alphabet, number, _ means this is a subword, no good */
		if (isalnum(match[len]) || match[len]=='_')
			ptr = match+len;
		else
			/* good, find one */
			return true;
	}
	return false;
}

bool oper::check_content(Symbol nam, const VAR_ENV *venv, OP_ENV *oenv)
{
	assert(contents->find(nam)!=contents->end());
	char *str = (*(contents->find(nam))).second;

	/* we first check which variables are used in this content */
	VAR_ENV::const_iterator vit;
	for (vit=venv->begin(); vit!=venv->end(); vit++) {

		string vname = symbol_get_string((*vit).first);
		if (str_contain_word(str, vname.c_str())) {
			var_table.insert(*vit);
		}
	}

	int  left = 0, right = 0;	//index for scanning
	bool in_escape = false;		//escape mode starts with '\' 
	bool in_field = false;		//field is surrounded by '$'

	vector<class token> *tokens = new vector<class token>;

	// we parse into the string
	while (str[right]) {

		if (in_escape) {
			in_escape = false;
		}
		else if (str[right]=='\\') {
			in_escape = true;
		}
		else if (str[right]=='$') {

			if (in_field) {

				in_field = false;
				str[right] = 0;

				// from left to right, we have a field
				tokens->push_back(token(str+left, token::FIELD));
				left = right+1;
			}
			else {
				in_field = true;
				str[right] = 0;

				// encounter a new field
				tokens->push_back(token(str+left, token::CHARS));
				left = right+1;
			}
		}
		right++;
	}

	if (in_field) {
		fprintf(stderr, "Unterminated field reference in oper %s\n",
			symbol_get_string(name).c_str());	
		delete tokens;
		return false;
	}

	if (left<right)
		tokens->push_back(token(str+left, token::CHARS));

	// now we check all the fields
	vector<class token>::iterator tit;
	for (tit=tokens->begin(); tit!=tokens->end(); tit++) {

		if ((*tit).get_type()==token::FIELD) {

			Symbol fname = symbol_find_or_insert((*tit).get_data());
			if (field_table.find(fname)==field_table.end()) {
				fprintf(stderr, "Undefined field %s in oper %s\n",
					(*tit).get_data(),
					symbol_get_string(name).c_str());	
				delete tokens;
				return false;
			}
		}
	}

	cont_table[nam] = tokens;

	return true;
}


oper::~oper()
{
	map<Symbol, char *>::iterator ait;
	for(ait=contents->begin(); ait!=contents->end(); ait++) {
		free((*ait).second);
	}
	delete contents;
	
	vector<class bin_code *>::iterator bit;
	for (bit=pat->begin(); bit!=pat->end(); bit++) {
		delete *bit;
	}

	map<Symbol, vector<class token> *>::iterator cit;
	for (cit=cont_table.begin(); cit!=cont_table.end(); cit++) {
		delete (*cit).second;
	}

	delete pat;
}

string oper::get_opcode(unsigned index) const
{
	/* safety checking */
	assert(index<count);

	string ret;

	vector<class bin_code *>::reverse_iterator rbit;
	for (rbit=pat->rbegin(); rbit!=pat->rend(); rbit++) {
		ret = (*rbit)->get_opcode(index % (*rbit)->get_count()) + ret;
		index = index/(*rbit)->get_count();
	}

	return ret;
}

string oper::get_qualified_name(unsigned index) const
{
	/* safety checking */
	assert(index<count);

	string ret(symbol_get_string(name)+'_');
	vector<class bin_code *>::reverse_iterator rbit;
	for (rbit=pat->rbegin(); rbit!=pat->rend(); rbit++) {
		if ((*rbit)->is_identifier()) {
			class bin_ident *bident = dynamic_cast<class bin_ident *>(*rbit);
			ret += bident->get_qualified_name(index % bident->get_count());
			index = index/bident->get_count();
		}
	}
	return ret;	
}

void oper::get_used_vars(unsigned index, VAR_ENV *vtab) const
{
	/* safety checking */
	assert(index<count);

	vector<class bin_code *>::reverse_iterator rbit;
	for (rbit=pat->rbegin(); rbit!=pat->rend(); rbit++) {
		if ((*rbit)->is_identifier()) {
			class bin_ident *bident = dynamic_cast<class bin_ident *>(*rbit);
			bident->get_used_vars(index % bident->get_count(), vtab);
			index = index/bident->get_count();
		}
	}

	vtab->insert(var_table.begin(), var_table.end());
}

/* group methods */

group::~group()
{
	vector<class oper_base *>::iterator oit;
	for (oit=opers->begin(); oit!=opers->end(); oit++) {
		delete *oit;
	}
	delete opers;
}


bool group::type_check(const FIELD_ENV *fenv, const VAR_ENV *venv, OP_ENV *oenv)
{
	if (incheck) {
		fprintf(stderr, "Cyclic reference detected involving group %s.\n",
			symbol_get_string(name).c_str());
		return false;
	}

	incheck = true;
	count = 0;

	/* first do recursive checking */
	vector<class oper_base *>::iterator op_it;
	for (op_it=opers->begin(); op_it!=opers->end(); op_it++) {
		if (!(*op_it)->type_check(fenv, venv, oenv)) return false;
		count += (*op_it)->get_count();
	}

	/* second check width */
	if (opers->size()) {
		width = opers->front()->get_width();
		for (op_it=opers->begin(); op_it!=opers->end(); op_it++) {
			if ((*op_it)->get_width()!=width) {
				fprintf(stderr, "%d %d Width mismatch for %s in group %s.\n",
					(*op_it)->get_width(), width,
					symbol_get_string((*op_it)->get_name()).c_str(), 
					symbol_get_string(name).c_str());
				return false;
			}
		}
	}
	else width = 0;

	checked = true;
	incheck = false;
	return true;
}

string group::get_opcode(unsigned index) const
{
	assert(index<count);

	vector<class oper_base *>::const_iterator op_it;
	for (op_it=opers->begin(); op_it!=opers->end(); op_it++) {
		if (index<(*op_it)->get_count())
			return (*op_it)->get_opcode(index);
		index -= (*op_it)->get_count();
	}
	assert(0); 			// should not reach here
	return string();	// shut up gcc
}


string group::get_qualified_name(unsigned index) const
{
	assert(index<count);

	vector<class oper_base *>::const_iterator op_it;
	for (op_it=opers->begin(); op_it!=opers->end(); op_it++) {
		if (index<(*op_it)->get_count())
			return (*op_it)->get_qualified_name(index);
		index -= (*op_it)->get_count();
	}
	assert(0); 			// should not reach here
	return string();	// shut up gcc
}

void group::get_used_vars(unsigned index, VAR_ENV *vtab) const
{
	assert(index<count);

	vector<class oper_base *>::const_iterator op_it;
	for (op_it=opers->begin(); op_it!=opers->end(); op_it++) {
		if (index<(*op_it)->get_count())
			return (*op_it)->get_used_vars(index, vtab);
		index -= (*op_it)->get_count();
	}
	assert(0); 			// should not reach here
}

/* isa_prog methods */

bool isa_prog::type_check()
{
	/* Maintaining a list for recursion */
	list<class oper_base *> op_list;

	/* Push every toplevel opers into the list */
	op_list.insert(op_list.end(), ops->begin(), ops->end());

	/* need to build up oenv */
	while (!op_list.empty()) {

		class oper_base *op_tmp = op_list.front();
		op_list.pop_front();

		if (oenv->find(op_tmp->get_name())!=oenv->end()) {
			fprintf(stderr, "Redefinition of op/group %s\n",
				symbol_get_string(op_tmp->get_name()).c_str());
			return false;
		}

		oenv->insert(OP_T(op_tmp->get_name(), op_tmp));

		if (op_tmp->is_group()) {

			class group *grp_tmp = dynamic_cast<class group *>(op_tmp);
			vector<class oper_base *> *nlist = grp_tmp->get_opers();
			op_list.insert(op_list.end(), nlist->begin(), nlist->end());
		}
	}

	/* now with oenv, we can type check */
	OP_ENV::iterator op_it;
	for (op_it = oenv->begin(); op_it!=oenv->end(); op_it++) {
		if (!(*op_it).second->type_check(fenv, venv, oenv)) return false;
	}

	/* now find the group named root */
	if (oenv->find(sym_root)!=oenv->end()) {
		grp_root = (*oenv->find(sym_root)).second;
		if (grp_root->get_width()!=32) {
			fprintf(stderr, "Root's encoding width is not 32\n");
			return false;
		}

		fprintf(stderr, "Total instruction count = %d\n",
			grp_root->get_count());
	}
	else {
		fprintf(stderr, "Cannot find root\n");
		return false;
	}

	return true;
}

isa_prog::~isa_prog() 
{
	delete fenv;
	delete venv;
	delete oenv;

	vector<class oper_base *>::iterator oit;
	for (oit=ops->begin(); oit!=ops->end(); oit++) {
		delete *oit;
	}
	delete ops;
}

static unsigned get_sign(string& code)
{
	unsigned ind, val;
	for (ind=0,val=0; ind<code.length(); ind++) {
		val = val<<1;	
		if (code[ind]=='1') val = val|1;
	}
	return val;
}

static unsigned get_mask(string& code)
{
	unsigned ind, val;
	for (ind=0,val=0; ind<code.length(); ind++) {
		val = val<<1;	
		if (code[ind]!='-') val = val|1;
	}
	return val;
}

void isa_prog::emit_decoder(ofstream& ofile)
{
	ofile << std::hex;
	ofile << std::setfill('0');
	ofile << std::uppercase;

	unsigned count = grp_root->get_count();
	for (unsigned ii=0; ii<count; ii++) {
		string opcode = grp_root->get_opcode(ii);
		ofile << "IDEF(" << grp_root->get_qualified_name(ii)
			<< ", 0x" << std::setw(8) << get_mask(opcode)
			<< ", 0x" << std::setw(8) << get_sign(opcode) << ", 1)\n";
	}
}

void bin_ident::emit_declaration(unsigned index,
	unsigned rmb, ofstream& ofile)
{
	if (type==BIN_FIELD) {
		ofile << "\tunsigned " << symbol_get_string(data)
			<< " = " << "(inst>>" << offset+rmb
			<< ")&" << (1<<width)-1 << ";\n";
	}
	else {
		oper->emit_field_declarations(index, offset+rmb, ofile);
	}
}


void bin_ident::emit_interpreter_content(unsigned index,
	Symbol attr, ofstream& ofile)
{
	assert(index<count);
	if (type==BIN_FIELD) {
		ofile << symbol_get_string(data);
	}
	else {
		oper->emit_interpreter_content(index, attr, ofile);
	}
}

void bin_ident::emit_codegenerator_content1(unsigned index,
	Symbol attr, ofstream& ofile)
{
	assert(index<count);
	if (type==BIN_FIELD) {
		ofile << "%u";
	}
	else {
		oper->emit_codegenerator_content1(index, attr, ofile);
	}
}

void bin_ident::emit_codegenerator_content2(unsigned index, unsigned rmb,
	Symbol attr, ofstream& ofile)
{
	assert(index<count);
	if (type==BIN_FIELD) {
		ofile << ", (inst>>" << offset+rmb
			<< ")&" << (1<<width)-1;
	}
	else {
		oper->emit_codegenerator_content2(index, offset+rmb, attr, ofile);
	}
}

void bin_ident::emit_codegenerator_content3(unsigned index, unsigned rmb,
	Symbol attr, ofstream& ofile)
{
	assert(index<count);
	if (type==BIN_FIELD) {
		ofile << "((inst>>" << offset+rmb
			<< ")&" << (1<<width)-1 <<")";
	}
	else {
		oper->emit_codegenerator_content3(index, offset+rmb, attr, ofile);
	}
}


unsigned oper::get_field_index(Symbol field, unsigned index) const
{
	vector<class bin_code *>::reverse_iterator rbit;
	for (rbit=pat->rbegin(); rbit!=pat->rend(); rbit++) {
		if ((*rbit)->is_identifier()) {
			if ((*rbit)->get_name()==field)
				return index % (*rbit)->get_count();
			index = index/(*rbit)->get_count();
		}
	}

	assert(0);	// should not be here
	return 0;	// shut up gcc
}

void oper::emit_interpreter_content(unsigned index,
	Symbol attr, ofstream& ofile)
{
	assert(index<count);
	assert(cont_table.find(attr)!=cont_table.end());

	vector<class token> *tokens = cont_table[attr];

	vector<class token>::iterator tit;
	for (tit=tokens->begin(); tit!=tokens->end(); tit++) {
		/* encountered a field */
		if ((*tit).get_type()==token::FIELD) {
			/* get the name of the field */
			Symbol fname = symbol_find_or_insert((*tit).get_data());
			/* must exist in field table after type_check */
			assert(field_table.find(fname)!=field_table.end());

			/* if this is program counter */
			if (fname==sym_pc) {
				ofile << "GET_PC()";
			}
			else if (fname==sym_inst) {
				ofile << "inst";
			}
			/* else emit its content */
			else
				field_table[fname]->emit_interpreter_content
					(get_field_index(fname, index), attr, ofile);
		}
		else {
			ofile << (*tit).get_data();
		}
	}
}

void group::emit_interpreter_content(unsigned index,
	Symbol attr, ofstream& ofile)
{
	assert(index<count);

	vector<class oper_base *>::const_iterator op_it;
	for (op_it=opers->begin(); op_it!=opers->end(); op_it++) {
		if (index<(*op_it)->get_count())
			return (*op_it)->emit_interpreter_content(index, attr, ofile);
		index -= (*op_it)->get_count();
	}

	assert(0);
}


void group::emit_interpreter(unsigned index, ofstream& ofile)
{
	assert(index<count);

	vector<class oper_base *>::const_iterator op_it;
	for (op_it=opers->begin(); op_it!=opers->end(); op_it++) {
		if (index<(*op_it)->get_count())
			return (*op_it)->emit_interpreter(index, ofile);
		index -= (*op_it)->get_count();
	}

	assert(0);
}


void group::emit_field_declarations(unsigned index,
	unsigned rmb, ofstream& ofile)
{
	assert(index<count);

	vector<class oper_base *>::const_iterator op_it;
	for (op_it=opers->begin(); op_it!=opers->end(); op_it++) {
		if (index<(*op_it)->get_count())
			return (*op_it)->emit_field_declarations(index, rmb, ofile);
		index -= (*op_it)->get_count();
	}

	assert(0);
}

void group::emit_codegenerator(unsigned index, ofstream& ofile)
{
	assert(index<count);

	vector<class oper_base *>::const_iterator op_it;
	for (op_it=opers->begin(); op_it!=opers->end(); op_it++) {
		if (index<(*op_it)->get_count()) {
			(*op_it)->emit_codegenerator(index, ofile);
			return;
		}
		index -= (*op_it)->get_count();
	}

	assert(0);
}

void group::emit_codegenerator_content1(unsigned index,
	Symbol attr, ofstream& ofile)
{
	assert(index<count);

	vector<class oper_base *>::const_iterator op_it;
	for (op_it=opers->begin(); op_it!=opers->end(); op_it++) {
		if (index<(*op_it)->get_count()) {
			(*op_it)->emit_codegenerator_content1(index, attr, ofile);
			return;
		}
		index -= (*op_it)->get_count();
	}

	assert(0);
}

void group::emit_codegenerator_content2(unsigned index, unsigned rmb,
	Symbol attr, ofstream& ofile)
{
	assert(index<count);

	vector<class oper_base *>::const_iterator op_it;
	for (op_it=opers->begin(); op_it!=opers->end(); op_it++) {
		if (index<(*op_it)->get_count()) {
			(*op_it)->emit_codegenerator_content2(index, rmb, attr, ofile);
			return;
		}
		index -= (*op_it)->get_count();
	}

	assert(0);
}

void group::emit_codegenerator_content3(unsigned index, unsigned rmb,
	Symbol attr, ofstream& ofile)
{
	assert(index<count);

	vector<class oper_base *>::const_iterator op_it;
	for (op_it=opers->begin(); op_it!=opers->end(); op_it++) {
		if (index<(*op_it)->get_count()) {
			(*op_it)->emit_codegenerator_content3(index, rmb, attr, ofile);
			return;
		}
		index -= (*op_it)->get_count();
	}

	assert(0);
}

void oper::emit_field_declarations(unsigned index,
	unsigned rmb, ofstream& ofile)
{
	assert(index<count);

	vector<class bin_code *>::reverse_iterator rbit;
	for (rbit=pat->rbegin(); rbit!=pat->rend(); rbit++) {
		if ((*rbit)->is_identifier()) {
			dynamic_cast<class bin_ident *>(*rbit)->emit_declaration
				(index%(*rbit)->get_count(), rmb, ofile);
			index = index/(*rbit)->get_count();
		}
	}
}

void oper::emit_interpreter(unsigned index, ofstream& ofile)
{
	// safety checking
	assert(index<count);

	// field variable declaration and initialization
	emit_field_declarations(index, 0, ofile);
	ofile << "\n";

	// emit all the vars used
	VAR_ENV vtable;
	get_used_vars(index, &vtable);

	VAR_ENV::iterator vit;
	for (vit=vtable.begin(); vit!=vtable.end(); vit++) {
		ofile << "\t" << symbol_get_string((*vit).second)
			<< " " << symbol_get_string((*vit).first)
			<< ";\n";
	}
	ofile << "\n";

	// emit execution blocks
	if (cont_table.find(sym_execute)!=cont_table.end()) {
		emit_interpreter_content(index, sym_execute, ofile);
		ofile << "\n";
	}

	if (cont_table.find(sym_condition)!=cont_table.end()) {
		ofile << "\tif (";
		emit_interpreter_content(index, sym_condition, ofile);
		ofile << ") {\n";
	}

	if (cont_table.find(sym_ctarget)!=cont_table.end()) {
		ofile << "\t\tSET_PC(";
		emit_interpreter_content(index, sym_ctarget, ofile);
		ofile << ");\n";
		//ofile << "\t\tSET_DELAY();\n"; //delay slot
	}
	else if (cont_table.find(sym_vtarget)!=cont_table.end()) {
		ofile << "\t\tSET_PC(";
		emit_interpreter_content(index, sym_vtarget, ofile);
		ofile << ");\n";
		//ofile << "\t\tSET_DELAY();\n"; //delay slot
	}

	if (cont_table.find(sym_condition)!=cont_table.end()) {
		ofile << "\t}\n";
	}

}

void oper::emit_codegenerator_content1(unsigned index,
	Symbol attr, ofstream& ofile)
{
	assert(index<count);
	assert(cont_table.find(attr)!=cont_table.end());

	vector<class token> *tokens = cont_table[attr];

	vector<class token>::iterator tit;
	for (tit=tokens->begin(); tit!=tokens->end(); tit++) {
		/* encountered a field */
		if ((*tit).get_type()==token::FIELD) {
			/* get the name of the field */
			Symbol fname = symbol_find_or_insert((*tit).get_data());
			/* must exist in field table after type_check */
			assert(field_table.find(fname)!=field_table.end());

			/* if this is program counter */
		//	if (fname==sym_pc || fname==sym_inst) {
		//		ofile << "%uU";
		//	}
			if (fname==sym_pc ) {
				ofile << "%s";
			}
			else if (fname==sym_inst) {
				ofile << "%uU";
			}
			else
				field_table[fname]->emit_codegenerator_content1
					(get_field_index(fname, index), attr, ofile);
		}
		else {
			const char *data = (*tit).get_data();
			int len = strlen(data);
			for (int i=0; i<len; i++)
				if (data[i]=='\n')
					ofile << "\\n\"\n\t\"";
				else
					ofile << data[i];
		}
	}
}

void oper::emit_codegenerator_content2(unsigned index, unsigned rmb,
	Symbol attr, ofstream& ofile)
{
	assert(index<count);
	assert(cont_table.find(attr)!=cont_table.end());

	vector<class token> *tokens = cont_table[attr];

	vector<class token>::iterator tit;
	for (tit=tokens->begin(); tit!=tokens->end(); tit++) {
		/* encountered a field */
		if ((*tit).get_type()==token::FIELD) {
			/* get the name of the field */
			Symbol fname = symbol_find_or_insert((*tit).get_data());
			/* must exist in field table after type_check */
			assert(field_table.find(fname)!=field_table.end());

	//		if (fname==sym_pc)
	//			ofile << ", CURPC";
	//		else if (fname==sym_inst)
	//			ofile << ", inst";
			if (fname==sym_pc)
				ofile << ", \"PC\"";
			else if (fname==sym_inst)
				ofile << ", inst";
			else
				field_table[fname]->emit_codegenerator_content2
					(get_field_index(fname, index), rmb, attr, ofile);
		}
	}
}

void oper::emit_codegenerator_content3(unsigned index, unsigned rmb,
	Symbol attr, ofstream& ofile)
{
	assert(index<count);
	assert(cont_table.find(attr)!=cont_table.end());

	vector<class token> *tokens = cont_table[attr];

	vector<class token>::iterator tit;
	for (tit=tokens->begin(); tit!=tokens->end(); tit++) {
		/* encountered a field */
		if ((*tit).get_type()==token::FIELD) {
			/* get the name of the field */
			Symbol fname = symbol_find_or_insert((*tit).get_data());
			/* must exist in field table after type_check */
			assert(field_table.find(fname)!=field_table.end());

		//	if (fname==sym_pc)
		//		ofile << "CURPC";
		//	else if (fname==sym_inst)
		//		ofile << ", inst";
			if (fname==sym_pc)
				ofile << "CURPC";
			else if (fname==sym_inst)
				ofile << ", inst";
			else
				field_table[fname]->emit_codegenerator_content3
					(get_field_index(fname, index), rmb, attr, ofile);
		}
		else {
			ofile << (*tit).get_data();
		}
	}
}

void oper::emit_codegenerator(unsigned index, ofstream& ofile)
{
//	ofile << "\ttarget_inst_t inst = emu->mem->read_word_fast(curpc);\n";
//	ofile << "\ttarget_inst_t inst = emu->fetch_inst(curpc);\n";
//	ofile << "\tbuf += sprintf(buf,\"fprintf(stderr,\\\"%xX\\\\t\\\");\\n\",inst);\n";
//	ofile << "\tbuf += sprintf(buf,\"fprintf(stderr,\\\"%xX \\\\n\\\");\\n\",curpc);\n";
//	ofile << "\tbuf += sprintf(buf,\"CHECK_RETURN;\\n\");\n";
//	ofile << "\tbuf += sprintf(buf,\"IO_DO_CYCLE;\\n\");\n";
		
	// emit all the vars used
	VAR_ENV vtable;
	get_used_vars(index, &vtable);

	VAR_ENV::iterator vit;
	for (vit=vtable.begin(); vit!=vtable.end(); vit++) {
		ofile << "\tbuf += sprintf(buf, \"\\t"
			<< symbol_get_string((*vit).second)
			<< " " << symbol_get_string((*vit).first)
			<< ";\\n\");\n";
	}
	ofile << "\n";

	// emit execution blocks
	if (cont_table.find(sym_execute)!=cont_table.end()) {
		ofile << "\tbuf += sprintf(buf, \"";
		emit_codegenerator_content1(index, sym_execute, ofile);
		ofile << "\"";
		emit_codegenerator_content2(index, 0, sym_execute, ofile);
		ofile << ");\n";
	}

	// emit conditionl execution block
	if (cont_table.find(sym_condition)!=cont_table.end()) {
		ofile << "\tbuf += sprintf(buf, \"\\tif (";
		emit_codegenerator_content1(index, sym_condition, ofile);
		ofile << ") {\\n\"";
		emit_codegenerator_content2(index, 0, sym_condition, ofile);
		ofile << ");\n";
	}

	if (cont_table.find(sym_ctarget)!=cont_table.end()) {

		// delay slot, define the macro to NULL if the processor has not
		ofile << "\tEMIT_DELAY_SLOT;\n";

		ofile << "\ttarget_addr_t jpc = ";
		emit_codegenerator_content3(index, 0, sym_ctarget, ofile);
		ofile << ";\n";
		ofile << "\tif (jpc >= size)\n";
		ofile << "\t\tbuf += sprintf(buf, "
				 "\"\\tLONG_JUMP(%uU, %uU);\\n\", "
				 "curpc, jpc);\n";
		ofile << "\telse\n";
		ofile << "\t\tbuf += sprintf(buf, \"\\tJUMP(%uU, %uU, "
				 "L%x);\\n\", "
				 "curpc, jpc, jpc);\n";
	}
	else if (cont_table.find(sym_vtarget)!=cont_table.end()) {

		assert(0); //not used for ARM

		// delay slot, define the macro to NULL if the processor has not
		ofile << "\tEMIT_DELAY_SLOT;\n";

		ofile << "\tbuf += sprintf(buf, \"\\tjpc = ";
		emit_codegenerator_content1(index, sym_vtarget, ofile);
		ofile << ";\\n\"";
		emit_codegenerator_content2(index, 0, sym_vtarget, ofile);
		ofile << ");\n";
		ofile << "\tbuf += sprintf(buf, "
				 "\"\\tif (jpc<CISS_FSTART || jpc>=CISS_FEND ) {\\n\");\n";
		ofile << "\tbuf += sprintf(buf, "
				 "\"\\t\\tLONG_JUMP2(%uU, jpc);\\n\\t}\\n\", curpc);\n";
		ofile << "\tbuf += sprintf(buf, "
				 "\"\\telse\\n\");\n";
		ofile << "\tbuf += sprintf(buf, "
				 "\"\\t\\tJUMP(%uU, jpc, *labels[(jpc-CISS_FSTART)/4]);\\n\","
				 " curpc);\n";
	}

	if (cont_table.find(sym_condition)!=cont_table.end()) {
		ofile << "\tbuf += sprintf(buf, \"\\t}\\n\");\n";
	}

	
	ofile << "\treturn buf;\n";


}


void isa_prog::emit_interpreter_dec(ofstream& ofile)
{
	unsigned count = grp_root->get_count();
	for (unsigned ii=0; ii<count; ii++) {
		ofile << "void SIMIT_IMP(" << grp_root->get_qualified_name(ii)
			<< ") (emulator_t *emu, target_inst_t inst);\n";
	}

}

void isa_prog::emit_interpreter(ofstream& ofile)
{
	ofile << 
"#include \"arch.hpp\"\n";

	unsigned count = grp_root->get_count();
	for (unsigned ii=0; ii<count; ii++) {
		ofile << "void SIMIT_IMP(" << grp_root->get_qualified_name(ii)
			<< ") (emulator_t *emu, target_inst_t inst)\n{\n\n";
		grp_root->emit_interpreter(ii, ofile);
		ofile << "\n}\n\n";
	}
}

void isa_prog::emit_codegenerator(ofstream& ofile)
{
	ofile << 
"#include \"arch_gen.hpp\"\n";

	unsigned count = grp_root->get_count();
	for (unsigned ii=0; ii<count; ii++) {
		ofile << "char *emitc_" << grp_root->get_qualified_name(ii)
			<< "(char *buf, unsigned curpc, "
			<< "target_inst_t inst, unsigned size)\n"
			<< "{\n";
		grp_root->emit_codegenerator(ii, ofile);
		ofile << "\n}\n\n";
	}

}

void isa_prog::emit_codegenerator_dec(ofstream& ofile)
{
	unsigned count = grp_root->get_count();
	for (unsigned ii=0; ii<count; ii++) {
		ofile << "char *emitc_" << grp_root->get_qualified_name(ii)
			<< "(char *, unsigned, target_inst_t, unsigned);\n";
	}

}

void usage(char *cname) {
	fprintf(stderr, "Usage: %s [-p <prefix>] <isa file>\n", cname);
}


int main(int argc, char *argv[])
{
	char *bin_name;
	char *prog_name = NULL;
	char *prefix = NULL;

	/* initialize symboles */
	sym_condition = symbol_find_or_insert("condition");
	sym_vtarget = symbol_find_or_insert("vtarget");
	sym_ctarget = symbol_find_or_insert("ctarget");
	sym_execute = symbol_find_or_insert("execute");
	sym_iswrite = symbol_find_or_insert("iswrite");
	sym_root = symbol_find_or_insert("root");

	// implicit variable
	sym_pc = symbol_find_or_insert("pc");
	sym_inst = symbol_find_or_insert("inst");

	for(int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-p") == 0) prefix = argv[++i]; else
        if (!prog_name)
        {
            prog_name = argv[i];
            break;
        }
    }

    if ((bin_name = strrchr(argv[0], '/'))!=NULL) bin_name++;
    else bin_name = argv[0];

	if (!prog_name) {
		usage(bin_name);
		exit(0);
	}

	if (!prefix) {
		prefix = prog_name;
	}

	ifstream infile;
	infile.open(prog_name);

	if (!infile.is_open()) {
		fprintf(stderr, "Cannot open file %s\n", argv[1]);
		exit(1);
	}

	isa_prog *isa = isa_parse_wrapper(&infile, argv[1]);
	infile.close();

	if (!isa->type_check()) {
		fprintf(stderr, "Fix above error first\n");
	}
	else {

		char *fname1 = new char[strlen(prefix)+10];
		char *fname2 = new char[strlen(prefix)+10];

		strcpy(fname1, prefix);
		strcat(fname1, "_inst.def");

		ofstream decfile;
		decfile.open(fname1);
		isa->emit_decoder(decfile);
		decfile.close();

		strcpy(fname1, prefix);
		strcat(fname1, "_iss.hpp");

		ofstream isshdrfile;
		isshdrfile.open(fname1);
		isa->emit_interpreter_dec(isshdrfile);
		isshdrfile.close();

		strcpy(fname2, prefix);
		strcat(fname2, "_iss.cpp");

		ofstream issfile;
		issfile.open(fname2);
		isa->emit_interpreter(issfile);
		issfile.close();

		strcpy(fname1, prefix);
		strcat(fname1, "_gen.hpp");

		ofstream genhdrfile;
		genhdrfile.open(fname1);
		isa->emit_codegenerator_dec(genhdrfile);
		genhdrfile.close();

		strcpy(fname2, prefix);
		strcat(fname2, "_gen.cpp");

		ofstream genfile;
		genfile.open(fname2);
		isa->emit_codegenerator(genfile);
		genfile.close();

		delete [] fname1;
		delete [] fname2;
	}

	delete isa;
}
