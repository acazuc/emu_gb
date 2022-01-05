#include "instr.h"
#include "z80.h"

#define INSTR_DEF_LD_RR_NN(rr) \
INSTR_DEF(ld_##rr##_nn) \
{ \
	switch (count) \
	{ \
		case 0: \
			return false; \
		case 1: \
			return false; \
		case 2: \
			z80->regs.pc += 3; \
			return true; \
	} \
	return true; \
}

INSTR_DEF_LD_RR_NN(bc);
INSTR_DEF_LD_RR_NN(de);
INSTR_DEF_LD_RR_NN(hl);
INSTR_DEF_LD_RR_NN(sp);

INSTR_DEF(ld_nn_sp)
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
			return false;
		case 4:
			z80->regs.pc += 3;
			return true;
	}
	return true;
}

#define INSTR_DEF_PUSH_POP_RR(rr) \
INSTR_DEF(push_##rr) \
{ \
	switch (count) \
	{ \
		case 0: \
			return false; \
		case 1: \
			return false; \
		case 2: \
			return false; \
		case 3: \
			z80->regs.pc += 1; \
			return true; \
	} \
	return true; \
} \
INSTR_DEF(pop_##rr) \
{ \
	switch (count) \
	{ \
		case 0: \
			return false; \
		case 1: \
			return false; \
		case 2: \
			z80->regs.pc += 1; \
			return true; \
	} \
	return true; \
}

INSTR_DEF_PUSH_POP_RR(bc);
INSTR_DEF_PUSH_POP_RR(de);
INSTR_DEF_PUSH_POP_RR(hl);
INSTR_DEF_PUSH_POP_RR(af);

INSTR_DEF(ld_sp_hl)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
			z80->regs.pc += 1;
			return true;
	}
	return true;
}
