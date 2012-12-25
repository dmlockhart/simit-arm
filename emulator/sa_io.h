#ifndef SA_IO_H
#define SA_IO_H

#include "arm_io.h"

namespace simit{ 
/* IO for StrongARM */
class sa_io : public arm_io 
{
	public:

		sa_io(arm_emulator *emu);
		~sa_io();

		void reset();
		void do_cycle();

		memory_fault_t read_word(target_addr_t addr, word_t *p);
		memory_fault_t write_word(target_addr_t addr, word_t v);

	private:

		void refresh_irq ();

		/*interrupt controller */
		word_t icpr;
		word_t icip;
		word_t icfp;
		word_t icmr;
		word_t iccr;
		word_t iclr;

		/*real time clock(RTC) */
		word_t rcnr;
		word_t rtar;
		word_t rtsr;
		word_t rttr;

		word_t rt_scale;		/*core frequence to 32.768K */
		word_t rt_count;

		/*os timer */
		word_t oscr;
		word_t osmr0, osmr1, osmr2, osmr3;
		word_t ower;
		word_t ossr;
		word_t oier;

		word_t os_scale;

		/*uart3 controller */
		word_t utcr0;
		word_t utcr1;
		word_t utcr2;
		word_t utcr3;
		word_t utdr;
		word_t utsr0;
		word_t utsr1;

		word_t uart3_scale;

};

}

#endif //SA_IO_H
