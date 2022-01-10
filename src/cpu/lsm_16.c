#include "instr.h"
#include "cpu.h"

#define INSTR_DEF_LD_RR_NN(r1, r2) \
INSTR_DEF(ld_##r1####r2##_nn) \
{ \
	switch (count) \
	{ \
		case 0: \
			return false; \
		case 1: \
			cpu->regs.r2 = mem_get8(cpu->mem, cpu->regs.pc + 1); \
			return false; \
		case 2: \
			cpu->regs.r1 = mem_get8(cpu->mem, cpu->regs.pc + 2); \
			break; \
	} \
	cpu->regs.pc += 3; \
	return true; \
}

INSTR_DEF_LD_RR_NN(b, c);
INSTR_DEF_LD_RR_NN(d, e);
INSTR_DEF_LD_RR_NN(h, l);
INSTR_DEF_LD_RR_NN(s, p);

INSTR_DEF(ld_nn_sp)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
			cpu->instr_tmp.u16[0] = mem_get8(cpu->mem, cpu->regs.pc + 1);
			return false;
		case 2:
			cpu->instr_tmp.u16[0] |= mem_get8(cpu->mem, cpu->regs.pc + 2) << 8;
			return false;
		case 3:
			mem_set8(cpu->mem, cpu->instr_tmp.u16[0], cpu->regs.sp);
			return false;
		case 4:
			mem_set8(cpu->mem, cpu->instr_tmp.u16[0] + 1, cpu->regs.sp >> 8);
			break;
	}
	cpu->regs.pc += 3;
	return true;
}

#define INSTR_DEF_PUSH_POP_RR(r1, r2) \
INSTR_DEF(push_##r1####r2) \
{ \
	switch (count) \
	{ \
		case 0: \
			return false; \
		case 1: \
			return false; \
		case 2: \
			mem_set8(cpu->mem, --cpu->regs.sp, cpu->regs.r1); \
			return false; \
		case 3: \
			mem_set8(cpu->mem, --cpu->regs.sp, cpu->regs.r2); \
			break; \
	} \
	cpu->regs.pc += 1; \
	return true; \
} \
INSTR_DEF(pop_##r1####r2) \
{ \
	switch (count) \
	{ \
		case 0: \
			return false; \
		case 1: \
			cpu->regs.r2 = mem_get8(cpu->mem, cpu->regs.sp++); \
			return false; \
		case 2: \
			cpu->regs.r1 = mem_get8(cpu->mem, cpu->regs.sp++); \
			break; \
	} \
	cpu->regs.pc += 1; \
	return true; \
}

INSTR_DEF_PUSH_POP_RR(b, c);
INSTR_DEF_PUSH_POP_RR(d, e);
INSTR_DEF_PUSH_POP_RR(h, l);
INSTR_DEF_PUSH_POP_RR(a, f);

INSTR_DEF(ld_sp_hl)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
			cpu->regs.sp = cpu->regs.hl;
			break;
	}
	cpu->regs.pc += 1;
	return true;
}
