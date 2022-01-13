#include "instr.h"
#include "cpu.h"
#include <stdio.h>

#define INSTR_DEF_JR_JP_CALL_RET_FLAG(f, flag, ref) \
INSTR_DEF(jr_##f##_n) \
{ \
	switch (count) \
	{ \
		case 0: \
			return false; \
		case 1: \
			cpu->instr_tmp.i8[0] = (int8_t)mem_get8(cpu->mem, cpu->regs.pc + 1); \
			if (CPU_GET_FLAG(cpu, flag) != ref) \
				break; \
			return false; \
		case 2: \
			cpu->regs.pc += cpu->instr_tmp.i8[0] + 2; \
			return true; \
	} \
	cpu->regs.pc += 2; \
	return true; \
} \
INSTR_DEF(jp_##f##_nn) \
{ \
	switch (count) \
	{ \
		case 0: \
			return false; \
		case 1: \
			cpu->instr_tmp.u16[0] = mem_get8(cpu->mem, cpu->regs.pc + 1); \
			return false; \
		case 2: \
			cpu->instr_tmp.u16[0] |= mem_get8(cpu->mem, cpu->regs.pc + 2) << 8; \
			if (CPU_GET_FLAG(cpu, flag) != ref) \
				break; \
			return false; \
		case 3: \
			cpu->regs.pc = cpu->instr_tmp.u16[0]; \
			return true; \
	} \
	cpu->regs.pc += 3; \
	return true; \
} \
INSTR_DEF(call_##f##_nn) \
{ \
	switch (count) \
	{ \
		case 0: \
			return false; \
		case 1: \
			cpu->instr_tmp.u16[0] = mem_get8(cpu->mem, cpu->regs.pc + 1); \
			return false; \
		case 2: \
			cpu->instr_tmp.u16[0] |= mem_get8(cpu->mem, cpu->regs.pc + 2) << 8; \
			if (CPU_GET_FLAG(cpu, flag) != ref) \
				break; \
			return false; \
		case 3: \
			cpu->instr_tmp.u16[1] = cpu->regs.pc + 3; \
			return false; \
		case 4: \
			mem_set8(cpu->mem, --cpu->regs.sp, cpu->instr_tmp.u16[1] >> 8); \
			return false; \
		case 5: \
			mem_set8(cpu->mem, --cpu->regs.sp, cpu->instr_tmp.u16[1]); \
			cpu->regs.pc = cpu->instr_tmp.u16[0]; \
			return true; \
	} \
	cpu->regs.pc += 3; \
	return true; \
} \
INSTR_DEF(ret_##f) \
{ \
	switch (count) \
	{ \
		case 0: \
			return false; \
		case 1: \
			if (CPU_GET_FLAG(cpu, flag) != ref) \
				break; \
			return false; \
		case 2: \
			cpu->instr_tmp.u16[0] = mem_get8(cpu->mem, cpu->regs.sp++); \
			return false; \
		case 3: \
			cpu->instr_tmp.u16[0] |= mem_get8(cpu->mem, cpu->regs.sp++) << 8; \
			return false; \
		case 4: \
			cpu->regs.pc = cpu->instr_tmp.u16[0]; \
			return true; \
	} \
	cpu->regs.pc += 1; \
	return true; \
}

INSTR_DEF_JR_JP_CALL_RET_FLAG(nz, CPU_FLAG_Z, 0);
INSTR_DEF_JR_JP_CALL_RET_FLAG(z, CPU_FLAG_Z, 1);
INSTR_DEF_JR_JP_CALL_RET_FLAG(nc, CPU_FLAG_C, 0);
INSTR_DEF_JR_JP_CALL_RET_FLAG(c, CPU_FLAG_C, 1);

INSTR_DEF(jr_dd)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
			cpu->instr_tmp.i8[0] = (int8_t)mem_get8(cpu->mem, cpu->regs.pc + 1);
			return false;
		case 2:
			cpu->regs.pc += cpu->instr_tmp.i8[0] + 2;
			return true;
	}
	cpu->regs.pc += 2;
	return true;
}

INSTR_DEF(jp_nn)
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
			cpu->regs.pc = cpu->instr_tmp.u16[0];
			return true;
	}
	cpu->regs.pc += 3;
	return true;
}

INSTR_DEF(jp_hl)
{
	switch (count)
	{
		case 0:
			cpu->regs.pc = cpu->regs.hl;
			return true;
	}
	cpu->regs.pc += 1;
	return true;
}

INSTR_DEF(call_nn)
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
			cpu->instr_tmp.u16[1] = cpu->regs.pc + 3;
			return false;
		case 4:
			mem_set8(cpu->mem, --cpu->regs.sp, cpu->instr_tmp.u16[1] >> 8);
			return false;
		case 5:
			mem_set8(cpu->mem, --cpu->regs.sp, cpu->instr_tmp.u16[1]);
			cpu->regs.pc = cpu->instr_tmp.u16[0];
			return true;
	}
	cpu->regs.pc += 3;
	return true;
}

INSTR_DEF(reti)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
			cpu->instr_tmp.u16[0] = mem_get8(cpu->mem, cpu->regs.sp++);
			return false;
		case 2:
			cpu->instr_tmp.u16[0] |= mem_get8(cpu->mem, cpu->regs.sp++) << 8;
			return false;
		case 3:
			cpu->ime = true;
			cpu->regs.pc = cpu->instr_tmp.u16[0];
			return true;
	}
	cpu->regs.pc += 1;
	return true;
}

INSTR_DEF(ret)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
			cpu->instr_tmp.u16[0] = mem_get8(cpu->mem, cpu->regs.sp++);
			return false;
		case 2:
			cpu->instr_tmp.u16[0] |= mem_get8(cpu->mem, cpu->regs.sp++) << 8;
			return false;
		case 3:
			cpu->regs.pc = cpu->instr_tmp.u16[0];
			return true;
	}
	cpu->regs.pc += 1;
	return true;
}

#define INSTR_RST(n, d) \
INSTR_DEF(rst_##n) \
{ \
	switch (count) \
	{ \
		case 0: \
			return false; \
		case 1: \
			cpu->instr_tmp.u16[0] = cpu->regs.pc + 1; \
			return false; \
		case 2: \
			mem_set8(cpu->mem, --cpu->regs.sp, cpu->instr_tmp.u16[0] >> 8); \
			return false; \
		case 3: \
			mem_set8(cpu->mem, --cpu->regs.sp, cpu->instr_tmp.u16[0]); \
			cpu->regs.pc = d; \
			return true; \
	} \
	cpu->regs.pc += 1; \
	return true; \
}

INSTR_RST(00, 0x00);
INSTR_RST(08, 0x08);
INSTR_RST(10, 0x10);
INSTR_RST(18, 0x18);
INSTR_RST(20, 0x20);
INSTR_RST(28, 0x28);
INSTR_RST(30, 0x30);
INSTR_RST(38, 0x38);

#define INSTR_INT(n, d) \
INSTR_DEF(int_##n) \
{ \
	switch (count) \
	{ \
		case 0: \
			return false; \
		case 1: \
			cpu->instr_tmp.u16[0] = cpu->regs.pc; \
			return false; \
		case 2: \
			mem_set8(cpu->mem, --cpu->regs.sp, cpu->instr_tmp.u16[0] >> 8); \
			return false; \
		case 3: \
			mem_set8(cpu->mem, --cpu->regs.sp, cpu->instr_tmp.u16[0]); \
			return false; \
		case 4: \
			cpu->regs.pc = d; \
			cpu->ime = false; \
			break; \
	} \
	return true; \
}

INSTR_INT(40, 0x40);
INSTR_INT(48, 0x48);
INSTR_INT(50, 0x50);
INSTR_INT(58, 0x58);
INSTR_INT(60, 0x60);
