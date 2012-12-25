#include <csignal>
#include <cstdio>
#include <cassert>
#include <cstdlib>
#include "armemul.h"
#include "armmmu.h"

using namespace simit;

/* Given a processor mode, this routine returns the
   register bank that will be accessed in that mode.  */

reg_bank_t arm_emulator::mode_to_bank(cpu_mode_t mode) const
{
	static reg_bank_t bankofmode[] = {
		USR_BANK, FIQ_BANK, IRQ_BANK, SVC_BANK,
		DUM_BANK, DUM_BANK, DUM_BANK, DUM_BANK,
		DUM_BANK, DUM_BANK, DUM_BANK, DUM_BANK,
		DUM_BANK, DUM_BANK, DUM_BANK, DUM_BANK,
		USR_BANK, FIQ_BANK, IRQ_BANK, SVC_BANK,
		DUM_BANK, DUM_BANK, DUM_BANK, ABT_BANK,
		DUM_BANK, DUM_BANK, DUM_BANK, UND_BANK,
		DUM_BANK, DUM_BANK, DUM_BANK, SYS_BANK
	};

	if ((unsigned)mode >= (sizeof (bankofmode) / sizeof (reg_bank_t)))
		return DUM_BANK;

	return bankofmode[mode];
}


void arm_emulator::raise_exception(ex_vector_t vector)
{
	word_t temp;
	const int isize = 0;
	const int esize = 4;
	const int e2size = 4;


	temp = read_gpr(PC_REAL_IND);

	switch (vector) {
	case ResetV:	/* RESET */
     	bank = mode_to_bank(SVC_MODE);
		write_spsr (read_cpsr());
		write_cpsr (((read_cpsr() & ~(mode | (1<<5))) | (3<<6) | SVC_MODE));
      	write_gpr2(LRIND, 0);
		status=ST_EXIT;
		break;
	case UndefinedInstrV:	/* Undefined Instruction */
     	bank = mode_to_bank(UND_MODE);
		write_spsr (read_cpsr());
		write_cpsr (((read_cpsr() & ~(mode | (1<<5))) | (1<<7) | UND_MODE));	
      	write_gpr2(LRIND, temp + isize);
		break;
	case SWIV:	/* Software Interrupt */
     	bank = mode_to_bank(SVC_MODE);
		write_spsr (read_cpsr());
		write_cpsr (((read_cpsr() & ~(mode | (1<<5))) | (1<<7) | SVC_MODE));
      	write_gpr2(LRIND, temp + isize);
		break;
	case PrefetchAbortV:	/* Prefetch Abort */
     	abort_addr = 1;
		bank = mode_to_bank(ABT_MODE);
		write_spsr (read_cpsr());
		write_cpsr (((read_cpsr() & ~(mode | (1<<5))) | (1<<7) | ABT_MODE));
      	write_gpr2( 14, temp + isize);
		break;
	case DataAbortV:	/* Data Abort */
     	bank = mode_to_bank(ABT_MODE);
		write_spsr (read_cpsr());
		write_cpsr (((read_cpsr() & ~(mode | (1<<5))) | (1<<7) | ABT_MODE));
      	write_gpr2(LRIND, temp + e2size);
		break;
	case IRQV:	/* IRQ */
		bank = mode_to_bank(IRQ_MODE);
		write_spsr (read_cpsr());
		write_cpsr (((read_cpsr() & ~(mode | (1<<5))) | (1<<7) | IRQ_MODE));
      	write_gpr2(LRIND, temp + esize);
		break;
	case FIQV:	/* FIQ */
     	bank = mode_to_bank(FIQ_MODE);
		write_spsr (read_cpsr());
		write_cpsr (((read_cpsr() & ~(mode | (1<<5))) | (3<<6) | FIQ_MODE));
      	write_gpr2(LRIND, temp + esize);
		break;
	case AddrExceptnV:	/* Address Exception, 26-bit only */
		assert(0);	// should never happen
		break;
	}
	write_gpr2(PC_REAL_IND, mmu->high_vector(vector));
}


/* This routine controls the saving and restoring of registers across mode
   changes.  The regbank matrix is largely unused, only rows 13 and 14 are
   used across all modes, 8 to 14 are used for FIQ, all others use the USER
   column.  It's easier this way.  old and new parameter are modes numbers.*/
void arm_emulator::switch_reg_bank (cpu_mode_t old_mode, cpu_mode_t new_mode)
{
	unsigned i;
	reg_bank_t old_bank;
	reg_bank_t new_bank;

	old_bank = mode_to_bank (old_mode);
	new_bank = bank = mode_to_bank (new_mode);

	/* Do we really need to do it?  */
	if (old_bank != new_bank) {
		/* Save away the old registers.  */
		switch (old_bank) {
			case USR_BANK:
			case IRQ_BANK:
			case SVC_BANK:
			case ABT_BANK:
			case UND_BANK:
				if (new_bank == FIQ_BANK)
					for (i = 8; i < 13; i++)
						my_regs.reg_bank[USR_BANK][i] = my_regs.gpr[i];
				my_regs.reg_bank[old_bank][13] = my_regs.gpr[13];
				my_regs.reg_bank[old_bank][14] = my_regs.gpr[14];
				break;
			case FIQ_BANK:
				for (i = 8; i < 15; i++)
					my_regs.reg_bank[FIQ_BANK][i] = my_regs.gpr[i];
				break;
			case DUM_BANK:
				for (i = 8; i < 15; i++)
					my_regs.reg_bank[DUM_BANK][i] = 0;
				break;
			default:
				abort ();
		}

		/* Restore the new registers.  */
		switch (new_bank) {
			case USR_BANK:
			case IRQ_BANK:
			case SVC_BANK:
			case ABT_BANK:
			case UND_BANK:
				if (old_bank == FIQ_BANK)
					for (i = 8; i < 13; i++)
						my_regs.gpr[i] = my_regs.reg_bank[USR_BANK][i];
				my_regs.gpr[13] = my_regs.reg_bank[new_bank][13];
				my_regs.gpr[14] = my_regs.reg_bank[new_bank][14];
				break;
			case FIQ_BANK:
				for (i = 8; i < 15; i++)
					my_regs.gpr[i] = my_regs.reg_bank[FIQ_BANK][i];
				break;
			case DUM_BANK:
				for (i = 8; i < 15; i++)
					my_regs.gpr[i] = 0;
				break;
			default:
				abort ();
		}
	}
}


bool arm_emulator::int_pending (void)
{
	if (SigSet) {

		/* Any exceptions ?  */
		if (!NresetSig) {
			raise_exception (ResetV);
			return true;
		}
		else if ((!NfiqSig) && (!my_regs.f_flag)) {
			raise_exception (FIQV);
			return true;
		}
		else if ((!NirqSig) && (!my_regs.i_flag)) {
			raise_exception (IRQV);
			return true;
		}
	}

	return false;
}

