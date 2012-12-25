/*  armcopro.c -- co-processor interface:  ARM6 Instruction Emulator.
    Copyright (C) 1994, 2000 Advanced RISC Machines Ltd.
 
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
 
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
 
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#include "armcopro.h"
#include "arm_io.h"

using namespace simit;
/* What follows is the Validation Suite Coprocessor.  It uses two
   co-processor numbers (4 and 5) and has the follwing functionality.
   Sixteen registers.  Both co-processor nuimbers can be used in an MCR
   and MRC to access these registers.  CP 4 can LDC and STC to and from
   the registers.  CP 4 and CP 5 CDP 0 will busy wait for the number of
   cycles specified by a CP register.  CP 5 CDP 1 issues a FIQ after a
   number of cycles (specified in a CP register), CDP 2 issues an IRQW
   in the same way, CDP 3 and 4 turn of the FIQ and IRQ source, and CDP 5
   stores a 32 bit time value in a CP register (actually it's the total
   number of N, S, I, C and F cyles).  */



unsigned copro1::MRC (unsigned type, word_t instr, word_t * value)
{
	*value = emu->val_register[BITS(instr, 16, 19)];
	return ARMul_DONE;
}

unsigned copro1::MCR (unsigned type, word_t instr, word_t value)
{
	emu->val_register[BITS(instr, 16, 19)] = value;
	return ARMul_DONE;
}

unsigned copro2::LDC (unsigned type, word_t instr, word_t data)
{
	static unsigned words;

	if (type != ARMul_DATA)
		words = 0;
	else {
		emu->val_register[BITS(instr, 12, 15)] = data;

		if (BITn(instr, 22))
			/* It's a long access, get two words.  */
			if (words++ != 4)
				return ARMul_INC;
	}

	return ARMul_DONE;
}

unsigned copro2::STC (unsigned type, word_t instr, word_t * data)
{
	static unsigned words;

	if (type != ARMul_DATA)
		words = 0;
	else {
		*data = emu->val_register[BITS(instr, 12, 15)];

		if (BITn(instr, 22))
			/* It's a long access, get two words.  */
			if (words++ != 4)
				return ARMul_INC;
	}

	return ARMul_DONE;
}
unsigned copro4::LDC (unsigned type, word_t instr, word_t data)
{
	static unsigned words;

	if (type != ARMul_DATA)
		words = 0;
	else {
		emu->val_register[BITS(instr, 12, 15)] = data;

		if (BITn(instr, 22))
			/* It's a long access, get two words.  */
			if (words++ != 4)
				return ARMul_INC;
	}

	return ARMul_DONE;
}

unsigned copro4::STC (unsigned type, word_t instr, word_t * data)
{
	static unsigned words;

	if (type != ARMul_DATA)
		words = 0;
	else {
		*data = emu->val_register[BITS(instr, 12, 15)];

		if (BITn(instr, 22))
			/* It's a long access, get two words.  */
			if (words++ != 4)
				return ARMul_INC;
	}

	return ARMul_DONE;
}

unsigned copro4::MRC (unsigned type, word_t instr, word_t * value)
{
	*value = emu->val_register[BITS(instr, 16, 19)];
	return ARMul_DONE;
}

unsigned copro4::MCR (unsigned type, word_t instr, word_t value)
{
	emu->val_register[BITS(instr, 16, 19)] = value;
	return ARMul_DONE;
}

unsigned copro4::CDP (unsigned type, word_t instr)
{
	static uint64_t finish = 0;

	if (BITS(instr, 20, 23) != 0)
		return ARMul_CANT;

	if (type == ARMul_FIRST) {
		word_t howlong;

		howlong = emu->val_register[BITS(instr, 0, 3)];

		/* First cycle of a busy wait.  */
		finish = emu->time () + howlong;

		return howlong == 0 ? ARMul_DONE : ARMul_BUSY;
	}
	else if (type == ARMul_BUSY) {
		if (emu->time () >= finish)
			return ARMul_DONE;
		else
			return ARMul_BUSY;
	}

	return ARMul_CANT;
}

unsigned DoAFIQ (arm_emulator *emu)
{//no one turn signal up to high so it will do over again and again....
	fprintf(stderr,"DoAFIQ\n");
	emu->raise_fiq_signal();
	return 0;
}

unsigned DoAIRQ (arm_emulator *emu)
{//no one turn signal up to high so it will do over again and again....
	fprintf(stderr,"DoAIRQ\n");
	emu->raise_irq_signal();
	return 0;
}

unsigned copro5::MRC (unsigned type, word_t instr, word_t * value)
{
	*value = emu->val_register[BITS(instr, 16, 19)];
	return ARMul_DONE;
}

unsigned copro5::MCR (unsigned type, word_t instr, word_t value)
{
	emu->val_register[BITS(instr, 16, 19)] = value;
	return ARMul_DONE;
}

unsigned copro5::CDP (unsigned type, word_t instr)
{
	static uint64_t finish;
	word_t howlong;

	howlong = emu->val_register[BITS(instr, 0, 3)];

	switch ((int) BITS(instr, 20, 23)) {
	case 0:
		if (type == ARMul_FIRST) {
			/* First cycle of a busy wait.  */
			finish = emu->time () + howlong;

			return howlong == 0 ? ARMul_DONE : ARMul_BUSY;
		}
		else if (type == ARMul_BUSY) {
			if (emu->time () >= finish)
				return ARMul_DONE;
			else
				return ARMul_BUSY;
		}
		return ARMul_DONE;

	case 1:
		if (howlong == 0)
			emu->raise_exception(FIQV);
	//	else
	//		emu->ScheduleEvent (howlong, DoAFIQ);
		return ARMul_DONE;

	case 2:
		if (howlong == 0)
			emu->raise_exception(IRQV);
	//	else
	//		emu->ScheduleEvent (howlong, DoAIRQ);
		return ARMul_DONE;

	case 3:
		emu->clear_fiq_signal();
		return ARMul_DONE;

	case 4:
		emu->clear_irq_signal();
		return ARMul_DONE;

	case 5:
		emu->val_register[BITS(instr, 0, 3)] = emu->time ();
		return ARMul_DONE;
	}

	return ARMul_CANT;
}

#if 1
//#ifdef XScale_mach
unsigned copro6::MRC (unsigned type, word_t instr, word_t *data)
{
	unsigned creg = (instr>>16)&15;

	enum
	{
		ICIP = 0x40D00000,	/* Interrupt Controller IRQ Pending Register */
		ICMR = 0x40D00004	/* Interrupt Controller Mask Register */
	};

	switch (creg) {
		case 0:
			emu->io->read_word(ICIP,data); 
			break;
		case 1:
			emu->io->read_word(ICMR,data);
			break;
		default:
			//fprintf (stderr,"cp6_mrc unknown cp6 regs!!!!!!\n");
			*data = 0;
			break;
	}
	return ARMul_DONE;
}

unsigned copro6::MCR (unsigned type, word_t instr, word_t data)
{
	unsigned creg = (instr>>16)&15;

	enum
	{
		ICMR = 0x40D00004,	/* Interrupt Controller Mask Register */
		ICLR = 0x40D00008	/* Interrupt Controller Level Register */
	};

	if (instr & 0x00e000ef == 0)
	switch (creg) {
		case 1:
			last_creg = creg;
			break;
		case 2:
			last_creg = creg;
			break;
		case 7:
			if(last_creg == 1)
				emu->io->write_word(ICMR, data); 
			last_creg = creg;
			break;
		case 8:
			if(last_creg == 2)
				emu->io->write_word(ICLR, data); 
			last_creg = creg;
			break;
		default:
			//fprintf (stderr,"cp6_mrc unknown cp6 regs!!!!!!\n");
			break;
	}

	return ARMul_DONE;
}

unsigned copro14::MRC (unsigned type, word_t instr, word_t *data)
{
	unsigned creg = (instr>>16)&15;

	switch (creg) {
		case 6:
			*data = pxa_cp14_regs.cclkcfg;
			break;
		case 7:
			*data = pxa_cp14_regs.pwrmode;
			break;
        default:
			*data = 0;
			fprintf (stderr,"cp14_mrc unknown cp14 regs!!!!!!\n");
			break;
	}
	return ARMul_DONE;
}

unsigned copro14::MCR (unsigned type, word_t instr, word_t data)
{
	unsigned creg = (instr>>16)&15;

	switch (creg) {
		case 6:
			pxa_cp14_regs.cclkcfg = data & 0xf;
			break;
		case 7:
			pxa_cp14_regs.pwrmode = data & 0x3;
			break;
		default:
			fprintf (stderr,"cp14_mcr unknown cp14 regs!!!!!!\n");
			break;
	}
	return ARMul_DONE;
}

#endif
