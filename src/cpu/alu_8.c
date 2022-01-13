#include "instr.h"
#include "cpu.h"
#include "mem.h"

#define INSTR_DEF_INC_R(r) \
INSTR_DEF(inc_##r) \
{ \
	switch (count) \
	{ \
		case 0: \
		{ \
			uint8_t v = cpu->regs.r; \
			cpu->regs.r++; \
			CPU_SET_FLAG_Z(cpu, !cpu->regs.r); \
			CPU_SET_FLAG_N(cpu, 0); \
			CPU_SET_FLAG_H(cpu, (v & 0xf) == 0xf); \
			break; \
		} \
	} \
	cpu->regs.pc += 1; \
	return true; \
}

#define INSTR_DEF_DEC_R(r) \
INSTR_DEF(dec_##r) \
{ \
	switch (count) \
	{ \
		case 0: \
		{ \
			uint8_t v = cpu->regs.r; \
			cpu->regs.r--; \
			CPU_SET_FLAG_Z(cpu, !cpu->regs.r); \
			CPU_SET_FLAG_N(cpu, 1); \
			CPU_SET_FLAG_H(cpu, !(v & 0xf)); \
			break; \
		} \
	} \
	cpu->regs.pc += 1; \
	return true; \
}

#define INSTR_DEF_INC_DEC_R(r) \
INSTR_DEF_INC_R(r) \
INSTR_DEF_DEC_R(r)

INSTR_DEF_INC_DEC_R(a);
INSTR_DEF_INC_DEC_R(b);
INSTR_DEF_INC_DEC_R(c);
INSTR_DEF_INC_DEC_R(d);
INSTR_DEF_INC_DEC_R(e);
INSTR_DEF_INC_DEC_R(h);
INSTR_DEF_INC_DEC_R(l);

#define INSTR_DEF_ADD_A_R(r) \
INSTR_DEF(add_a_##r) \
{ \
	switch (count) \
	{ \
		case 0: \
		{ \
			uint8_t v = cpu->regs.a; \
			uint8_t add = cpu->regs.r; \
			cpu->regs.a += add; \
			CPU_SET_FLAG_Z(cpu, !cpu->regs.a); \
			CPU_SET_FLAG_N(cpu, 0); \
			CPU_SET_FLAG_H(cpu, ((v & 0xf) + (add & 0xf)) & 0x10); \
			CPU_SET_FLAG_C(cpu, v > cpu->regs.a); \
			break; \
		} \
	} \
	cpu->regs.pc += 1; \
	return true; \
}

INSTR_DEF_ADD_A_R(a);
INSTR_DEF_ADD_A_R(b);
INSTR_DEF_ADD_A_R(c);
INSTR_DEF_ADD_A_R(d);
INSTR_DEF_ADD_A_R(e);
INSTR_DEF_ADD_A_R(h);
INSTR_DEF_ADD_A_R(l);

INSTR_DEF(add_a_hl)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
		{
			uint8_t v = cpu->regs.a;
			uint8_t add = mem_get8(cpu->mem, cpu->regs.hl);
			cpu->regs.a += add;
			CPU_SET_FLAG_Z(cpu, !cpu->regs.a);
			CPU_SET_FLAG_N(cpu, 0);
			CPU_SET_FLAG_H(cpu, ((v & 0xf) + (add & 0xf)) & 0x10);
			CPU_SET_FLAG_C(cpu, v > cpu->regs.a);
			break;
		}
	}
	cpu->regs.pc += 1;
	return true;
}

INSTR_DEF(add_a_n)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
		{
			uint8_t v = cpu->regs.a;
			uint8_t add = mem_get8(cpu->mem, cpu->regs.pc + 1);
			cpu->regs.a += add;
			CPU_SET_FLAG_Z(cpu, !cpu->regs.a);
			CPU_SET_FLAG_N(cpu, 0);
			CPU_SET_FLAG_H(cpu, ((v & 0xf) + (add & 0xf)) & 0x10);
			CPU_SET_FLAG_C(cpu, v > cpu->regs.a);
			break;
		}
	}
	cpu->regs.pc += 2;
	return true;
}

#define INSTR_DEF_ADC_A_R(r) \
INSTR_DEF(adc_a_##r) \
{ \
	switch (count) \
	{ \
		case 0: \
		{ \
			uint8_t v = cpu->regs.a; \
			uint8_t add = cpu->regs.r; \
			uint8_t c = CPU_GET_FLAG_C(cpu); \
			cpu->regs.a += add + c; \
			CPU_SET_FLAG_Z(cpu, !cpu->regs.a); \
			CPU_SET_FLAG_N(cpu, 0); \
			CPU_SET_FLAG_H(cpu, ((v & 0xf) + (add & 0xf) + c) & 0x10); \
			CPU_SET_FLAG_C(cpu, v > cpu->regs.a); \
			break; \
		} \
	} \
	cpu->regs.pc += 1; \
	return true; \
}

INSTR_DEF_ADC_A_R(a);
INSTR_DEF_ADC_A_R(b);
INSTR_DEF_ADC_A_R(c);
INSTR_DEF_ADC_A_R(d);
INSTR_DEF_ADC_A_R(e);
INSTR_DEF_ADC_A_R(h);
INSTR_DEF_ADC_A_R(l);

INSTR_DEF(adc_a_hl)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
		{
			uint8_t v = cpu->regs.a;
			uint8_t add = mem_get8(cpu->mem, cpu->regs.hl);
			uint8_t c = CPU_GET_FLAG_C(cpu);
			cpu->regs.a += add + c;
			CPU_SET_FLAG_Z(cpu, !cpu->regs.a);
			CPU_SET_FLAG_N(cpu, 0);
			CPU_SET_FLAG_H(cpu, ((v & 0xf) + (add & 0xf) + c) & 0x10);
			CPU_SET_FLAG_C(cpu, v > cpu->regs.a);
			break;
		}
	}
	cpu->regs.pc += 1;
	return true;
}

INSTR_DEF(adc_a_n)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
		{
			uint8_t v = cpu->regs.a;
			uint8_t add = mem_get8(cpu->mem, cpu->regs.pc + 1);
			uint8_t c = CPU_GET_FLAG_C(cpu);
			cpu->regs.a += add + c;
			CPU_SET_FLAG_Z(cpu, !cpu->regs.a);
			CPU_SET_FLAG_N(cpu, 0);
			CPU_SET_FLAG_H(cpu, ((v & 0xf) + (add & 0xf) + c) & 0x10);
			CPU_SET_FLAG_C(cpu, v > cpu->regs.a);
			break;
		}
	}
	cpu->regs.pc += 2;
	return true;
}

#define INSTR_DEF_SUB_A_R(r) \
INSTR_DEF(sub_a_##r) \
{ \
	switch (count) \
	{ \
		case 0: \
		{ \
			uint8_t v = cpu->regs.a; \
			uint8_t sub = cpu->regs.r; \
			cpu->regs.a -= sub; \
			CPU_SET_FLAG_Z(cpu, !cpu->regs.a); \
			CPU_SET_FLAG_N(cpu, 1); \
			CPU_SET_FLAG_H(cpu, (v & 0xf) < (sub & 0xf)); \
			CPU_SET_FLAG_C(cpu, cpu->regs.a > v); \
			break; \
		} \
	} \
	cpu->regs.pc += 1; \
	return true; \
}

INSTR_DEF_SUB_A_R(a);
INSTR_DEF_SUB_A_R(b);
INSTR_DEF_SUB_A_R(c);
INSTR_DEF_SUB_A_R(d);
INSTR_DEF_SUB_A_R(e);
INSTR_DEF_SUB_A_R(h);
INSTR_DEF_SUB_A_R(l);

INSTR_DEF(sub_a_hl)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
		{
			uint8_t v = cpu->regs.a;
			uint8_t sub = mem_get8(cpu->mem, cpu->regs.hl);
			cpu->regs.a -= sub;
			CPU_SET_FLAG_Z(cpu, !cpu->regs.a);
			CPU_SET_FLAG_N(cpu, 1);
			CPU_SET_FLAG_H(cpu, (v & 0xf) < (sub & 0xf));
			CPU_SET_FLAG_C(cpu, cpu->regs.a > v);
			break;
		}
	}
	cpu->regs.pc += 1;
	return true;
}

INSTR_DEF(sub_a_n)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
		{
			uint8_t v = cpu->regs.a;
			uint8_t sub = mem_get8(cpu->mem, cpu->regs.pc + 1);
			cpu->regs.a -= sub;
			CPU_SET_FLAG_Z(cpu, !cpu->regs.a);
			CPU_SET_FLAG_N(cpu, 1);
			CPU_SET_FLAG_H(cpu, (v & 0xf) < (sub & 0xf));
			CPU_SET_FLAG_C(cpu, cpu->regs.a > v);
			break;
		}
	}
	cpu->regs.pc += 2;
	return true;
}

#define INSTR_DEF_SBC_A_R(r) \
INSTR_DEF(sbc_a_##r) \
{ \
	switch (count) \
	{ \
		case 0: \
		{ \
			uint8_t v = cpu->regs.a; \
			uint8_t sub = cpu->regs.r; \
			uint8_t c = CPU_GET_FLAG_C(cpu); \
			cpu->regs.a -= sub + c; \
			CPU_SET_FLAG_Z(cpu, !cpu->regs.a); \
			CPU_SET_FLAG_N(cpu, 1); \
			CPU_SET_FLAG_H(cpu, (v & 0xf) < (sub & 0xf) + c); \
			CPU_SET_FLAG_C(cpu, cpu->regs.a > v); \
			break; \
		} \
	} \
	cpu->regs.pc += 1; \
	return true; \
}

INSTR_DEF_SBC_A_R(a);
INSTR_DEF_SBC_A_R(b);
INSTR_DEF_SBC_A_R(c);
INSTR_DEF_SBC_A_R(d);
INSTR_DEF_SBC_A_R(e);
INSTR_DEF_SBC_A_R(h);
INSTR_DEF_SBC_A_R(l);

INSTR_DEF(sbc_a_hl)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
		{
			uint8_t v = cpu->regs.a;
			uint8_t sub = mem_get8(cpu->mem, cpu->regs.hl);
			uint8_t c = CPU_GET_FLAG_C(cpu);
			cpu->regs.a -= sub + c;
			CPU_SET_FLAG_Z(cpu, !cpu->regs.a);
			CPU_SET_FLAG_N(cpu, 1);
			CPU_SET_FLAG_H(cpu, (v & 0xf) < (sub & 0xf) + c);
			CPU_SET_FLAG_C(cpu, cpu->regs.a > v);
			break;
		}
	}
	cpu->regs.pc += 1;
	return true;
}

INSTR_DEF(sbc_a_n)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
		{
			uint8_t v = cpu->regs.a;
			uint8_t sub = mem_get8(cpu->mem, cpu->regs.pc + 1);
			uint8_t c = CPU_GET_FLAG_C(cpu);
			cpu->regs.a -= sub + c;
			CPU_SET_FLAG_Z(cpu, !cpu->regs.a);
			CPU_SET_FLAG_N(cpu, 1);
			CPU_SET_FLAG_H(cpu, (v & 0xf) < (sub & 0xf) + c);
			CPU_SET_FLAG_C(cpu, cpu->regs.a > v);
			break;
		}
	}
	cpu->regs.pc += 2;
	return true;
}

#define INSTR_DEF_AND_A_R(r) \
INSTR_DEF(and_a_##r) \
{ \
	switch (count) \
	{ \
		case 0: \
			cpu->regs.a &= cpu->regs.r; \
			CPU_SET_FLAG_Z(cpu, !cpu->regs.a); \
			CPU_SET_FLAG_N(cpu, 0); \
			CPU_SET_FLAG_H(cpu, 1); \
			CPU_SET_FLAG_C(cpu, 0); \
			break; \
	} \
	cpu->regs.pc += 1; \
	return true; \
}

INSTR_DEF_AND_A_R(a);
INSTR_DEF_AND_A_R(b);
INSTR_DEF_AND_A_R(c);
INSTR_DEF_AND_A_R(d);
INSTR_DEF_AND_A_R(e);
INSTR_DEF_AND_A_R(h);
INSTR_DEF_AND_A_R(l);

INSTR_DEF(and_a_hl)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
			cpu->regs.a &= mem_get8(cpu->mem, cpu->regs.hl);
			CPU_SET_FLAG_Z(cpu, !cpu->regs.a);
			CPU_SET_FLAG_N(cpu, 0);
			CPU_SET_FLAG_H(cpu, 1);
			CPU_SET_FLAG_C(cpu, 0);
			break;
	}
	cpu->regs.pc += 1;
	return true;
}

INSTR_DEF(and_a_n)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
			cpu->regs.a &= mem_get8(cpu->mem, cpu->regs.pc + 1);
			CPU_SET_FLAG_Z(cpu, !cpu->regs.a);
			CPU_SET_FLAG_N(cpu, 0);
			CPU_SET_FLAG_H(cpu, 1);
			CPU_SET_FLAG_C(cpu, 0);
			break;
	}
	cpu->regs.pc += 2;
	return true;
}

#define INSTR_DEF_XOR_A_R(r) \
INSTR_DEF(xor_a_##r) \
{ \
	switch (count) \
	{ \
		case 0: \
			cpu->regs.a ^= cpu->regs.r; \
			CPU_SET_FLAG_Z(cpu, !cpu->regs.a); \
			CPU_SET_FLAG_N(cpu, 0); \
			CPU_SET_FLAG_H(cpu, 0); \
			CPU_SET_FLAG_C(cpu, 0); \
			break; \
	} \
	cpu->regs.pc += 1; \
	return true; \
}

INSTR_DEF_XOR_A_R(a);
INSTR_DEF_XOR_A_R(b);
INSTR_DEF_XOR_A_R(c);
INSTR_DEF_XOR_A_R(d);
INSTR_DEF_XOR_A_R(e);
INSTR_DEF_XOR_A_R(h);
INSTR_DEF_XOR_A_R(l);

INSTR_DEF(xor_a_hl)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
			cpu->regs.a ^= mem_get8(cpu->mem, cpu->regs.hl);
			CPU_SET_FLAG_Z(cpu, !cpu->regs.a);
			CPU_SET_FLAG_N(cpu, 0);
			CPU_SET_FLAG_H(cpu, 0);
			CPU_SET_FLAG_C(cpu, 0);
			break;
	}
	cpu->regs.pc += 1;
	return true;
}

INSTR_DEF(xor_a_n)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
			cpu->regs.a ^= mem_get8(cpu->mem, cpu->regs.pc + 1);
			CPU_SET_FLAG_Z(cpu, !cpu->regs.a);
			CPU_SET_FLAG_N(cpu, 0);
			CPU_SET_FLAG_H(cpu, 0);
			CPU_SET_FLAG_C(cpu, 0);
			break;
	}
	cpu->regs.pc += 2;
	return true;
}

#define INSTR_DEF_OR_A_R(r) \
INSTR_DEF(or_a_##r) \
{ \
	switch (count) \
	{ \
		case 0: \
			cpu->regs.a |= cpu->regs.r; \
			CPU_SET_FLAG_Z(cpu, !cpu->regs.a); \
			CPU_SET_FLAG_N(cpu, 0); \
			CPU_SET_FLAG_H(cpu, 0); \
			CPU_SET_FLAG_C(cpu, 0); \
			break; \
	} \
	cpu->regs.pc += 1; \
	return true; \
}

INSTR_DEF_OR_A_R(a);
INSTR_DEF_OR_A_R(b);
INSTR_DEF_OR_A_R(c);
INSTR_DEF_OR_A_R(d);
INSTR_DEF_OR_A_R(e);
INSTR_DEF_OR_A_R(h);
INSTR_DEF_OR_A_R(l);

INSTR_DEF(or_a_hl)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
			cpu->regs.a |= mem_get8(cpu->mem, cpu->regs.hl);
			CPU_SET_FLAG_Z(cpu, !cpu->regs.a);
			CPU_SET_FLAG_N(cpu, 0);
			CPU_SET_FLAG_H(cpu, 0);
			CPU_SET_FLAG_C(cpu, 0);
			break;
	}
	cpu->regs.pc += 1;
	return true;
}

INSTR_DEF(or_a_n)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
			cpu->regs.a |= mem_get8(cpu->mem, cpu->regs.pc + 1);
			CPU_SET_FLAG_Z(cpu, !cpu->regs.a);
			CPU_SET_FLAG_N(cpu, 0);
			CPU_SET_FLAG_H(cpu, 0);
			CPU_SET_FLAG_C(cpu, 0);
			break;
	}
	cpu->regs.pc += 2;
	return true;
}

#define INSTR_DEF_CP_A_R(r) \
INSTR_DEF(cp_a_##r) \
{ \
	switch (count) \
	{ \
		case 0: \
		{ \
			uint8_t v = cpu->regs.r; \
			CPU_SET_FLAG_Z(cpu, cpu->regs.a == v); \
			CPU_SET_FLAG_N(cpu, 1); \
			CPU_SET_FLAG_H(cpu, (cpu->regs.a & 0xF) < (v & 0xF)); \
			CPU_SET_FLAG_C(cpu, cpu->regs.a < v); \
			break; \
		} \
	} \
	cpu->regs.pc += 1; \
	return true; \
}

INSTR_DEF_CP_A_R(a);
INSTR_DEF_CP_A_R(b);
INSTR_DEF_CP_A_R(c);
INSTR_DEF_CP_A_R(d);
INSTR_DEF_CP_A_R(e);
INSTR_DEF_CP_A_R(h);
INSTR_DEF_CP_A_R(l);

INSTR_DEF(cp_a_hl)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
		{
			uint8_t v = mem_get8(cpu->mem, cpu->regs.hl);
			CPU_SET_FLAG_Z(cpu, cpu->regs.a == v);
			CPU_SET_FLAG_N(cpu, 1);
			CPU_SET_FLAG_H(cpu, (cpu->regs.a & 0xf) < (v & 0xf));
			CPU_SET_FLAG_C(cpu, cpu->regs.a < v);
			break;
		}
	}
	cpu->regs.pc += 1;
	return true;
}

INSTR_DEF(cp_a_n)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
		{
			uint8_t v = mem_get8(cpu->mem, cpu->regs.pc + 1);
			CPU_SET_FLAG_Z(cpu, cpu->regs.a == v);
			CPU_SET_FLAG_N(cpu, 1);
			CPU_SET_FLAG_H(cpu, (cpu->regs.a & 0xf) < (v & 0xf));
			CPU_SET_FLAG_C(cpu, cpu->regs.a < v);
			break;
		}
	}
	cpu->regs.pc += 2;
	return true;
}

INSTR_DEF(inc_rhl)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
			cpu->instr_tmp.u8[0] = mem_get8(cpu->mem, cpu->regs.hl);
			return false;
		case 2:
		{
			uint8_t v = cpu->instr_tmp.u8[0] + 1;
			mem_set8(cpu->mem, cpu->regs.hl, v);
			CPU_SET_FLAG_Z(cpu, !v);
			CPU_SET_FLAG_N(cpu, 0);
			CPU_SET_FLAG_H(cpu, !(v & 0xf));
			break;
		}
	}
	cpu->regs.pc += 1;
	return true;
}

INSTR_DEF(dec_rhl)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
			cpu->instr_tmp.u8[0] = mem_get8(cpu->mem, cpu->regs.hl);
			return false;
		case 2:
		{
			uint8_t v = cpu->instr_tmp.u8[0] - 1;
			mem_set8(cpu->mem, cpu->regs.hl, v);
			CPU_SET_FLAG_Z(cpu, !v);
			CPU_SET_FLAG_N(cpu, 1);
			CPU_SET_FLAG_H(cpu, (v & 0xf) == 0xf);
			break;
		}
	}
	cpu->regs.pc += 1;
	return true;
}

INSTR_DEF(daa)
{
	switch (count)
	{
		case 0:
			CPU_SET_FLAG_H(cpu, 0);
			break;
	}
	cpu->regs.pc += 1;
	return true;
}

INSTR_DEF(cpl)
{
	switch (count)
	{
		case 0:
			cpu->regs.a ^= 0xFF;
			CPU_SET_FLAG_N(cpu, 1);
			CPU_SET_FLAG_H(cpu, 1);
			break;
	}
	cpu->regs.pc += 1;
	return true;
}

INSTR_DEF(scf)
{
	switch (count)
	{
		case 0:
			CPU_SET_FLAG_N(cpu, 0);
			CPU_SET_FLAG_H(cpu, 0);
			CPU_SET_FLAG_C(cpu, 1);
			break;
	}
	cpu->regs.pc += 1;
	return true;
}

INSTR_DEF(ccf)
{
	switch (count)
	{
		case 0:
			CPU_SET_FLAG_N(cpu, 0);
			CPU_SET_FLAG_H(cpu, 0);
			CPU_SET_FLAG_C(cpu, !CPU_GET_FLAG_C(cpu));
			break;
	}
	cpu->regs.pc += 1;
	return true;
}
