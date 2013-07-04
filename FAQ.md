## FAQ

### What host platforms does SimIt-ARM-3.0 support?

SimIt-ARM was developed on IA32 Linux platform.
Though not fully tested, system level simulation may also work on
other major platforms, including 64-bit IA32e Linux platforms.
User-level simulation may work on other 32-bit POSIX compliant platforms.

### What target processors does SimIt-ARM-3.0 support?

Current SimIt-ARM supports StrongARM 1100, PXA 25x, and PXA27x.
Other ARM processors may also work but haven't been tested.

### How can I terminate system-level simulation without using "kill -9"?

The easy approach to terminate normally is to use `kill` without `-9`.
The simulator will exit smoothly. Also, if your busybox enables poweroff or reboot,
you can simply run `poweroff` or `reboot` in your simulation shell.
The simulation will stop gracefully.
Note the Colibri image in the release supports only `reboot`.

### How can I modify the provided ramdisk and add my own programs?

First you need to have root privilege.
Suppose your uncompressed ramdisk is named `initrd8M.img`.
You can do the following to mount it under Linux.

    mkdir mnt
    sudo mount -o loop initrd8M.img mnt/

Now the `mnt` directory has all the content of the ramdisk.
You can copy new files into it, create new symbolic links or device nodes,
and delete files. After you are done, exit the director and do the following.

    sudo umount mnt/
    touch initrd8M.img

Now you have your modified `initrd8M.img`.
The last `touch` command may not be necessary.
However, it is safe to use it just to remind the OS that the file has been changed.

### I encountered a "Warning: Unimplemented instruction ..." message. What does it mean?

This most probably occurs when you are doing user-level simulation
and your ARM program contains floating-point emulation (FPE) instructions.
They are not real ARM instructions.
Instead, they are coprocessor instructions meant to trigger OS faults.
OS will then call the Netwinder FPE library to interpret them.
It is long known that the FPE solution is inefficient and is gradually
superseded by improved solutions such as soft-float compilation,
or floating-point ISA extensions.
Therefore, SimIt-ARM-3.0 stops providing support to such instructions at the user-level.
However, if you are performing system-level simulation, the OS under simulation will
take care of FPE instructions if you enabled its NWFPE feature when configuring the kernel.
So you will not see this message during user-level simulation.

### Are Thumb instructions supported?

No, they are not supported. It is not in our plan yet.

### Why does't my XScale program work?

If your program is compiled by a GCC configured for XScale,
it may contain special wireless MMX instructions of XScale.
These instructions are not yet supported by SimIt-ARM.

### Why doesn't ethernet work in the Linux under simulation?

For ethernet to work, an ethernet interface card model must be built into the processor model.
It is on the TODO list.
Similarly, device models for LCD, touch screen, flash memory are all on the list.

### I want to learn more about embedded Linux. Where can I start?

It takes a lot of experience.
Reading books, Linux documentation and source code will help.
Below are some books that may be useful.
They are ordered by difficulty level, starting from the easiest.

* Building Embedded Linux Systems, by Karim Yaghmour;
* Linux Kernel Development, by Robert Love;
* Linux Device Drivers, 2nd or 3rd edition,
  by Jonathan Corbet, Alessandro Rubini, and Greg Kroah-Hartman;

It is also necessary to do some small projects to sharpen your skills,
either on a simulator or on real hardware platforms.
The [Gumstix](http://www.gumstix.com) boards are good choices
since they are affordable and well-supported.
