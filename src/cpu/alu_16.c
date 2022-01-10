#include "instr.h"
#include "cpu.h"

#define INSTR_DEF_INC_RR(rr) \
INSTR_DEF(inc_##rr) \
{ \
	switch (count) \
	{ \
		case 0: \
			return false; \
		case 1: \
			cpu->regs.rr++; \
			break; \
	} \
	cpu->regs.pc += 1; \
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
			cpu->regs.rr--; \
			break; \
	} \
	cpu->regs.pc += 1; \
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
			uint16_t v = cpu->regs.hl; \
			cpu->regs.hl += cpu->regs.rr; \
			CPU_SET_FLAG_N(cpu, 0); \
			cpu_update_hflag16(cpu, v, cpu->regs.hl, 1); \
			cpu_update_cflag16(cpu, v, cpu->regs.hl, 1); \
			break; \
		} \
	} \
	cpu->regs.pc += 1; \
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
			cpu->instr_tmp.u16[1] = cpu->regs.sp;
			cpu->instr_tmp.u16[0] = cpu->regs.sp + (int8_t)mem_get8(cpu->mem, cpu->regs.pc + 1);
			return false;
		case 2:
			cpu->regs.sp = (cpu->regs.sp & 0xFF00) | (cpu->instr_tmp.u16[0] & 0x00FF);
			return false;
		case 3:
		{
			uint16_t v = cpu->instr_tmp.u16[1];
			cpu->regs.sp = (cpu->regs.sp & 0x00FF) | (cpu->instr_tmp.u16[0] & 0xFF00);
			CPU_SET_FLAG_Z(cpu, 0);
			CPU_SET_FLAG_N(cpu, 0);
			cpu_update_hflag16(cpu, v, cpu->regs.sp, 1);
			cpu_update_cflag16(cpu, v, cpu->regs.sp, 1);
			break;
		}
	}
	cpu->regs.pc += 2;
	return true;
}

INSTR_DEF(ld_hl_spn)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
			cpu->instr_tmp.i8[0] = (int8_t)mem_get8(cpu->mem, cpu->regs.pc + 1);
			return false;
		case 2:
		{
			uint16_t v = cpu->regs.hl;
			cpu->regs.hl = cpu->regs.sp + cpu->instr_tmp.i8[0];
			CPU_SET_FLAG_Z(cpu, 0);
			CPU_SET_FLAG_N(cpu, 0);
			cpu_update_hflag16(cpu, v, cpu->regs.hl, 1);
			cpu_update_cflag16(cpu, v, cpu->regs.hl, 1);
			break;
		}
	}
	cpu->regs.pc += 2;
	return true;
}
