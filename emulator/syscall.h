/***************************************************************************
                          syscall.h  -  description
                             -------------------
    begin                : Wed Sep 26 2001
    copyright            : (C) 2001 CEA and Université Paris XI Orsay
    author               : Gilles Mouchard
    email                : gilles.mouchard@lri.fr, gilles.mouchard@cea.fr
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef SIMIT_SYSCALL_H
#define SIMIT_SYSCALL_H


#ifdef COMPILE_SIMULATOR
namespace simulator {
  class arm_simulator;
}
typedef simulator::arm_simulator armulator;
#else 
namespace simit {
  class arm_emulator;
}
typedef simit::arm_emulator armulator;
#endif

extern void sc_impl(armulator *emu, arm_inst_t inst);
extern char *sc_disasm(arm_inst_t inst, target_addr_t pc, char *buf);
extern void do_syscall(armulator *emu, arm_inst_t inst);

#define impl_sc sc_impl
#define disasm_sc sc_disasm
#define incr_sc INST_SYSCALL

#endif
