#ifndef _PXA_IO_H_
#define _PXA_IO_H_

#include "arm_io.h"

namespace simit{ 

class pxa_io : public arm_io
{
	public:
	
		pxa_io(arm_emulator *emu);
		~pxa_io();

		void reset();
		void do_cycle();

		memory_fault_t read_word(target_addr_t addr, word_t *p);
		memory_fault_t write_word(target_addr_t addr, word_t v);
	
	private:
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
		//chy: maybe not regs ????
		word_t rt_scale;		/*core frequence to 32.768K */
		word_t rt_count;

		/*os timer */
		word_t oscr;
		word_t osmr0, osmr1, osmr2, osmr3;
		word_t ower;
		word_t ossr;
		word_t oier;
		//chy: maybe not regs ????
		word_t os_scale;		//chy: I can not find it in pxa-regs.h????

		/*full function uart controller */
		word_t ffrbr;
		word_t ffthr;
		word_t ffier;
		word_t ffiir;
		word_t fffcr;
		word_t fflcr;
		word_t ffmcr;
		word_t fflsr;
		word_t ffmsr;
		word_t ffspr;
		word_t ffisr;
		word_t ffdll;
		word_t ffdlh;

		word_t ff_scale;
		/*bluetooth function uart controller */
		word_t btrbr;
		word_t btthr;
		word_t btier;
		word_t btiir;
		word_t btfcr;
		word_t btlcr;
		word_t btmcr;
		word_t btlsr;
		word_t btmsr;
		word_t btspr;
		word_t btisr;
		word_t btdll;
		word_t btdlh;
		/*standard uart controller */
		word_t strbr;
		word_t stthr;
		word_t stier;
		word_t stiir;
		word_t stfcr;
		word_t stlcr;
		word_t stmcr;
		word_t stlsr;
		word_t stmsr;
		word_t stspr;
		word_t stisr;
		word_t stdll;
		word_t stdlh;
		/*core clock */
		word_t cccr;
		word_t cken;
		word_t oscc;

		//ywc,2004-11-30,add io of LCD and Touchscreen
		  /*LCD*/
			/* remove them later. */
		  word_t lccr0;
		word_t lccr1;
		word_t lccr2;
		word_t lccr3;

		word_t fdadr0;
		word_t fdadr1;

		word_t fsadr0;
		word_t fsadr1;

		/*TouchScreen */
		word_t ts_int;
		word_t ts_buffer[8];
		word_t ts_addr_begin;
		word_t ts_addr_end;
		//ywc,2004-11-30,add io of LCD and Touchscreen,end
};

}
#endif //_PXA_IO_H_
