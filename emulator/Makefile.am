
DECODER_GEN := $(top_builddir)/decgen/decgen32
DECODER_HDR := arm_dec.h
DECODER_DEF := arm_inst.def
DECODER_HDR2 := arm_dec2.h
DECODER_DEF2 := arm_inst2.def

$(DECODER_HDR): $(DECODER_DEF)
	$(DECODER_GEN) -i -g 0.25 -o $@ $<

$(DECODER_HDR2): $(DECODER_DEF2)
	$(DECODER_GEN) -i -g 0.25 -o $@ $<


CONFLEX := scan_conf.l
CONFLEX_SRC := scan_conf.cpp

CONFPARSE := parse_conf.y
CONFPARSE_SRC := parse_conf.cpp
CONFPARSE_HDR := parse_conf.hpp

$(CONFPARSE_SRC): $(CONFPARSE)
	@YACC@ -d -p conf_ $^ -o $@

$(CONFLEX_SRC): $(CONFLEX)
	@LEX@ -Pconf_ -o$@ $^


#SUBDIRS = nwfpe old

bin_PROGRAMS = ema 

ema_SOURCES = main.cpp

ema_LDADD = libarmemu.a

arm_iss_sys.o :: arm_iss.cpp
	$(CXXCOMPILE) -DSIMIT_SYSTEM_LEVEL -c $< -o $@

lib_LIBRARIES = libarmemu.a

libarmemu_a_SOURCES = bittypes.h armemul.cpp emumem.cpp read_elf.cpp \
	syscall.cpp loader.h loader.cpp decode.cpp armsupp.cpp \
	armcopro.cpp armcopro.h pxa_io.cpp pxa_io.h\
	debug.cpp util.cpp arm_iss.cpp arm_iss.hpp sa_io.cpp sa_io.h arm_io.h\
	arm_dec.h arm_dec2.h armmmu.h armmmu.cpp tlb.h \
	read_elf.h auto_disasm.h decode.h auto_impl.h syscall.h \
	parse_conf.cpp parse_conf.hpp scan_conf.cpp simconf.cpp simconf.h arch.hpp

libarmemu_a_LIBADD = arm_iss_sys.o


include_HEADERS = bittypes.h armemul.h emumem.h misc.h \
	arch.hpp armmmu.h tlb.h armcopro.h arm_io.h


EXTRA_DIST = $(DECODER_DEF) $(DECODER_DEF2) $(CONFPARSE) $(CONFLEX)

