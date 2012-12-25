#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "pxa_io.h"

#define F_CORE (100 * 1024 * 1024)	//core frequence
#define F_RTC 32768		//RTC
#define F_OS	3686400		//OS timer
#define RT_SCALE (F_CORE / F_RTC) / 50
#define OS_SCALE (F_CORE / F_OS / 10) / 50

#define FF_SCALE	200	//FF UART


using namespace simit;

enum
{
	RTC_ALARM_IRQ = (1 << 31),
	RTC_HZ_IRQ = (1 << 30),
	OS_IRQ_SHF = 26,
	FFUART_IRQ = (1 << 22),
};

enum
{
	ICIP = 0x40D00000,	/* Interrupt Controller IRQ Pending Register */
	ICMR = 0x40D00004,	/* Interrupt Controller Mask Register */
	ICLR = 0x40D00008,	/* Interrupt Controller Level Register */
	ICFP = 0x40D0000C,	/* Interrupt Controller FIQ Pending Register */
	ICPR = 0x40D00010,	/* Interrupt Controller Pending Register */
	ICCR = 0x40D00014,	/* Interrupt Controller Control Register */

	RCNR = 0x40900000,	/* RTC Count Register */
	RTAR = 0x40900004,	/* RTC Alarm Register */
	RTSR = 0x40900008,	/* RTC Status Register */
	RTTR = 0x4090000C,	/* RTC Timer Trim Register */

	OSMR0 = 0x40A00000,
	OSMR1 = 0x40A00004,
	OSMR2 = 0x40A00008,
	OSMR3 = 0x40A0000C,
	OSCR = 0x40A00010,	/* OS Timer Counter Register */
	OSSR = 0x40A00014,	/* OS Timer Status Register */
	OWER = 0x40A00018,	/* OS Timer Watchdog Enable Register */
	OIER = 0x40A0001C,	/* OS Timer Interrupt Enable Register */


	/*Full Function UART */
	FFRBR = 0x40100000,	/* Receive Buffer Register (read only) */
	FFTHR = 0x40100000,	/* Transmit Holding Register (write only) */
	FFIER = 0x40100004,	/* Interrupt Enable Register (read/write) */
	FFIIR = 0x40100008,	/* Interrupt ID Register (read only) */
	FFFCR = 0x40100008,	/* FIFO Control Register (write only) */
	FFLCR = 0x4010000C,	/* Line Control Register (read/write) */
	FFMCR = 0x40100010,	/* Modem Control Register (read/write) */
	FFLSR = 0x40100014,	/* Line Status Register (read only) */
	FFMSR = 0x40100018,	/* Reserved */
	FFSPR = 0x4010001C,	/* Scratch Pad Register (read/write) */
	FFISR = 0x40100020,	/* Infrared Selection Register (read/write) */
	FFDLL = 0x40100000,	/* Divisor Latch Low Register (DLAB = 1) (read/write) */
	FFDLH = 0x40100004,	/* Divisor Latch High Register (DLAB = 1) (read/write) */
	/*Standard UART */
	BTRBR = 0x40200000,	/* Receive Buffer Register (read only) */
	BTTHR = 0x40200000,	/* Transmit Holding Register (write only) */
	BTIER = 0x40200004,	/* Interrupt Enable Register (read/write) */
	BTIIR = 0x40200008,	/* Interrupt ID Register (read only) */
	BTFCR = 0x40200008,	/* FIFO Control Register (write only) */
	BTLCR = 0x4020000C,	/* Line Control Register (read/write) */
	BTMCR = 0x40200010,	/* Modem Control Register (read/write) */
	BTLSR = 0x40200014,	/* Line Status Register (read only) */
	BTMSR = 0x40200018,	/* Reserved */
	BTSPR = 0x4020001C,	/* Scratch Pad Register (read/write) */
	BTISR = 0x40200020,	/* Infrared Selection Register (read/write) */
	BTDLL = 0x40200000,	/* Divisor Latch Low Register (DLAB = 1) (read/write) */
	BTDLH = 0x40200004,	/* Divisor Latch High Register (DLAB = 1) (read/write) */
	/*Standard UART */
	STRBR = 0x40700000,	/* Receive Buffer Register (read only) */
	STTHR = 0x40700000,	/* Transmit Holding Register (write only) */
	STIER = 0x40700004,	/* Interrupt Enable Register (read/write) */
	STIIR = 0x40700008,	/* Interrupt ID Register (read only) */
	STFCR = 0x40700008,	/* FIFO Control Register (write only) */
	STLCR = 0x4070000C,	/* Line Control Register (read/write) */
	STMCR = 0x40700010,	/* Modem Control Register (read/write) */
	STLSR = 0x40700014,	/* Line Status Register (read only) */
	STMSR = 0x40700018,	/* Reserved */
	STSPR = 0x4070001C,	/* Scratch Pad Register (read/write) */
	STISR = 0x40700020,	/* Infrared Selection Register (read/write) */
	STDLL = 0x40700000,	/* Divisor Latch Low Register (DLAB = 1) (read/write) */
	STDLH = 0x40700004,	/* Divisor Latch High Register (DLAB = 1) (read/write) */

	//core clock regs
	CCCR = 0x41300000,	/* Core Clock Configuration Register */
	CKEN = 0x41300004,	/* Clock Enable Register */
	OSCC = 0x41300008,	/* Oscillator Configuration Register */

	//ywc,2004-11-30, add LCD control register
	LCCR0 = 0x44000000,
	LCCR1 = 0x44000004,
	LCCR2 = 0x44000008,
	LCCR3 = 0x4400000C,

	FDADR0 = 0x44000200,
	FSADR0 = 0x44000204,
	FIDR0 = 0x44000208,
	LDCMD0 = 0x4400020C,

	FDADR1 = 0x44000210,
	FSADR1 = 0x44000214,
	FIDR1 = 0x44000218,
	LDCMD1 = 0x4400021C
	//ywc,2004-11-30, add LCD control register,end
};


void pxa_io::reset ()
{
	icpr = 0;
	icip = 0;
	icfp = 0;
	icmr = 0;
	iccr = 0;
	iclr = 0;

	rcnr = 0;
	rtar = 0;
	rtsr = 0;
	rttr = 0;

	rt_scale = 0;
	rt_count = 0;

	/*os timer */
	oscr = 0;
	osmr0 = osmr1 = osmr2 = osmr3 = 0;
	ower = 0;
	ossr = 0;
	oier = 0;

	os_scale = 0;


	/*full function uart controller */
	ffrbr = 0;
	ffthr = 0;
	ffier = 0;
	ffiir = 0;
	fffcr = 0;
	fflcr = 0;
	ffmcr = 0;
	fflsr = 0;
	ffmsr = 0;
	ffspr = 0;
	ffisr = 0;
	ffdll = 0;
	ffdlh = 0;

	ff_scale = 0;

	oscc = 0;

	fdadr0 = 0;
	fdadr1 = 0;

	fsadr0 = 0;
	fsadr1 = 0;


	cccr = 0x121;	// 1 0010 0001
	cken = 0x17def;

}

pxa_io::pxa_io (arm_emulator *emu) : arm_io(emu)
{
	reset();
}

pxa_io::~pxa_io ()
{
}

void pxa_io::do_cycle ()
{
	/*RTC*/ 
	if (++rt_scale >= RT_SCALE) {
		rt_scale = 0;
		if (rt_count++ == (rttr & 0xffff)) {
			rt_count = 0;

			if (rcnr++ == rtar) {
				if (rtsr & 0x4) {
					rtsr |= 0x1;
				};
			}
			if (rtsr & 0x8) {
				rtsr |= 0x2;
			}
		}
		if ((rtsr & 0x1) && (rtsr & 0x4))
			icpr |= RTC_ALARM_IRQ;
		if ((rtsr & 0x2) && (rtsr & 0x8))
			icpr |= RTC_HZ_IRQ;
	}

	/*OS timer */
	if (++os_scale >= OS_SCALE) {
		unsigned mask = 0;
		unsigned count;


		os_scale = 0;
		count = oscr++;

		if (count == osmr0)
			mask = 1;
		if (count == osmr1)
			mask |= 0x2;
		if (count == osmr2)
			mask |= 0x4;
		if (count == osmr3) {
			mask |= 0x8;
			if (ower & 1) {
				emu->raise_reset_signal();
			}
		}
		ossr |= mask;
		mask = oier & ossr;
		icpr |= (mask << OS_IRQ_SHF);
	}

	/*FF UART */

	if (++ff_scale >= FF_SCALE) {
		ff_scale = 0;
		if (!(FFUART_IRQ & icpr)) {
			fd_set rset;
			struct timeval tv;

			FD_ZERO (&rset);
			FD_SET (0, &rset);
			tv.tv_sec = 0;
			tv.tv_usec = 0;
			if (select (1, &rset, NULL, NULL, &tv) == 1) {
				
				unsigned char c;
				read (0, &c, 1);

				ffrbr = (int) c;
				ffiir |= 0x4;	//Rx idle
				fflsr |= 0x01;	//Data ready

			}
			if ((ffier & 0x1) && (ffiir & 0x4)) {
				icpr |= FFUART_IRQ;
				ffiir &= ~0x1;
			}
			/*chy 2004-07-21 from luzhetao: produce a IRQ and ox2 should clean by OS driver */
			if (ffier & 0x2) {
				icpr |= FFUART_IRQ;
				ffiir |= 0x2;
				ffiir &= ~0x1;
				fflsr |= 0x60;
			}
		}

	}

	icip = (icmr & icpr) & ~iclr;
	icfp = (icmr & icpr) & iclr;
	if (icip) emu->raise_irq_signal(); else emu->clear_irq_signal();
	if (icfp) emu->raise_fiq_signal(); else emu->clear_fiq_signal();

}

memory_fault_t pxa_io::read_word(target_addr_t addr, word_t *p)
{
	word_t data = 0;

	switch (addr) {
	 /*RTC*/ 
	case RCNR:
		data = rcnr;
		break;
	case RTAR:
		data = rtar;
		break;
	case RTSR:
		data = rtsr;
		break;
	case RTTR:
		data = rttr;
		break;
	/*OS timer */
	case OSCR:
		data = oscr;
		break;
	case OSMR0:
		data = osmr0;
		break;
	case OSMR1:
		data = osmr1;
		break;
	case OSMR2:
		data = osmr2;
		break;
	case OSMR3:
		data = osmr3;
		break;
	case OWER:
		data = ower;
		break;
	case OSSR:
		data = ossr;
		break;
	case OIER:
		data = oier;
		break;

		/*interrupt controler */
	case ICPR:
		data = icpr;
		break;
	case ICIP:
		data = (icmr & icpr) & ~iclr;
		break;
	case ICFP:
		data = (icmr & icpr) & iclr;
		break;
	case ICMR:
		data = icmr;
		break;
	case ICLR:
		data = iclr;
		break;
	/* ffuart control */
	case FFRBR:		
		{
			
			data = ffrbr & 0xff;
			icpr &= ~FFUART_IRQ;
			icip &= ~FFUART_IRQ;
			ffiir &= ~0x4;
			ffiir |= 0x1;
			fflsr &= ~0x1;

		};
		break;
	case FFIER:
		data = ffier;
		break;
	case FFIIR:		//read only
		data = ffiir & 0xcf;
		icpr &= ~FFUART_IRQ;
		icip &= ~FFUART_IRQ;
		break;
	case FFLCR:
		data = fflcr;
		break;
	case FFLSR:		//read only 
		fflsr |= 0x60;
		data = fflsr & 0xff;
		break;
		// core clock 
	case CCCR:
		data = cccr;
		break;
	case CKEN:
		data = cken;
		break;
	case OSCC:
		data = oscc;
		break;

	default:
		data = 0;
		break;	
	};

	*p = data;
	return MEM_NO_FAULT;
};

memory_fault_t pxa_io::write_word(target_addr_t addr, word_t data)
{

	switch (addr) {
	 /*RTC*/ 
	case RCNR:
		rcnr = data;
		break;
	case RTAR:
		rtar = data;
		break;
	case RTSR:
		rtsr |= (data & 0xc);
		rtsr &= ~(data & 0x3);
		break;
	case RTTR:
		rttr = data & 0x3ffffff;
		break;
	/*OS timer */
	case OSCR:
		oscr = data;
		break;
	case OSMR0:
		osmr0 = data;
		break;
	case OSMR1:
		osmr1 = data;
		break;
	case OSMR2:
		osmr2 = data;
		break;
	case OSMR3:
		osmr3 = data;
		break;
	case OWER:
		ower |= data & 0x1;
		break;
	case OSSR:
		ossr &= ~(data & 0xf);
		break;
	case OIER:
		oier = data & 0xf;
		break;

		/*interrupt control */
		//ywc,2004-11-30,for touchscreen use ICPR
	case ICPR:
		icpr = data;
		//printf("\n write ICPR=%x",icpr);
		//printf("\n");
		break;
		//ywc,2004-11-30,for touchscreen use ICPR,end

		//case ICIP:
		//case ICFP:
		/*read only */
		//      break;
	case ICMR:
		icmr = data;
		icpr &= ~data;
		break;
	case ICLR:
		iclr = data;
		break;
	//ffuart control
	case FFTHR:
		{
			unsigned char c = data;
			write (1, &c, 1);
			
			ffiir &= ~0x2;
			ffiir |= 0x1;
			fflsr &= ~0x60;
		}
		break;
	case FFIER:
		ffier = data & 0xff;
		break;
	case FFFCR:		//write only
		fffcr = data & 0xc7;
		break;
	case FFLCR:
		fflcr = data & 0xff;
		break;

	//core clock 
	case CCCR:
		cccr = data & 0x3ff;
		break;
	case CKEN:
		cken = data & 0x17def;
		break;
	case OSCC:
		oscc = data & 0x3;
		break;

	default:
		return MEM_NO_FAULT;
	};
	return MEM_NO_FAULT;
}

