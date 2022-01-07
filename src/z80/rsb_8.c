#include "instr.h"
#include "z80.h"

INSTR_DEF(rlca)
{
	switch (count)
	{
		case 0:
		{
			uint8_t v = 0;
			Z80_SET_FLAG_Z(z80, 0);
			Z80_SET_FLAG_N(z80, 0);
			Z80_SET_FLAG_H(z80, 0);
			z80_update_cflag(z80, v, 0);
			break;
		}
	}
	z80->regs.pc++;
	return true;
}

INSTR_DEF(rrca)
{
	switch (count)
	{
		case 0:
		{
			uint8_t v = 0;
			Z80_SET_FLAG_Z(z80, 0);
			Z80_SET_FLAG_N(z80, 0);
			Z80_SET_FLAG_H(z80, 0);
			z80_update_cflag(z80, v, 0);
			break;
		}
	}
	z80->regs.pc++;
	return true;
}

INSTR_DEF(rla)
{
	switch (count)
	{
		case 0:
		{
			uint8_t v = 0;
			Z80_SET_FLAG_Z(z80, 0);
			Z80_SET_FLAG_N(z80, 0);
			Z80_SET_FLAG_H(z80, 0);
			z80_update_cflag(z80, v, 0);
			break;
		}
	}
	z80->regs.pc++;
	return true;
}

INSTR_DEF(rra)
{
	switch (count)
	{
		case 0:
		{
			uint8_t v = 0;
			Z80_SET_FLAG_Z(z80, 0);
			Z80_SET_FLAG_N(z80, 0);
			Z80_SET_FLAG_H(z80, 0);
			z80_update_cflag(z80, v, 0);
			break;
		}
	}
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
		{ \
			uint8_t v = z80->regs.r; \
			uint8_t newv = (v << 1) | (v >> 7); \
			z80->regs.r = newv; \
			Z80_SET_FLAG_Z(z80, !(newv & 0xFE)); \
			Z80_SET_FLAG_N(z80, 0); \
			Z80_SET_FLAG_H(z80, 0); \
			Z80_SET_FLAG_C(z80, v & 0x80); \
			break; \
		} \
	} \
	z80->regs.pc += 2; \
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
			z80->instr_tmp.u8[0] = mem_gu8(z80->mem, z80->regs.hl);
			return false;
		case 3:
		{
			uint8_t v = z80->instr_tmp.u8[0];
			uint8_t newv = (v << 1) | (v >> 7);
			mem_su8(z80->mem, z80->regs.hl, newv);
			Z80_SET_FLAG_Z(z80, !(newv & 0xFE));
			Z80_SET_FLAG_N(z80, 0);
			Z80_SET_FLAG_H(z80, 0);
			Z80_SET_FLAG_C(z80, v & 0x80);
			break;
		}
	}
	z80->regs.pc += 2;
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
		{ \
			uint8_t v = z80->regs.r; \
			uint8_t newv = (v >> 1) | (v << 7); \
			z80->regs.r = newv; \
			Z80_SET_FLAG_Z(z80, !newv); \
			Z80_SET_FLAG_N(z80, 0); \
			Z80_SET_FLAG_H(z80, 0); \
			Z80_SET_FLAG_C(z80, v & 0x1); \
			break; \
		} \
	} \
	z80->regs.pc += 2; \
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
			z80->instr_tmp.u8[0] = mem_gu8(z80->mem, z80->regs.hl);
			return false;
		case 3:
		{
			uint8_t v = z80->instr_tmp.u8[0];
			uint8_t newv = (v >> 1) | (v << 7);
			mem_su8(z80->mem, z80->regs.hl, newv);
			Z80_SET_FLAG_Z(z80, !newv);
			Z80_SET_FLAG_N(z80, 0);
			Z80_SET_FLAG_H(z80, 0);
			Z80_SET_FLAG_C(z80, v & 0x1);
			break;
		}
	}
	z80->regs.pc += 2;
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
		{ \
			uint8_t v = z80->regs.r; \
			uint8_t newv = (v << 1) | Z80_GET_FLAG_C(z80); \
			z80->regs.r = newv; \
			Z80_SET_FLAG_Z(z80, !newv); \
			Z80_SET_FLAG_N(z80, 0); \
			Z80_SET_FLAG_H(z80, 0); \
			Z80_SET_FLAG_C(z80, v & 0x80); \
			break; \
		} \
	} \
	z80->regs.pc += 2; \
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
			z80->instr_tmp.u8[0] = mem_gu8(z80->mem, z80->regs.hl);
			return false;
		case 3:
		{
			uint8_t v = z80->instr_tmp.u8[0];
			uint8_t newv = (v << 1) | Z80_GET_FLAG_C(z80);
			mem_su8(z80->mem, z80->regs.hl, newv);
			Z80_SET_FLAG_Z(z80, !newv);
			Z80_SET_FLAG_N(z80, 0);
			Z80_SET_FLAG_H(z80, 0);
			Z80_SET_FLAG_C(z80, v & 0x80);
			break;
		}
	}
	z80->regs.pc += 2;
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
		{ \
			uint8_t v = z80->regs.r; \
			uint8_t newv = (v >> 1) | (Z80_GET_FLAG_C(z80) << 7); \
			z80->regs.r = newv; \
			Z80_SET_FLAG_Z(z80, !newv); \
			Z80_SET_FLAG_N(z80, 0); \
			Z80_SET_FLAG_H(z80, 0); \
			Z80_SET_FLAG_C(z80, v & 0x1); \
			break; \
		} \
	} \
	z80->regs.pc += 2; \
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
			z80->instr_tmp.u8[0] = mem_gu8(z80->mem, z80->regs.hl);
			return false;
		case 3:
		{
			uint8_t v = z80->instr_tmp.u8[0];
			uint8_t newv = (v >> 1) | (Z80_GET_FLAG_C(z80) << 7);
			mem_su8(z80->mem, z80->regs.hl, newv);
			Z80_SET_FLAG_Z(z80, !newv);
			Z80_SET_FLAG_N(z80, 0);
			Z80_SET_FLAG_H(z80, 0);
			Z80_SET_FLAG_C(z80, v & 0x1);
			break;
		}
	}
	z80->regs.pc += 2;
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
		{ \
			uint8_t v = z80->regs.r; \
			uint8_t newv = v << 1; \
			z80->regs.r = newv; \
			Z80_SET_FLAG_Z(z80, !newv); \
			Z80_SET_FLAG_N(z80, 0); \
			Z80_SET_FLAG_H(z80, 0); \
			Z80_SET_FLAG_C(z80, v & 0x80); \
			break; \
		} \
	} \
	z80->regs.pc += 2; \
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
			z80->instr_tmp.u8[0] = mem_gu8(z80->mem, z80->regs.hl);
			return false;
		case 3:
		{
			uint8_t v = z80->instr_tmp.u8[0];
			uint8_t newv = v << 1;
			mem_su8(z80->mem, z80->regs.hl, newv);
			Z80_SET_FLAG_Z(z80, !newv);
			Z80_SET_FLAG_N(z80, 0);
			Z80_SET_FLAG_H(z80, 0);
			Z80_SET_FLAG_C(z80, v & 0x80);
			break;
		}
	}
	z80->regs.pc += 2;
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
		{ \
			uint8_t v = z80->regs.r; \
			uint8_t newv = (v >> 1) | (v & 0x80); \
			z80->regs.r = newv; \
			Z80_SET_FLAG_Z(z80, !newv); \
			Z80_SET_FLAG_N(z80, 0); \
			Z80_SET_FLAG_H(z80, 0); \
			Z80_SET_FLAG_C(z80, v & 0x1); \
			break; \
		} \
	} \
	z80->regs.pc += 2; \
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
			z80->instr_tmp.u8[0] = mem_gu8(z80->mem, z80->regs.hl);
			return false;
		case 3:
		{
			uint8_t v = z80->instr_tmp.u8[0];
			uint8_t newv = (v >> 1) | (v & 0x80);;
			mem_su8(z80->mem, z80->regs.hl, newv);
			Z80_SET_FLAG_Z(z80, !newv);
			Z80_SET_FLAG_N(z80, 0);
			Z80_SET_FLAG_H(z80, 0);
			Z80_SET_FLAG_C(z80, v & 0x1);
			break;
		}
	}
	z80->regs.pc += 2;
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
			z80->regs.r = (z80->regs.r >> 4) | (z80->regs.r << 4); \
			Z80_SET_FLAG_Z(z80, !z80->regs.r); \
			Z80_SET_FLAG_N(z80, 0); \
			Z80_SET_FLAG_H(z80, 0); \
			Z80_SET_FLAG_C(z80, 0); \
			break; \
	} \
	z80->regs.pc += 2; \
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
			z80->instr_tmp.u8[0] = mem_gu8(z80->mem, z80->regs.hl);
			return false;
		case 3:
		{
			uint8_t v = z80->instr_tmp.u8[0];
			uint8_t newv = (v >> 4) | (v << 4);
			mem_su8(z80->mem, z80->regs.hl, newv);
			Z80_SET_FLAG_Z(z80, !newv);
			Z80_SET_FLAG_N(z80, 0);
			Z80_SET_FLAG_H(z80, 0);
			Z80_SET_FLAG_C(z80, 0);
			break;
		}
	}
	z80->regs.pc += 2;
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
		{ \
			uint8_t v = z80->regs.r; \
			uint8_t newv = v >> 1; \
			z80->regs.r = newv; \
			Z80_SET_FLAG_Z(z80, !newv); \
			Z80_SET_FLAG_N(z80, 0); \
			Z80_SET_FLAG_H(z80, 0); \
			Z80_SET_FLAG_C(z80, v & 0x1); \
			break; \
		} \
	} \
	z80->regs.pc += 2; \
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
			z80->instr_tmp.u8[0] = mem_gu8(z80->mem, z80->regs.hl);
			return false;
		case 3:
		{
			uint8_t v = z80->instr_tmp.u8[0];
			uint8_t newv = v >> 1;
			mem_su8(z80->mem, z80->regs.hl, newv);
			Z80_SET_FLAG_Z(z80, !newv);
			Z80_SET_FLAG_N(z80, 0);
			Z80_SET_FLAG_H(z80, 0);
			Z80_SET_FLAG_C(z80, v & 0x1);
			break;
		}
	}
	z80->regs.pc += 2;
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
			Z80_SET_FLAG_Z(z80, !(z80->regs.r & (1 << n))); \
			Z80_SET_FLAG_N(z80, 0); \
			Z80_SET_FLAG_H(z80, 1); \
			break; \
	} \
	z80->regs.pc += 2; \
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
			Z80_SET_FLAG_Z(z80, !(mem_gu8(z80->mem, z80->regs.hl) & (1 << n))); \
			Z80_SET_FLAG_N(z80, 0); \
			Z80_SET_FLAG_H(z80, 1); \
			break; \
	} \
	z80->regs.pc += 2; \
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
			z80->regs.r &= ~(1 << n); \
			break; \
	} \
	z80->regs.pc += 2; \
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
			z80->instr_tmp.u8[0] = mem_gu8(z80->mem, z80->regs.hl); \
			return false; \
		case 3: \
			mem_su8(z80->mem, z80->regs.hl, z80->instr_tmp.u8[0] & ~(1 << n)); \
			break; \
	} \
	z80->regs.pc += 2; \
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
			z80->regs.r |= 1 << n; \
			break; \
	} \
	z80->regs.pc += 2; \
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
			z80->instr_tmp.u8[0] = mem_gu8(z80->mem, z80->regs.hl); \
			return false; \
		case 3: \
			mem_su8(z80->mem, z80->regs.hl, z80->instr_tmp.u8[0] | (1 << n)); \
			break; \
	} \
	z80->regs.pc += 2; \
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
