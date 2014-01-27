# SimIt-ARM

## About

SimIt-ARM 3.0 currently contains a functional simulator for the
ARM v5 architecture. It supports both user-level simulation and
system-level simulation.

## Installation

To install, clone git repository

    git clone https://github.com/medvid/SimIt-ARM.git
    cd SimIt-ARM

Then, create build directory and run CMake from here:

    mkdir -p build && cd build
    cmake ..
    make
    make install

CMake supports following configuration options:

* ENABLE_DEVEL - enable development features
* ENABLE_DECGEN - build decoder generator
* ENABLE_ISSGEN - build instruction set generator
* ENABLE_JIT - enable dynamic-compiled simulation engine

The `<prefix>/bin` directory will contain 1 file:

* ema - ARM instruction set simulator

## Usage

You can test the simulator by doing
the following:

    cd test
    <prefix>/bin/ema wc ../README.md

`wc` is the UNIX utility word count. The above command interprets the
cross-compiled `wc` program to count the words in this file.
You can also test system-level simulation by doing the following:

    <prefix>/bin/ema -s sa1100.cfg

The above program will boot the `vmlinux` image, a Linux 2.4.19 kernel
configured for the SA1100 processor. The configuration file `sa1100.cfg`
specifies some architecture information of the system to simulate.
The file and the related binary images can be downloaded from Simit-ARM
website.

## Development

### TODO

Short-term list:

* ARMv5 (LDC2, STC2, MCR2, MRC2, CDP2, BKPT, PLD), Thumb, XSCALE extensions
* LCD, Touch-screen, NIC, Flash, ...
* GDB remote debugger interface
* Support for 64-bit host platforms.

Long-term list:

* Class restructuring for better OOP?
* Cycle accurate simulator from arm.isa?
* Direct translation (like QEMU) based on arm.isa?

### Changelog

**armsim-0.1** 07-02-2002, initial release.

**armsim-0.2** 07-15-2002, improved release, 50% faster than 0.1.

Major changes:

* Bug fixed on swap,swapbyte (when src and dst reg be the same)
* Exploit instruction memory accessing locality to save pc page table lookups.
* Exploit data access locality (pc based, stack pointer based) to save page table lookup.
* Use table lookup to replace switch/case in predicate testing.
* Specializes some memory access operation implementations to save execution time.
* Bug fixed on carry bit updating of adc, sbc and rsc.

**SimIt-ARM-1.0b** 10-6-2002, a major release including a cycle-accurate simulator.

* Enhanced portability. Installation tested on x86-linux, sun-solaris, alpha and Cygwin.
* Added mmap system call interpretation, shortens program execution trace.

**SimIt-ARM-1.0** 4-6-2003, minor chages from 1.0b.

* Used automatic synthesized decoder, correctness now ensured.
* Added argument "-m" so that user can control the length of simulation.
* Small fix on fstat64 system call interpretation for Linux compatibility.
* Small fix on cache.h to suppress g++3.2 complaints.

**SimIt-ARM-1.1a** 4-29-2003, alpha release of modularized emulator and simulator.

* Bug fixes on shifter operand of dpi operations.
* Emulator source code becomes CPP code, 5-10% slow down for emulator.
* The emulator and simulator are modularized, i.e. multiple instances
  can be created without interfering each other. Very useful for
  multi-processor platform simulation.
* Emulator and simulator now can be reused, i.e. they can load a program
* and run, then dump the program and reload another to run.

**SimIt-ARM-1.1b** 7-10-2003, bug fix release

* Bug fixes on half-word/signed byte load/store.
* Bug fix on mla_s's erroneous zero flag setting.
* Bug fixes in ema_count:  forgot to count fpe and syscall.
* Buf fixes in syscall.cpp:read/write, size_t --> ssize_t.
* Bug fixes in emumem.cpp:get_page, C++ array constructor does not do
  initialization, nor type checking on initializer.
* Inlined some memory functions to boost ISS speed by about 10%.

**SimIt-ARM-1.1** 9-1-2003, release

* Various small bug fixes with the help of Valgrind.

**SimIt-ARM-2.0** 1-8-2004, release

* The cycle accurate simulator is mostly synthesized from MADL.
* Cycle accurate simulator is 2x faster than the one in 1.x release.
* Many bugs in release 1.x fixed, improved timing accuracy.
* Flexible stub provided for external device models.
  To learn how to use the stubs, see file dev_test.hpp.
* Simplified configuration.

**SimIt-ARM-2.0.1** 2-10-2004, bug fix release

* Store_ext_reg instruction's address computation is fixed.
* Load_ext_imm instruction's sign extension is fixed.
* One less cycle for coprocessor instruction execution.

**SimIt-ARM-2.0.2** 5-17-2004, bug fix release with improvements

* Smull and umull encoding fixed.
* Fix building out-of-source problem. (Thanks to Artur Klauser)
* Speed up of emulator by hacking PC and CPSR. (Thanks to Artur Klauser)
* Included decgen for user to add new instructions.
* MMAP based speedup. (Thanks to Artur Klauser)

**SimIt-ARM-2.0.3** 9-15-2004, cache model fix

* Fixed some problems in data cache, adjusted cache parameters, improves
  accuracy.

**SimIt-ARM-2.0.4** 12-9-2004, C++ syntax fix

* Fixed some problems exposed by gcc3.4.
* Chris Han contributed to implement some system call routines.

**SimIt-ARM-2.1** 2-2-2005, Bug fix, portability fix, added singleton

* Fixed sima's bug in sign-extending for some load instructions.
* Added emu_single directory, generates an non-modularized emulator,
  about 5-6% faster than emu.
* Fixed one portability problem in libosm, _S and _L are used as macros
  in c library. Never use these names
* Fixed one portability problem in decgen/decode.h. label_t is used
  somewhere in the library. Change that to dg_label_t.

**SimIt-ARM-2.1.1** 5-21-2005, cleanup not really to be released

* Removed emu_single, reverted related changes in version 2.1.
  Too complicated and not worthwhile the slight performance gain.
* Improved memory emulator, added access permission control.
* Remved the original external memory interface since the new memory
  emulator has integrated similar function in a better way.
* Fixed some bugs in libosm/mpz_base.?pp. The bugs were irrelevant
  to SimIt-ARM since this part of the library was not used in SimIt.

**SimIt-ARM-3.0** 6-25-2007, supports system-level simulation

* Supports system level simulation.
* Supports compiled simulation (ema_jit).
* Improved the interface of the builtin debugger.


### Authors

* Wei Qin (weiqin@gmail.com)
* Joseph D'Errico (jderrico@gmail.com)
* Apisate Thammanomai (apisate@yahoo.com)


### Contributors

* Artur Klauser improved the memory emulator in release 2.0.2.
* John Beniston and Stephen Li reported several emulator bugs.


### Acknowledgements

* SimIt-ARM uses the elf loader and the system call interpreter
  from Microlib.org by Gilles Mouchard.
* SimIt-ARM borrows some system-level emulation code from Skyeye/Armulator.
* SimIt-ARM uses some ideas from QEMU to improve performance.
