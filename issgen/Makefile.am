
noinst_PROGRAMS = issgen
issgen_SOURCES = symbol.cpp symbol.hpp issgen.hpp issgen.cpp parse_isa.cpp scan_isa.cpp parse_isa.hpp

parse_isa.cpp : parse_isa.y
	@YACC@ -d -p isa_ $^ -o $@

scan_isa.cpp : scan_isa.l
	@LEX@ -Pisa_ -o$@ $^

deploy: arm.isa issgen
	./issgen -p arm arm.isa
	cp arm_iss.cpp $(top_srcdir)/emulator
	cp arm_iss.hpp $(top_srcdir)/emulator
	cp arm_inst.def $(top_srcdir)/emulator
	cp arm_gen.cpp $(top_srcdir)/jit
	cp arm_gen.hpp $(top_srcdir)/jit

EXTRA_DIST = parse_isa.y scan_isa.l
