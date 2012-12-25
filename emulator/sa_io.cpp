#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "sa_io.h"

#define F_CORE (100 * 1024 * 1024)	//core frequence
#define F_RTC 32768		//RTC
#define F_OS	3686400		//OS timer
#define RT_SCALE (F_CORE / F_RTC)
#define OS_SCALE (F_CORE / F_OS / 10)

#define UART3_SCALE	200

using namespace simit;

enum
{
	RTC_ALARM_IRQ = (1 << 31),
	RTC_HZ_IRQ = (1 << 30),
	OS_IRQ_SHF = 26,
	UART3_IRQ = (1 << 17),
};

enum
{
	ICIP = 0x90050000,
	ICMR = 0x90050004,
	ICLR = 0x90050008,
	ICFP = 0x90050010,
	ICPR = 0x90050020,
	ICCR = 0x9005000C,

	/* reset controller */
	RSRR = 0x90030000,
	RCSR = 0x90030004,

	/* PM control register */
	PMCR = 0x90020000,

	RTAR = 0x90010000,
	RCNR = 0x90010004,
	RTTR = 0x90010008,
	RTSR = 0x90010010,

	OSMR0 = 0x90000000,
	OSMR1 = 0x90000004,
	OSMR2 = 0x90000008,
	OSMR3 = 0x9000000C,
	OSCR = 0x90000010,
	OSSR = 0x90000014,
	OWER = 0x90000018,
	OIER = 0x9000001C,

	/*UART3 */
	UTCR0 = 0x80050000,
	UTCR1 = 0x80050004,
	UTCR2 = 0x80050008,
	UTCR3 = 0x8005000C,
	UTDR = 0x80050014,
	UTSR0 = 0x8005001C,
	UTSR1 = 0x80050020,
};

void sa_io::reset ()
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

	/*uart3 controller */
	utcr0 = 0;
	utcr1 = 0;
	utcr2 = 0;
	utcr3 = 0;
	utdr = 0;
	utsr0 = 0;
	utsr1 = 0;

	uart3_scale = 0;

	utsr0 = 1;	/*always TFS, no others */
	utsr1 = 0x4;

}

sa_io::sa_io (arm_emulator *emu) : arm_io(emu)
{
	reset();
}

sa_io::~sa_io ()
{
}

void sa_io::do_cycle ()
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
	};

	/*OS timer */
	if (++os_scale >= OS_SCALE) {
		uint32_t mask = 0;
		uint32_t count;

		os_scale = 0;
		// printf("count = oscr = %x + 1\n",oscr);
		count = oscr++;

		if (count == osmr0)
			mask = 1;
		if (count == osmr1)
			mask |= 0x2;
		if (count == osmr2)
			mask |= 0x4;
		if (count == osmr3) {
			mask |= 0x8;
			if (ower & 1)
				emu->raise_reset_signal();
		}
		ossr |= mask;
	}


	/*UART 3 */
	if (++uart3_scale > UART3_SCALE) {
		fd_set rset;
		struct timeval tv;


		utsr0 |= 1;	/*always TFS, no others */
		utsr1 |= 0x4;
		/*TNF*/ uart3_scale = 0;
		// fprintf(stderr,"PC = 0x%x\n",PC);
		FD_ZERO (&rset);
		FD_SET (0, &rset);
		tv.tv_sec = 0;
		tv.tv_usec = 0;
		if (select(1, &rset, NULL, NULL,&tv) == 1) {
			utsr1 |= 0x2;	//RNE
			utsr0 |= 0x4;	//RID
		}
	}

	/*reset interrupt pin status */
	refresh_irq ();
}

void sa_io::refresh_irq ()
{
	uint32_t irq = 0;
	uint32_t mask;

	 /*RTC*/
	 if ((rtsr & 0x1) && (rtsr & 0x4))
		irq |= RTC_ALARM_IRQ;


	if ((rtsr & 0x2) && (rtsr & 0x8))
		irq |= RTC_HZ_IRQ;
	
		
	/*OS time */
	mask = oier & ossr;
	irq |= (mask << OS_IRQ_SHF);
	

	/*UART3 */
	if ((utcr3 & 0x8) && (utsr0 & 0x6))
		irq |= UART3_IRQ;
	
	
	if (utsr0 & 0x38)
		irq |= UART3_IRQ;

	if ((utcr3 & 0x10) && (utsr0 & 0x1))
		irq |= UART3_IRQ;

	icpr = irq;
	icip = (icmr & icpr) & ~iclr;
	icfp = (icmr & icpr) & iclr;

	if (icip) emu->raise_irq_signal(); else emu->clear_irq_signal();
	if (icfp) emu->raise_fiq_signal(); else emu->clear_fiq_signal();
}

memory_fault_t sa_io::read_word(target_addr_t addr, word_t *p)
{
	word_t data;

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
//		printf("oscr = %x\n",oscr);
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
	/*UART 3 */
	case UTCR0:
		data = utcr0;
		break;
	case UTCR1:
		data = utcr1;
		break;
	case UTCR2:
		data = utcr2;
		break;
	case UTCR3:
		data = utcr3;
		break;
	case UTDR:
		{
			char c;
			read (0, &c, 1);
			data = c;
		}

		utsr0 = 1;	/*always TFS, no others */
		utsr1 = 0x4;
		 /*TNF*/ break;
	case UTSR0:
		data = utsr0;
		break;
	case UTSR1:
		data = utsr1;
		break;

	default:
		data = 0;
		break;	
	};

	*p = data;
	return MEM_NO_FAULT;
}

memory_fault_t sa_io::write_word(target_addr_t addr, word_t data)
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
//		printf("oscr = data = %x\n",oscr);
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
	case ICPR:
	case ICIP:
	case ICFP:
		/*read only */
		break;
	case ICMR:
		icmr = data;
		break;
	case ICLR:
		iclr = data;
		break;
	/*UART 3 */
	case UTCR0:
		utcr0 = data & 0x7f;
		break;
	case UTCR1:
		utcr1 = data & 0xf;
		break;
	case UTCR2:
		utcr2 = data & 0xff;
		break;
	case UTCR3:
		utcr3 = data & 0x3f;
		break;
	case UTDR:
		{
			char c = data;
			write (1, &c, 1);
			//log_msg("UTDR write\n");
		}
		break;
	case UTSR0:
		utsr0 &= ~(data & 0x1b);
		break;
	case PMCR: // sleep mode == power off
	case RSRR: // power off
		if (data & 1) 
			emu->raise_reset_signal();
		break;
	default:
		return MEM_NO_FAULT;
	}
	return MEM_NO_FAULT;
}

