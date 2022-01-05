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
			z80->regs.pc += 1; \
			return true; \
	} \
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
			z80->regs.pc += 1; \
			return true; \
	} \
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
			z80->regs.pc += 1; \
			return true; \
	} \
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
			return false;
		case 2:
			return false;
		case 3:
			z80->regs.pc += 2;
			return true;
	}
	return true;
}

INSTR_DEF(ld_hl_spn)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
			return false;
		case 2:
			z80->regs.pc += 2;
			return true;
	}
	return true;
}
