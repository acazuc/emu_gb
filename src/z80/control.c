#include "instr.h"
#include "z80.h"

INSTR_DEF(jr_dd)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
			z80->instr_tmp.i8[0] = mem_get_i8(z80->mem, z80->regs.pc + 1);
			return false;
		case 2:
			z80->regs.pc += z80->instr_tmp.i8[0] + 2;
			return true;
	}
	return true;
}

INSTR_DEF(jr_nz_n)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
			z80->instr_tmp.i8[0] = mem_get_i8(z80->mem, z80->regs.pc + 1);
			if (Z80_HAS_FLAG_Z(z80))
			{
				z80->regs.pc += 2;
				return true;
			}
			return false;
		case 2:
			z80->regs.pc += z80->instr_tmp.i8[0] + 2;
			return true;
	}
	return true;
}

INSTR_DEF(jr_z_n)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
			z80->instr_tmp.i8[0] = mem_get_i8(z80->mem, z80->regs.pc + 1);
			if (!Z80_HAS_FLAG_Z(z80))
			{
				z80->regs.pc += 2;
				return true;
			}
			return false;
		case 2:
			z80->regs.pc += z80->instr_tmp.i8[0] + 2;
			return true;
	}
	return true;
}

INSTR_DEF(jr_nc_n)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
			z80->instr_tmp.i8[0] = mem_get_i8(z80->mem, z80->regs.pc + 1);
			if (Z80_HAS_FLAG_C(z80))
			{
				z80->regs.pc += 2;
				return true;
			}
			return false;
		case 2:
			z80->regs.pc += z80->instr_tmp.i8[0] + 2;
			return true;
	}
	return true;
}

INSTR_DEF(jr_c_n)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
			z80->instr_tmp.i8[0] = mem_get_i8(z80->mem, z80->regs.pc + 1);
			if (Z80_HAS_FLAG_C(z80))
			{
				z80->regs.pc += 2;
				return true;
			}
			return false;
		case 2:
			z80->regs.pc += z80->instr_tmp.i8[0] + 2;
			return true;
	}
	return true;
}

#define INSTR_DEF_JP_CALL_RET_FLAG(f) \
INSTR_DEF(jp_##f##_nn) \
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
} \
INSTR_DEF(jp_n##f##_nn) \
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
} \
INSTR_DEF(call_##f##_nn) \
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
} \
INSTR_DEF(call_n##f##_nn) \
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
} \
INSTR_DEF(ret_##f) \
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
} \
INSTR_DEF(ret_n##f) \
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

INSTR_DEF_JP_CALL_RET_FLAG(z);
INSTR_DEF_JP_CALL_RET_FLAG(c);

INSTR_DEF(jp_nn)
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
			z80->regs.pc += 3;
			return true;
	}
	return true;
}

INSTR_DEF(jp_hl)
{
	switch (count)
	{
		case 0:
			z80->regs.pc += 1;
			return true;
	}
	return true;
}

INSTR_DEF(call_nn)
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
			return false;
		case 5:
			z80->regs.pc += 3;
			return true;
	}
	return true;
}

INSTR_DEF(reti)
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
			z80->regs.pc += 1;
			return true;
	}
	return true;
}

INSTR_DEF(ret)
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
			z80->regs.pc += 1;
			return true;
	}
	return true;
}
#define INSTR_RST(n) \
INSTR_DEF(rst_##n) \
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
}

INSTR_RST(00);
INSTR_RST(08);
INSTR_RST(10);
INSTR_RST(18);
INSTR_RST(20);
INSTR_RST(28);
INSTR_RST(30);
INSTR_RST(38);
