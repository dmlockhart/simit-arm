
if ENABLE_JIT
JIT = jit
else
JIT =
endif

if ENABLE_DEV
DEV = decgen issgen
else
DEV =
endif

SUBDIRS = emulator $(JIT) $(DEV)

EXTRA_DIST = test/grep test/wc issgen/arm.isa TODO

dist-hook:
	$(top_srcdir)/prepend_license $(top_srcdir)/license_notice $(distdir) $(top_srcdir)/filelist
	touch $(distdir)/issgen/parse_isa.cpp $(distdir)/issgen/scan_isa.cpp $(distdir)/issgen/parse_isa.hpp
	touch $(distdir)/decgen/scan_idef.c $(distdir)/decgen/parse_idef.cpp $(distdir)/decgen/parse_idef.h
	touch $(distdir)/emulator/scan_conf.cpp $(distdir)/emulator/parse_conf.cpp $(distdir)/emulator/parse_conf.hpp

include_HEADERS = config.h
