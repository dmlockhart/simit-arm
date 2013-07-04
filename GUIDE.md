# Users' Guide

* [Installation](#installation)
* [Quick Start](#quick-start)
  * [User-level simulation](#user-level-simulation)
  * [System-level simulation](#system-level-simulation)
* [Interpreter](#interpreter)
  * [Commandline Options](#commandline-options)
  * [Built-in Debugger](#built-in-debugger)
* [Dynamic compiled simulation](#dynamic-compiled-simulation)
* [Related Work](#related-work)
* [Reference](#reference)


## Installation

To install, first untar the source by

    tar xzvf SimIt-ARM-3.0.tar.gz

Then build the source code by

    cd SimIt-ARM-3.0
    ./configure
    make
    make install

After these steps, the ./build/bin directory contains the following programs:

* ema - An ARM interpreter.

To test the installation was successful type

    ./build/bin/ema test/wc configure

The above command simulates the UNIX utility wc.
If the simulator is installed correctly, you should see the line count, word
count, and character count of the file configure.

If one prefers other installation directory than the default build, one can
specify the prefix configuration flag.

    ./configure --prefix=<PREFIX path>

For convenience of referencing the simulator commands, one can include the
`bin/` directory into the environment variable `PATH` by typing in bash

    export PATH=$PATH:<PREFIX PATH>/bin

or in csh

    setenv PATH ${PATH}:<PREFIX PATH>/bin


By default, the dynamic-compiled simulation engine is not built.
To enable it, one can run the configuration script with the `--enable-jit` option.

    ./configure --enable-jit

After `make install`, the `./build/bin` directory contains two additional programs:

* `ema_jit` - An ARM dynamic-compiled simulator.
* `decomp_server` - Translation server program.

The dynamic-compiled simulator runs significantly faster than the interpreter
for long simulations. However, for short simulations or for programs with poor
locality, the speed may sometimes be slower due to the translation overhead.

## Quick Start

### User-level simulation

SimIt-ARM reads ELF32 little-endian ARM binaries.
The binaries must be statically linked.
See the [crosstool-NG page](http://crosstool-ng.org/) for more information
on building a soft-float enabled cross ARM compiler.

This section uses a simple C program to demonstrate the essential steps
involved in using the simulator. Below shows the example program:

    #include <stdio.h>
    int main()
    {
      printf("Hello World!\n");
      return 0;
    }

Now compile the program using the ELF32 ARM cross-compiler.
Remember to statically link the binary. The command should be something like:

    arm-softfloat-linux-gnu-gcc -static -o hello_world hello_world.c

The compiler generates the `hello_world` ARM binary.
Assuming that the `<PREFIX>/bin` directory has been added to the environment
variable `PATH`, one can simulate the binary by typing

    ema hello_world

The interpreter ema will output "Hello World!" in the standard output.
Usage of the interpreter is described in more detail in
Section [Interpreter](#interpreter).
Note if you used a regular arm-linux gcc for compilation, ema may report a couple
of error messages about unimplemented instructions.
You can safely ignore those message if the program does not involve any
floating-point computation.

Lastly, if you turned on `--enable-jit` during configuration, the
dynamic-compiled simulator can be invoked by

    ema_jit hello_world

In this example, the dynamic-compiled simulator interprets the whole program
since `hello_world` is too short to trigger run-time compilation.
A full description of the dynamic-compiled simulator is provided
in Section [Dynamic compiled simulation](#dynamic-compiled-simulation).

### System-level simulation

SimIt-ARM can boot ARM linux kernels by using `-s` option. Both `ema` and `ema_jit`
support the option.
You can download some prebuilt kernel images and ramdisks from
[here](http://sourceforge.net/projects/simit-arm/files/simit-arm/).

To try ARM linux kernel, first untar the file by

    tar xjf linux_images.tar.bz2

Then start simulation by either

    cd sa1100
    ema -s sa1100.cfg

or

    ema_jit -s sa1100.cfg

In this example,the program will boot the `vmlinux` image,
a Linux 2.4.19 kernel configured for the SA1100 processor.
The configuration file `sa1100.cfg` specifies some architecture information of the system,
and instructions to initialize the memory and registers.

## Interpreter

### Commandline Options

The default syntax for running the interpreter is:

* For user-level

    ./ema <program name> <program arguments>

* For system-level

    ./ema -s <config file>

By default, the interpreter will print out running time information and instruction counts.
If one wants SimIt-ARM to print out error messages and system call information,
run it in verbose mode by typing

* For user-level

    ./ema -v <program name> <program arguments>

* For system-level

    ./ema -v -s <config file>

Often simulation can take a very long period of time.
To avoid waiting indefinitely, one can truncate simulation by setting
an upper bound of the number of instructions to simulate.

* For user-level

    ./ema -m [million instructions] <program name> <program arguments>

* For system-level

    ./ema -m [million instructions] -s <config file>


### Built-in Debugger

SimIt-ARM also has a light-weight built-in debugger to trace the target programs.
To start SimIt-ARM in debugging mode type:

* For user-level

    ./ema -d <program name>

* For system-level

    ./ema -d -s <config file>

The following instructions are available at the debugging prompt:

| Command  | Description |
|----------|---------------------------------------------|
| h        | print help message
| u [addr] | disassemble 10 instructions from `[addr]`
             addr can be either a hex address or `pc`,
             which will disassemble from the current program counter
| d [addr] | dump 256 bytes from memory address `addr`
| g [addr] | run until `pc = addr`
| s [num]  | run until `pc = pc + num*4`
| t [num]  | step `num` instruction(s)
| r[n]     | dump register value(s)
| cpsr     | dump `cpsr` value
| spsr     | dump `spsr` value
| c        | dump instruction counters
| cc       | reset instruction counters
| q        | quit

We again use the program `hello_world` as an example.
It is often useful to create a dump of the target program before debugging.
The ARM cross objdump can be used to create the dump.
It is normally built together with the cross-compiler.

    arm-softfloat-linux-gnu-objdump -d hello_world >hello_world.dump

Now start the debugger with the command:

    ./ema -d hello_world

One should now see the debugger prompt `>`.
At the prompt, one can type any of the commands above.
We assume that, from our dump file, we learn that the main function starts
at address `0x004003e0`.
We can instruct the debugger to run until the main function by typing:

    >g 4003e0

At this point, if we want to see the first 10 instructions in main, we type:

    >u pc

Suppose we want to monitor certain memory accesses and we notice
that the 5th instruction is a memory store.

    0x004003f0 : 0xafbc0010    sw $gp, 16($sp)

We can step through the first 4 instructions with:

    >t 4

To inspect the contents of the registers, we can simply type:

    >r

From the screen printout, we can learn value of the stack
pointer, `$sp`, which is used in the sw instruction.
Assume its value is `0xbfffbda8`.
We can observe the memory contents from this address by typing:

    >d bfffbda8

The command will display 256 bytes of memory.
So we can find the contents of memory that the program is accessing.
If we decide that the contents of memory is incorrect, we may want
to go back and check previous memory accesses.
We can now terminate the debugger by typing:

    >q

and restart it to trace back to the first memory access.
Obviously there are more complicated debugging scenarios than this.
Hopefully the above example will get you started.

## Dynamic compiled simulation

The dynamic-compiled simulator combines the functionality of the interpreter
and the static-compiled simulator.
It interprets the target program at the beginning.
Meanwhile it profiles the program by keeping a counter for each block of code.
The counter records the cumulative number of simulated instructions in the page.
If the number is beyond a predefined threshold, then this page is deemed hot and is translated.
Translation is performed in two steps.
First the page is translated to a C++ function.
Next the C++ function is compiled into a shared library and linked to the simulator itself.
Compilation of the C++ function can be done by the simulator process itself, or can be
distributed to a separate thread, or a remote translation server.
Obviously, using of more CPUs or translation servers means faster simulation speed.

The dynamic-compiled simulator can be invoked by the command `ema_jit`.
The command has the following format:

    ema_jit [-h] [-v] [-m num] [-f fname] [-j num] [-s config] <program> <program arguments>

The meaning of the command line arguments are explained below:

| Argument       | Description |
|----------------|----------------------------------------------------------|
| -h             | To print the usage information and exit.
| -v             | To turn on the verbose mode.
| -m num         | Million instructions to simulate.
| -j num         | To specify additional translation threads to create
| -f <file name> | To specify translation server configuration file
| -s config      | For system-level simulation, path to configuration file
| file name      | ARM ELF32 program to simulate
| args           | arguments to the program, ignored when -s is given

Translation of code blocks into dynamic libraries is performed by GCC.
Due to the slow speed of GCC in compiling C++ code, for small programs
it may take `ema_jit` longer to simulate than `ema`.
But for large programs, the translation overhead will be sufficiently
absorbed and `ema_jit` will outperform ema.

The dynamic-compiled simulator will create a folder `$HOME/.ema` to cache translation results.
If a program has been simulated once, subsequent simulation may be much faster
because shared libraries in the cache can be directly loaded on simulation start.

To reduce the overhead of translation, `ema_jit` is capable of distributing
translation tasks to other local CPUs, or remote workstations.
If are you have a multi-processor computer, you can use the `-j` option
to specify the number of additional threads to perform translation.
If you want to use additional workstations, you can use the `-f` option.
The file specified by the option is simple text file.
Each line specifies an IP address, and a port number.
`ema_jit` will establish communication with the servers via Linux socket.
Below is the content of an example server configuration file.

    192.168.1.28 12345
    128.168.1.29 12345

On each of the remote translation server, `SimIt-ARM-3.0` must be fully installed.
The program `decomp_server` must be started to receive translation tasks.
The `-p` option of the program specifies the port number for communication.
For example, for the above server configuration file to work,
one should run on both servers with IP addresses of 192.168.1.28 and 192.168.1.29
the following prior to the start of `ema_jit`.

    decomp_server -p 12345

The server configuration file for the `-s` switch is also a text file.
The `colibri.cfg` file in the pre-built `linux_images.tar.bz2`
is a complete example of its format.

One need to disable the firewalls for socket communication to work.
More technical details of the dynamic-compiled simulator
have been published in some conference papers.
See [References](#reference) for information.


## Related Work

* [QEMU](http://www.qemu.org) includes a very fast ARM emulator.
  SimIt-ARM borrowed some ideas on memory emulation from QEMU.
  It is possible to strip some features of `ema_jit` (e.g. instruction counting)
  so that it runs at a similar speed as QEMU.

* [Skyeye](http://www.skyeye.org) is a continuation of the GDB/Armulator project.
  It simulates many devices. SimIt-ARM borrowed some code from Skyeye.

* [Crosstool-NG](http://crosstool-ng.org) is what you need to build cross compilers.

## Reference

* W. Qin, J. D'Errico, X. Zhu,
  A Multiprocessing Approach to Accelerate Retargetable and Portable
  Dynamic-compiled Instruction-set Simulation,
  International Conference on Hardware/Software Codesign and System Synthesis, November 2006

* J. D'Errico, W. Qin,
  Constructing Portable Compiled Instruction-set Simulators — An ADL-driven Approach,
  IEEE/ACM Design Automation and Test in Europe, March 2006.

* Apisate Thammanomai,
  Engineering a System-level Instruction-set Simulator,
  MS Thesis, Boston University, College of Engineering, May 2007.

If you have further questions, please check out the FAQ page before contacting the authors.
The authors can be reached at weiqin@gmail.com or apisate@gmail.com.
