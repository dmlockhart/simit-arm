#ifndef SIMIT_ARMCOPRO_H
#define SIMIT_ARMCOPRO_H

#include "armemul.h"
#include "armmmu.h"

namespace simit {

#define ARMul_FIRST 0
#define ARMul_TRANSFER 1
#define ARMul_BUSY 2
#define ARMul_DATA 3
#define ARMul_INTERRUPT 4
#define ARMul_DONE 0
#define ARMul_CANT 1
#define ARMul_INC 3

class arm_copro {

	public:
		arm_copro(arm_emulator *emu) : emu(emu) {}
		virtual ~arm_copro() {}

		virtual unsigned LDC (unsigned type, word_t instr, word_t data) = 0;
		virtual unsigned STC (unsigned type, word_t instr, word_t * data) = 0;
		virtual unsigned MRC (unsigned type, word_t instr, word_t * value) = 0;
		virtual unsigned MCR (unsigned type, word_t instr, word_t value) = 0;
		virtual unsigned CDP (unsigned type, word_t instr) = 0;

	protected:
		arm_emulator *emu;
};

/*
class copro0:public arm_copro{

	public:

	copro0(arm_emulator *emu) : arm_copro(emu) {
		Accumulator = 0;
	}
	unsigned LDC (unsigned type, word_t instr, word_t data) {
		return ARMul_CANT;
	}
	unsigned STC (unsigned type, word_t instr, word_t * data) {
		return ARMul_CANT;
	}
	unsigned MRC (unsigned type, word_t instr, word_t * value) {
		return ARMul_CANT;
	}
	unsigned MCR (unsigned type, word_t instr, word_t value) {
		return ARMul_CANT;
	}
	unsigned CDP (unsigned type, word_t instr) {
		return ARMul_CANT;
	}
	~copro0(){};
	uint64_t Accumulator;
};*/

class copro1 : public arm_copro
{
	public:
		copro1(arm_emulator *emu) : arm_copro(emu) {}

		unsigned LDC (unsigned type, word_t instr, word_t data) {
			return ARMul_CANT;
		}
		unsigned STC (unsigned type, word_t instr, word_t * data) {
			return ARMul_CANT;
		}
		unsigned MRC (unsigned type, word_t instr, word_t * value);
		unsigned MCR (unsigned type, word_t instr, word_t value);
		unsigned CDP (unsigned type, word_t instr){
			return ARMul_CANT;
		}
		~copro1() {};
};

class copro2 : public arm_copro
{
	public:
		copro2(arm_emulator *emu) : arm_copro(emu) {}
		unsigned LDC (unsigned type, word_t instr, word_t data);
		unsigned STC (unsigned type, word_t instr, word_t * data);
		unsigned MRC (unsigned type, word_t instr, word_t * value){
			return ARMul_CANT;
		}
		unsigned MCR (unsigned type, word_t instr, word_t value){
			return ARMul_CANT;
		}
		unsigned CDP (unsigned type, word_t instr){
			return ARMul_CANT;
		}
		~copro2(){};
};


class copro4 : public arm_copro
{
	public:
		copro4(arm_emulator *emu) : arm_copro(emu) {}
		unsigned LDC (unsigned type, word_t instr, word_t data);
		unsigned STC (unsigned type, word_t instr, word_t * data);
		unsigned MRC (unsigned type, word_t instr, word_t * value);
		unsigned MCR (unsigned type, word_t instr, word_t value);
		unsigned CDP (unsigned type, word_t instr);
		~copro4(){};
};

class copro5 : public arm_copro
{

	public:
		copro5(arm_emulator *emu) : arm_copro(emu) {}
		unsigned LDC (unsigned type, word_t instr, word_t data) {
			return ARMul_CANT;
		}
		unsigned STC (unsigned type, word_t instr, word_t * data){
			return ARMul_CANT;
		}
		unsigned MRC (unsigned type, word_t instr, word_t * value);
		unsigned MCR (unsigned type, word_t instr, word_t value);
		unsigned CDP (unsigned type, word_t instr);
		~copro5(){};
};

#if 1
//#ifdef XScale_mach

class copro6 : public arm_copro // pxa27x
{
	public:
		copro6(arm_emulator *emu) : arm_copro(emu) {
			last_creg = 0;
		}
	
		unsigned LDC (unsigned type, word_t instr, word_t data){
			return ARMul_CANT;
		}
		unsigned STC (unsigned type, word_t instr, word_t * data){
			return ARMul_CANT;
		}
		unsigned MRC (unsigned type, word_t instr, word_t * value);
		unsigned MCR (unsigned type, word_t instr, word_t value);
	
		unsigned CDP (unsigned type, word_t instr){
			return ARMul_CANT;
		}
	
		~copro6() {};
	
	private:
		word_t last_creg;
};

typedef struct xscale_cp14_reg_s
{
	unsigned cclkcfg;
	unsigned pwrmode;
} xscale_cp14_reg_s;

class copro14:public arm_copro // XScale
{
	public:
	
		copro14(arm_emulator *emu) : arm_copro(emu) {
			pxa_cp14_regs.cclkcfg = 0;
			pxa_cp14_regs.pwrmode = 0;
		}
		unsigned LDC (unsigned type, word_t instr, word_t data){
			return ARMul_CANT;
		}
		unsigned STC (unsigned type, word_t instr, word_t * data){
			return ARMul_CANT;
		}
		unsigned MRC (unsigned type, word_t instr, word_t * value);
		unsigned MCR (unsigned type, word_t instr, word_t value);
	
		unsigned CDP (unsigned type, word_t instr){
			return ARMul_CANT;
		}
	
		~copro14() {};

	private:
		xscale_cp14_reg_s pxa_cp14_regs;
};

#endif

class copro15 : public arm_copro
{
	public:
		copro15(arm_emulator *emu) : arm_copro(emu) {}

		unsigned LDC (unsigned type, word_t instr, word_t data){
			return ARMul_CANT;
		}
		unsigned STC (unsigned type, word_t instr, word_t * data){
			return ARMul_CANT;
		}
		unsigned MRC (unsigned type, word_t instr, word_t * value){
			if (emu->is_xscale()) 
				* value= emu->mmu->xscale_mrc(instr);
			else
				* value= emu->mmu->mrc(instr);
			return ARMul_DONE; 
		}
		unsigned MCR (unsigned type, word_t instr, word_t value){
			if (emu->is_xscale()) 
				emu->mmu->xscale_mcr(instr,value);
			else
				emu->mmu->mcr(instr,value);
			return ARMul_DONE; 
		}
		unsigned CDP (unsigned type, word_t instr){
			return ARMul_CANT;
		}
		~copro15() {};
};


} /* namespace */
#endif /* _ARMCOPRO_H_ */
