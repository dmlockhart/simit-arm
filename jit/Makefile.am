
bin_PROGRAMS = ema_jit decomp_server  #decomp


#decomp builds the static compiled simulator
# decomp_SOURCES = decomp.cpp decomp.hpp decomp_main.cpp auto_emitc.h

# decomp_LDADD = libdynarmemu.a $(top_builddir)/emulator/libarmemu.a
# decomp_LDFLAGS = -ldl


#ema_jit is the dynamic compiled simulator
ema_jit_SOURCES = dyn_main.cpp dyn_arm_emul.cpp

ema_jit_LDADD = libdynarmemu.a $(top_builddir)/emulator/libarmemu.a
ema_jit_LDFLAGS = -export-dynamic -ldl -lpthread



#server to assist compiled simulation 
decomp_server_SOURCES = decomp_server_main.cpp decomp_server.cpp \
	decomp_server.hpp

decomp_server_LDADD = libdynarmemu.a $(top_builddir)/emulator/libarmemu.a
decomp_server_LDFLAGS = -ldl



INCLUDES = -I$(top_srcdir)/emulator 
include_HEADERS = arch_jit.hpp dyn_arm_emul.hpp build_dll.hpp

lib_LIBRARIES = libdynarmemu.a

libdynarmemu_a_SOURCES = dyn_arm_emul.cpp arm_gen.cpp \
		dyn_arm_emul.hpp arm_gen.hpp arm_iss_jit.cpp auto_emitc.h arch_gen.hpp

EXTRA_DIST = build_dll.hpp.in
#	decomp_src/compiled_main.cpp decomp_src/compiled_run.cpp \
#	decomp_src/compiled_run.hpp decomp_src/Makefile

install-data-hook:
	\rm -rf ~/.ema/*

