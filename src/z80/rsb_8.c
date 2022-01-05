#include "instr.h"
#include "z80.h"

INSTR_DEF(rlca)
{
	(void)count;
	z80->regs.pc++;
	return true;
}

INSTR_DEF(rrca)
{
	(void)count;
	z80->regs.pc++;
	return true;
}

INSTR_DEF(rla)
{
	(void)count;
	z80->regs.pc++;
	return true;
}

INSTR_DEF(rra)
{
	(void)count;
	z80->regs.pc++;
	return true;
}

#define INSTR_DEF_RLC_R(r) \
INSTR_DEF(rlc_##r) \
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

INSTR_DEF_RLC_R(a);
INSTR_DEF_RLC_R(b);
INSTR_DEF_RLC_R(c);
INSTR_DEF_RLC_R(d);
INSTR_DEF_RLC_R(e);
INSTR_DEF_RLC_R(h);
INSTR_DEF_RLC_R(l);

INSTR_DEF(rlc_hl)
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

#define INSTR_DEF_RRC_R(r) \
INSTR_DEF(rrc_##r) \
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

INSTR_DEF_RRC_R(a);
INSTR_DEF_RRC_R(b);
INSTR_DEF_RRC_R(c);
INSTR_DEF_RRC_R(d);
INSTR_DEF_RRC_R(e);
INSTR_DEF_RRC_R(h);
INSTR_DEF_RRC_R(l);

INSTR_DEF(rrc_hl)
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

#define INSTR_DEF_RL_R(r) \
INSTR_DEF(rl_##r) \
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

INSTR_DEF_RL_R(a);
INSTR_DEF_RL_R(b);
INSTR_DEF_RL_R(c);
INSTR_DEF_RL_R(d);
INSTR_DEF_RL_R(e);
INSTR_DEF_RL_R(h);
INSTR_DEF_RL_R(l);

INSTR_DEF(rl_hl)
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

#define INSTR_DEF_RR_R(r) \
INSTR_DEF(rr_##r) \
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

INSTR_DEF_RR_R(a);
INSTR_DEF_RR_R(b);
INSTR_DEF_RR_R(c);
INSTR_DEF_RR_R(d);
INSTR_DEF_RR_R(e);
INSTR_DEF_RR_R(h);
INSTR_DEF_RR_R(l);

INSTR_DEF(rr_hl)
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

#define INSTR_DEF_SLA_R(r) \
INSTR_DEF(sla_##r) \
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

INSTR_DEF_SLA_R(a);
INSTR_DEF_SLA_R(b);
INSTR_DEF_SLA_R(c);
INSTR_DEF_SLA_R(d);
INSTR_DEF_SLA_R(e);
INSTR_DEF_SLA_R(h);
INSTR_DEF_SLA_R(l);

INSTR_DEF(sla_hl)
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

#define INSTR_DEF_SRA_R(r) \
INSTR_DEF(sra_##r) \
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

INSTR_DEF_SRA_R(a);
INSTR_DEF_SRA_R(b);
INSTR_DEF_SRA_R(c);
INSTR_DEF_SRA_R(d);
INSTR_DEF_SRA_R(e);
INSTR_DEF_SRA_R(h);
INSTR_DEF_SRA_R(l);

INSTR_DEF(sra_hl)
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

#define INSTR_DEF_SWAP_R(r) \
INSTR_DEF(swap_##r) \
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

INSTR_DEF_SWAP_R(a);
INSTR_DEF_SWAP_R(b);
INSTR_DEF_SWAP_R(c);
INSTR_DEF_SWAP_R(d);
INSTR_DEF_SWAP_R(e);
INSTR_DEF_SWAP_R(h);
INSTR_DEF_SWAP_R(l);

INSTR_DEF(swap_hl)
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

#define INSTR_DEF_SRL_R(r) \
INSTR_DEF(srl_##r) \
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

INSTR_DEF_SRL_R(a);
INSTR_DEF_SRL_R(b);
INSTR_DEF_SRL_R(c);
INSTR_DEF_SRL_R(d);
INSTR_DEF_SRL_R(e);
INSTR_DEF_SRL_R(h);
INSTR_DEF_SRL_R(l);

INSTR_DEF(srl_hl)
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

#define INSTR_DEF_BIT_N_R(n, r) \
INSTR_DEF(bit_##n##_##r) \
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

#define INSTR_DEF_BIT_N(n) \
INSTR_DEF_BIT_N_R(n, a); \
INSTR_DEF_BIT_N_R(n, b); \
INSTR_DEF_BIT_N_R(n, c); \
INSTR_DEF_BIT_N_R(n, d); \
INSTR_DEF_BIT_N_R(n, e); \
INSTR_DEF_BIT_N_R(n, h); \
INSTR_DEF_BIT_N_R(n, l); \
INSTR_DEF(bit_##n##_hl) \
{ \
	switch (count) \
	{ \
		case 0: \
			return false; \
		case 1: \
			return false; \
		case 2: \
			z80->regs.pc += 2; \
			return true; \
	} \
	return true; \
}

INSTR_DEF_BIT_N(0);
INSTR_DEF_BIT_N(1);
INSTR_DEF_BIT_N(2);
INSTR_DEF_BIT_N(3);
INSTR_DEF_BIT_N(4);
INSTR_DEF_BIT_N(5);
INSTR_DEF_BIT_N(6);
INSTR_DEF_BIT_N(7);

#define INSTR_DEF_RES_N_R(n, r) \
INSTR_DEF(res_##n##_##r) \
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

#define INSTR_DEF_RES_N(n) \
INSTR_DEF_RES_N_R(n, a); \
INSTR_DEF_RES_N_R(n, b); \
INSTR_DEF_RES_N_R(n, c); \
INSTR_DEF_RES_N_R(n, d); \
INSTR_DEF_RES_N_R(n, e); \
INSTR_DEF_RES_N_R(n, h); \
INSTR_DEF_RES_N_R(n, l); \
INSTR_DEF(res_##n##_hl) \
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
			z80->regs.pc += 2; \
			return true; \
	} \
	return true; \
}

INSTR_DEF_RES_N(0);
INSTR_DEF_RES_N(1);
INSTR_DEF_RES_N(2);
INSTR_DEF_RES_N(3);
INSTR_DEF_RES_N(4);
INSTR_DEF_RES_N(5);
INSTR_DEF_RES_N(6);
INSTR_DEF_RES_N(7);

#define INSTR_DEF_SET_N_R(n, r) \
INSTR_DEF(set_##n##_##r) \
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

#define INSTR_DEF_SET_N(n) \
INSTR_DEF_SET_N_R(n, a); \
INSTR_DEF_SET_N_R(n, b); \
INSTR_DEF_SET_N_R(n, c); \
INSTR_DEF_SET_N_R(n, d); \
INSTR_DEF_SET_N_R(n, e); \
INSTR_DEF_SET_N_R(n, h); \
INSTR_DEF_SET_N_R(n, l); \
INSTR_DEF(set_##n##_hl) \
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
			z80->regs.pc += 2; \
			return true; \
	} \
	return true; \
}

INSTR_DEF_SET_N(0);
INSTR_DEF_SET_N(1);
INSTR_DEF_SET_N(2);
INSTR_DEF_SET_N(3);
INSTR_DEF_SET_N(4);
INSTR_DEF_SET_N(5);
INSTR_DEF_SET_N(6);
INSTR_DEF_SET_N(7);
