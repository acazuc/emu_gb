#include "instr.h"
#include "z80.h"

#define INSTR_DEF_LD_RR_A(rr) \
INSTR_DEF(ld_##rr##_a) \
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

INSTR_DEF_LD_RR_A(bc);
INSTR_DEF_LD_RR_A(de);
INSTR_DEF_LD_RR_A(sp);

#define INSTR_DEF_LD_A_RR(rr) \
INSTR_DEF(ld_a_##rr) \
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

INSTR_DEF_LD_A_RR(bc);
INSTR_DEF_LD_A_RR(de);
INSTR_DEF_LD_A_RR(sp);

#define INSTR_DEF_LD_R_N(r) \
INSTR_DEF(ld_##r##_n) \
{ \
	switch (count) \
	{ \
		case 0: \
			return false; \
		case 1: \
			z80->regs.pc += 2; \
			return true; \
	} \
	return true; \
}

INSTR_DEF_LD_R_N(a);
INSTR_DEF_LD_R_N(b);
INSTR_DEF_LD_R_N(c);
INSTR_DEF_LD_R_N(d);
INSTR_DEF_LD_R_N(e);
INSTR_DEF_LD_R_N(h);
INSTR_DEF_LD_R_N(l);

#define INSTR_DEF_LD_R_R(r1, r2) \
INSTR_DEF(ld_##r1##_##r2) \
{ \
	switch (count) \
	{ \
		case 0: \
			z80->regs.pc += 1; \
			return true; \
	} \
	return true; \
}

#define INSTR_DEF_LD_R(r) \
INSTR_DEF_LD_R_R(r, a) \
INSTR_DEF_LD_R_R(r, b) \
INSTR_DEF_LD_R_R(r, c) \
INSTR_DEF_LD_R_R(r, d) \
INSTR_DEF_LD_R_R(r, e) \
INSTR_DEF_LD_R_R(r, h) \
INSTR_DEF_LD_R_R(r, l) \
INSTR_DEF(ld_##r##_hl) \
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
INSTR_DEF(ld_hl_##r) \
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

INSTR_DEF_LD_R(a);
INSTR_DEF_LD_R(b);
INSTR_DEF_LD_R(c);
INSTR_DEF_LD_R(d);
INSTR_DEF_LD_R(e);
INSTR_DEF_LD_R(h);
INSTR_DEF_LD_R(l);

INSTR_DEF(ldi_hl_a)
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

INSTR_DEF(ldi_a_hl)
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

INSTR_DEF(ldd_hl_a)
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

INSTR_DEF(ldd_a_hl)
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

INSTR_DEF(ld_rhl_n)
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

INSTR_DEF(ld_ffn_a)
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

INSTR_DEF(ld_ffc_a)
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

INSTR_DEF(ld_a_ffn)
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

INSTR_DEF(ld_a_ffc)
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

INSTR_DEF(ld_a_nn)
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

INSTR_DEF(ld_nn_a)
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
