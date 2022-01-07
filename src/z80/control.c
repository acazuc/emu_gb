#include "instr.h"
#include "z80.h"
#include <stdio.h>

#define INSTR_DEF_JR_JP_CALL_RET_FLAG(f, flag, ref) \
INSTR_DEF(jr_##f##_n) \
{ \
	switch (count) \
	{ \
		case 0: \
			return false; \
		case 1: \
			z80->instr_tmp.i8[0] = mem_gi8(z80->mem, z80->regs.pc + 1); \
			if (Z80_GET_FLAG(z80, flag) != ref) \
				break; \
			return false; \
		case 2: \
			z80->regs.pc += z80->instr_tmp.i8[0] + 2; \
			return true; \
	} \
	z80->regs.pc += 2; \
	return true; \
} \
INSTR_DEF(jp_##f##_nn) \
{ \
	switch (count) \
	{ \
		case 0: \
			return false; \
		case 1: \
			z80->instr_tmp.u16[0] = mem_gu8(z80->mem, z80->regs.pc + 1); \
			return false; \
		case 2: \
			z80->instr_tmp.u16[0] |= mem_gu8(z80->mem, z80->regs.pc + 2) << 8; \
			if (Z80_GET_FLAG(z80, flag) != ref) \
				break; \
			return false; \
		case 3: \
			z80->regs.pc += z80->instr_tmp.u16[0] + 3; \
			return true; \
	} \
	z80->regs.pc += 3; \
	return true; \
} \
INSTR_DEF(call_##f##_nn) \
{ \
	switch (count) \
	{ \
		case 0: \
			return false; \
		case 1: \
			z80->instr_tmp.u16[0] = mem_gu8(z80->mem, z80->regs.pc + 1); \
			return false; \
		case 2: \
			z80->instr_tmp.u16[0] |= mem_gu8(z80->mem, z80->regs.pc + 2) << 8; \
			if (Z80_GET_FLAG(z80, flag) != ref) \
				break; \
			return false; \
		case 3: \
			z80->instr_tmp.u16[1] = z80->regs.pc + 3; \
			return false; \
		case 4: \
			mem_su8(z80->mem, --z80->regs.sp, z80->instr_tmp.u16[1] >> 8); \
			return false; \
		case 5: \
			mem_su8(z80->mem, --z80->regs.sp, z80->instr_tmp.u16[1]); \
			z80->regs.pc = z80->instr_tmp.u16[0]; \
			return true; \
	} \
	z80->regs.pc += 3; \
	return true; \
} \
INSTR_DEF(ret_##f) \
{ \
	switch (count) \
	{ \
		case 0: \
			return false; \
		case 1: \
			if (Z80_GET_FLAG(z80, flag) == ref) \
				break; \
			return false; \
		case 2: \
			z80->instr_tmp.u16[0] = mem_gu8(z80->mem, z80->regs.sp++); \
			return false; \
		case 3: \
			z80->instr_tmp.u16[0] |= mem_gu8(z80->mem, z80->regs.sp++) << 8; \
			return false; \
		case 5: \
			z80->regs.pc = z80->instr_tmp.u16[0]; \
			return true; \
	} \
	z80->regs.pc += 3; \
	return true; \
}

INSTR_DEF_JR_JP_CALL_RET_FLAG(nz, Z80_FLAG_Z, 0);
INSTR_DEF_JR_JP_CALL_RET_FLAG(z, Z80_FLAG_Z, 1);
INSTR_DEF_JR_JP_CALL_RET_FLAG(nc, Z80_FLAG_C, 0);
INSTR_DEF_JR_JP_CALL_RET_FLAG(c, Z80_FLAG_C, 1);

INSTR_DEF(jr_dd)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
			z80->instr_tmp.i8[0] = mem_gi8(z80->mem, z80->regs.pc + 1);
			return false;
		case 2:
			z80->regs.pc += z80->instr_tmp.i8[0] + 2;
			return true;
	}
	z80->regs.pc += 2;
	return true;
}

INSTR_DEF(jp_nn)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
			z80->instr_tmp.u16[0] = mem_gu8(z80->mem, z80->regs.pc + 1);
			return false;
		case 2:
			z80->instr_tmp.u16[0] |= mem_gu8(z80->mem, z80->regs.pc + 2) << 8;
			return false;
		case 3:
			z80->regs.pc += z80->instr_tmp.u16[0] + 3;
			return true;
	}
	z80->regs.pc += 3;
	return true;
}

INSTR_DEF(jp_hl)
{
	switch (count)
	{
		case 0:
			z80->regs.pc += z80->regs.hl + 1;
			return true;
	}
	z80->regs.pc += 1;
	return true;
}

INSTR_DEF(call_nn)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
			z80->instr_tmp.u16[0] = mem_gu8(z80->mem, z80->regs.pc + 1);
			return false;
		case 2:
			z80->instr_tmp.u16[0] |= mem_gu8(z80->mem, z80->regs.pc + 2) << 8;
			return false;
		case 3:
			z80->instr_tmp.u16[1] = z80->regs.pc + 3;
			return false;
		case 4:
			mem_su8(z80->mem, --z80->regs.sp, z80->instr_tmp.u16[1] >> 8);
			return false;
		case 5:
			mem_su8(z80->mem, --z80->regs.sp, z80->instr_tmp.u16[1]);
			z80->regs.pc = z80->instr_tmp.u16[0];
			return true;
	}
	z80->regs.pc += 3;
	return true;
}

INSTR_DEF(reti)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
			z80->instr_tmp.u16[0] = mem_gu8(z80->mem, z80->regs.sp++);
			return false;
		case 2:
			z80->instr_tmp.u16[0] |= mem_gu8(z80->mem, z80->regs.sp++) << 8;
			return false;
		case 3:
			z80->ime = true;
			z80->regs.pc = z80->instr_tmp.u16[0];
			return true;
	}
	z80->regs.pc += 1;
	return true;
}

INSTR_DEF(ret)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
			z80->instr_tmp.u16[0] = mem_gu8(z80->mem, z80->regs.sp++);
			return false;
		case 2:
			z80->instr_tmp.u16[0] |= mem_gu8(z80->mem, z80->regs.sp++) << 8;
			return false;
		case 3:
			z80->regs.pc = z80->instr_tmp.u16[0];
			return true;
	}
	z80->regs.pc += 1;
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
			return false; \
		case 2: \
			mem_su8(z80->mem, --z80->regs.sp, z80->instr_tmp.u16[1] >> 8); \
			return false; \
		case 3: \
			mem_su8(z80->mem, --z80->regs.sp, z80->instr_tmp.u16[1]); \
			z80->regs.pc = d; \
			return true; \
	} \
	z80->regs.pc += 1; \
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
