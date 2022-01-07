#include "instr.h"
#include "z80.h"

#define INSTR_DEF_INC_RR(rr) \
INSTR_DEF(inc_##rr) \
{ \
	switch (count) \
	{ \
		case 0: \
			return false; \
		case 1: \
			z80->regs.rr++; \
			break; \
	} \
	z80->regs.pc += 1; \
	return true; \
}

#define INSTR_DEF_DEC_RR(rr) \
INSTR_DEF(dec_##rr) \
{ \
	switch (count) \
	{ \
		case 0: \
			return false; \
		case 1: \
			z80->regs.rr--; \
			break; \
	} \
	z80->regs.pc += 1; \
	return true; \
}

#define INSTR_DEF_ADD_HL_RR(rr) \
INSTR_DEF(add_hl_##rr) \
{ \
	switch (count) \
	{ \
		case 0: \
			return false; \
		case 1: \
		{ \
			uint16_t v = z80->regs.hl; \
			z80->regs.hl += z80->regs.rr; \
			Z80_SET_FLAG_N(z80, 0); \
			z80_update_hflag(z80, v, z80->regs.rr); \
			z80_update_cflag(z80, v, z80->regs.rr); \
			break; \
		} \
	} \
	z80->regs.pc += 1; \
	return true; \
}

#define INSTR_DEF_INC_DEC_RR(rr) \
INSTR_DEF_INC_RR(rr) \
INSTR_DEF_DEC_RR(rr) \
INSTR_DEF_ADD_HL_RR(rr)

INSTR_DEF_INC_DEC_RR(bc);
INSTR_DEF_INC_DEC_RR(de);
INSTR_DEF_INC_DEC_RR(hl);
INSTR_DEF_INC_DEC_RR(sp);

INSTR_DEF(add_sp_n)
{
	switch (count) 
	{
		case 0:
			return false;
		case 1:
			z80->instr_tmp.u16[0] = z80->regs.sp + mem_gi8(z80->mem, z80->regs.pc + 1);
			return false;
		case 2:
			z80->regs.sp = (z80->regs.sp & 0xFF00) | (z80->instr_tmp.u16[0] & 0xFF);
			return false;
		case 3:
		{
			uint8_t v = z80->regs.sp;
			z80->regs.sp = (z80->regs.sp & 0xFF) | (z80->instr_tmp.u16[0] & 0xFF00);
			Z80_SET_FLAG_Z(z80, 0);
			Z80_SET_FLAG_N(z80, 0);
			z80_update_hflag(z80, v, z80->regs.sp);
			z80_update_cflag(z80, v, z80->regs.sp);
			break;
		}
	}
	z80->regs.pc += 2;
	return true;
}

INSTR_DEF(ld_hl_spn)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
			z80->instr_tmp.i8[0] = mem_gi8(z80->mem, z80->regs.pc + 1);
			return false;
		case 2:
		{
			uint8_t v = z80->regs.hl;
			z80->regs.hl = z80->regs.sp + z80->instr_tmp.i8[0];
			Z80_SET_FLAG_Z(z80, 0);
			Z80_SET_FLAG_N(z80, 0);
			z80_update_hflag(z80, v, z80->regs.hl);
			z80_update_cflag(z80, v, z80->regs.hl);
			break;
		}
	}
	z80->regs.pc += 2;
	return true;
}
