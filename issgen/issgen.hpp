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

#include <map>
#include <vector>
#include <string>
#include <cassert>

namespace issgen {

/* environment for fields */
typedef std::pair<Symbol, unsigned> FIELD_T;
typedef std::map<Symbol, unsigned> FIELD_ENV;

/* environment for vars */
typedef std::pair<Symbol, Symbol> VAR_T;
typedef std::map<Symbol, Symbol> VAR_ENV;

/* environment for oper/groups */
typedef std::pair<Symbol, class oper_base *> OP_T;
typedef std::map<Symbol, class oper_base *> OP_ENV;

/* fields */
typedef std::pair<Symbol, char *> OP_CON_T;
typedef std::map<Symbol, char *> OP_CON_ENV;


/* forward declaration */
class oper_base;

/* binary code */
class bin_code {

  public:

	bin_code() : offset(0), count(1), width(0) {}

	virtual ~bin_code() {}

	virtual std::string get_opcode(unsigned index) const = 0;

	virtual bool type_check(const FIELD_ENV *fenv, const VAR_ENV *venv,
		 OP_ENV *oenv, Symbol opname, unsigned offset) = 0;

	virtual Symbol get_name() const = 0;

	bool is_identifier() const {return get_name()!=SymNULL;}

	/* get the number of possible encodings, can call after type_check */
	unsigned get_count() const {return count;}

	/* get the position of the RMB, can call after type_check */
	unsigned get_offset() const {return offset;}

	/* get the width of the encoding, can call after type_check */
	unsigned get_width() const {return width;}

  protected:

	unsigned offset;
	unsigned count;
	unsigned width;
};


class bin_ident : public bin_code {

  public:

	bin_ident(const Symbol& data) : data(data) {}

	~bin_ident() {}

	std::string get_opcode(unsigned index) const;
	std::string get_qualified_name(unsigned index) const;
	void get_used_vars(unsigned index, VAR_ENV *vtab) const;

	bool type_check(const FIELD_ENV *fenv, const VAR_ENV *venv,
		OP_ENV *oenv, Symbol opname, unsigned offset);

	Symbol get_name() const {return data;}

	void emit_declaration(unsigned index,
		unsigned rmb, std::ofstream& ofile);
	void emit_interpreter_content(unsigned index,
		Symbol attr, std::ofstream& ofile);

	void emit_codegenerator_content1(unsigned index,
		Symbol attr, std::ofstream& ofile);
	void emit_codegenerator_content2(unsigned index, unsigned rmb,
		Symbol attr, std::ofstream&);
	void emit_codegenerator_content3(unsigned index, unsigned rmb,
		Symbol attr, std::ofstream&);


  private:

	Symbol data;

	enum {
		BIN_FIELD,
		BIN_OPER,
	} type;
	class oper_base *oper; //useful only when type is BIN_OPER
};


class bin_literal : public bin_code {

  public:

	bin_literal(const char *str);

	~bin_literal() {}

	std::string get_opcode(unsigned index) const {
		assert(index==0);
		return data;
	}

	bool type_check(const FIELD_ENV *fenv, const VAR_ENV *venv,
		OP_ENV *oenv, Symbol opname, unsigned off) {
		offset = off;
		return true;
	}

	Symbol get_name() const {return SymNULL;}

  private:

	std::string data;
};


class token {

  public:

	typedef enum {
		FIELD,
		CHARS
	} TOKEN_T;

	token(const char *str, TOKEN_T type) : data(str), type(type) {}

	~token() {};	

	const char *get_data() const {return data;}
	TOKEN_T get_type() const {return type;}

  private:
	const char *data;
	TOKEN_T type;

};


class oper_base {

  public:
	// constructor
	oper_base(const Symbol& name) :
			name(name), checked(false), incheck(false), width(0) {}

	// destructor
	virtual ~oper_base() {}

	// get the name of an operation/group
	Symbol get_name() const {return name;}

	//typecheck given the environment
	virtual bool type_check(const FIELD_ENV *fenv, const VAR_ENV *venv,
		OP_ENV *oenv) = 0;

	// get the opdeocde string, in 0, 1 and -'s.
	virtual std::string get_opcode(unsigned index) const = 0;

	// get the qualified name
	virtual std::string get_qualified_name(unsigned index) const = 0;

	// get used variables
	virtual void get_used_vars(unsigned index, VAR_ENV *vtab) const = 0;

	// get opcode width, valid after type_check is called
	unsigned get_width() const {
		assert(checked);
		return width;
	}

	// get the number of expansions, valid after type_check is called
	unsigned get_count() const {
		assert(checked);
		return count;
	}

	// get the actual type of the oper
	virtual bool is_oper() const  = 0;
	virtual bool is_group() const = 0;

	// emit an interpretation routine for the function
	virtual void emit_interpreter(unsigned index, std::ofstream&) = 0;
	virtual void emit_field_declarations(unsigned index,
		unsigned rmb, std::ofstream& ofile) = 0;
	virtual void emit_interpreter_content(unsigned index, Symbol attr,
		std::ofstream&) = 0;

	// emit runtime code generation routine
	virtual void emit_codegenerator(unsigned index, std::ofstream&) = 0;
	virtual void emit_codegenerator_content1(unsigned index, Symbol attr,
		std::ofstream&) = 0;
	virtual void emit_codegenerator_content2(unsigned index, unsigned rmb,
		Symbol attr, std::ofstream&) = 0;
	virtual void emit_codegenerator_content3(unsigned index, unsigned rmb,
		Symbol attr, std::ofstream&) = 0;

  protected:

	Symbol name;
	bool checked;	// type-check has been called
	bool incheck;	// being type-checked, to detect cycles
	unsigned width;	// width of encoding
	unsigned count;	// number of options 

};


class oper : public oper_base {

  public:
 
	// constructor
	oper(const Symbol& name, std::vector<class bin_code *> *pat, 
		OP_CON_ENV *contents) :
		oper_base(name), pat(pat), contents(contents) {}

	// destructor
	~oper();

	// typecheck given the variable names
	bool type_check(const FIELD_ENV *fenv, const VAR_ENV *venv,
		OP_ENV *oenv);

	// get the actual type of the oper
	bool is_oper() const  {return true;}
	bool is_group() const {return false;}

	// get the opdeocde string, in 0, 1 and -'s.
	std::string get_opcode(unsigned index) const;

	// get the qualified name
	std::string get_qualified_name(unsigned index) const;

	// get used variables
	void get_used_vars(unsigned index, VAR_ENV *vtab) const;

	// emit an interpretation routine for the function
	void emit_interpreter(unsigned index, std::ofstream&);
	void emit_field_declarations(unsigned index,
		unsigned rmb, std::ofstream& ofile);
	void emit_interpreter_content(unsigned index, Symbol attr,
		std::ofstream&);

	// emit runtime code generation routine
	void emit_codegenerator(unsigned index, std::ofstream&);
	void emit_codegenerator_content1(unsigned index, Symbol attr,
		std::ofstream&);
	void emit_codegenerator_content2(unsigned index, unsigned rmb,
		Symbol attr, std::ofstream&);
	void emit_codegenerator_content3(unsigned index, unsigned rmb,
		Symbol attr, std::ofstream&);

  private:

	std::vector<class bin_code *> *pat;
	OP_CON_ENV *contents;

	bool check_content(Symbol, const VAR_ENV *venv, OP_ENV *oenv);

	std::map<Symbol, class bin_ident *> field_table;
	VAR_ENV var_table;	// vars used in this oper, type

	// name -> vector<token> *
	std::map<Symbol, std::vector<class token> *> cont_table;

	unsigned get_field_index(Symbol field, unsigned index) const;

};


class group : public oper_base {

  public:

	// constructor
	group(const Symbol& name, std::vector<class oper_base *> *ops) :
			oper_base(name), opers(ops) {}

	// destructor
	~group();

	// get the opdeocde string, in 0, 1 and -'s.
	std::string get_opcode(unsigned index) const;

	// get the qualified name
	std::string get_qualified_name(unsigned index) const;

	// get used variables
	void get_used_vars(unsigned index, VAR_ENV *vtab) const;

	// get the actual type of the oper
	bool is_oper() const  {return false;}
	bool is_group() const {return true;}

	// typecheck given the variable names
	bool type_check(const FIELD_ENV *fenv, const VAR_ENV *venv,
		OP_ENV *oenv);

	// return all sub-oper/groups
	std::vector<class oper_base *> *get_opers() {return opers;}

	// emit an interpretation routine for the function
	void emit_interpreter(unsigned index, std::ofstream&);
	void emit_field_declarations(unsigned index,
		unsigned rmb, std::ofstream& ofile);
	void emit_interpreter_content(unsigned index, Symbol attr,
		std::ofstream&);

	void emit_codegenerator(unsigned index, std::ofstream&);
	void emit_codegenerator_content1(unsigned index, Symbol attr,
		std::ofstream&);
	void emit_codegenerator_content2(unsigned index, unsigned rmb,
		Symbol attr, std::ofstream&);
	void emit_codegenerator_content3(unsigned index, unsigned rmb,
		Symbol attr, std::ofstream&);

  private:

	std::vector<class oper_base *> *opers;

};


class isa_prog {

  public:

    isa_prog(FIELD_ENV *fenv, VAR_ENV *venv,
		std::vector<class oper_base *> *ops) :
			fenv(fenv), venv(venv), oenv(new OP_ENV), ops(ops) {}

	bool type_check();

	// emit file for decoder generation
	void emit_decoder(std::ofstream& ofs);

	// emit interpretation routines for all instructions
	void emit_interpreter(std::ofstream&);
	void emit_interpreter_dec(std::ofstream&);

	// emit runtime code generation routine
	void emit_codegenerator(std::ofstream&);
	void emit_codegenerator_dec(std::ofstream&);

	
	~isa_prog();

  private:

	FIELD_ENV *fenv;
	VAR_ENV	  *venv;
	OP_ENV    *oenv;

	std::vector<class oper_base *> *ops;

	// the special toplevel group
	class oper_base *grp_root;

};

}
