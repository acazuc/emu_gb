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
			cpu_update_hflag8(cpu, v, cpu->regs.r, 1); \
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
			cpu_update_hflag8(cpu, v, cpu->regs.r, 0); \
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
			cpu->regs.a += cpu->regs.r; \
			CPU_SET_FLAG_Z(cpu, !cpu->regs.a); \
			CPU_SET_FLAG_N(cpu, 0); \
			cpu_update_hflag8(cpu, v, cpu->regs.a, 1); \
			cpu_update_cflag8(cpu, v, cpu->regs.a, 1); \
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
			cpu->regs.a += mem_get8(cpu->mem, cpu->regs.hl);
			CPU_SET_FLAG_Z(cpu, !cpu->regs.a);
			CPU_SET_FLAG_N(cpu, 0);
			cpu_update_hflag8(cpu, v, cpu->regs.a, 1);
			cpu_update_cflag8(cpu, v, cpu->regs.a, 1);
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
			cpu->regs.a += mem_get8(cpu->mem, cpu->regs.pc + 1);
			CPU_SET_FLAG_Z(cpu, !cpu->regs.a);
			CPU_SET_FLAG_N(cpu, 0);
			cpu_update_hflag8(cpu, v, cpu->regs.a, 1);
			cpu_update_cflag8(cpu, v, cpu->regs.a, 1);
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
			cpu->regs.a += cpu->regs.r + CPU_GET_FLAG_C(cpu); \
			CPU_SET_FLAG_Z(cpu, !cpu->regs.a); \
			CPU_SET_FLAG_N(cpu, 0); \
			cpu_update_hflag8(cpu, v, cpu->regs.a, 1); \
			cpu_update_cflag8(cpu, v, cpu->regs.a, 1); \
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
			cpu->regs.a += mem_get8(cpu->mem, cpu->regs.hl) + CPU_GET_FLAG_C(cpu);
			CPU_SET_FLAG_Z(cpu, !cpu->regs.a);
			CPU_SET_FLAG_N(cpu, 0);
			cpu_update_hflag8(cpu, v, cpu->regs.a, 1);
			cpu_update_cflag8(cpu, v, cpu->regs.a, 1);
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
			cpu->regs.a += mem_get8(cpu->mem, cpu->regs.pc + 1) + CPU_GET_FLAG_C(cpu);
			CPU_SET_FLAG_Z(cpu, !cpu->regs.a);
			CPU_SET_FLAG_N(cpu, 0);
			cpu_update_hflag8(cpu, v, cpu->regs.a, 1);
			cpu_update_cflag8(cpu, v, cpu->regs.a, 1);
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
			cpu->regs.a -= cpu->regs.r; \
			CPU_SET_FLAG_Z(cpu, !cpu->regs.a); \
			CPU_SET_FLAG_N(cpu, 1); \
			cpu_update_hflag8(cpu, v, cpu->regs.a, 0); \
			cpu_update_cflag8(cpu, v, cpu->regs.a, 0); \
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
			cpu->regs.a -= mem_get8(cpu->mem, cpu->regs.hl);
			CPU_SET_FLAG_Z(cpu, !cpu->regs.a);
			CPU_SET_FLAG_N(cpu, 1);
			cpu_update_hflag8(cpu, v, cpu->regs.a, 0);
			cpu_update_cflag8(cpu, v, cpu->regs.a, 0);
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
			cpu->regs.a -= mem_get8(cpu->mem, cpu->regs.pc + 1);
			CPU_SET_FLAG_Z(cpu, !cpu->regs.a);
			CPU_SET_FLAG_N(cpu, 1);
			cpu_update_hflag8(cpu, v, cpu->regs.a, 0);
			cpu_update_cflag8(cpu, v, cpu->regs.a, 0);
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
			cpu->regs.a -= cpu->regs.r - CPU_GET_FLAG_C(cpu); \
			CPU_SET_FLAG_Z(cpu, !cpu->regs.a); \
			CPU_SET_FLAG_N(cpu, 1); \
			cpu_update_hflag8(cpu, v, cpu->regs.a, 0); \
			cpu_update_cflag8(cpu, v, cpu->regs.a, 0); \
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
			cpu->regs.a -= mem_get8(cpu->mem, cpu->regs.hl) - CPU_GET_FLAG_C(cpu);
			CPU_SET_FLAG_Z(cpu, !cpu->regs.a);
			CPU_SET_FLAG_N(cpu, 1);
			cpu_update_hflag8(cpu, v, cpu->regs.a, 0);
			cpu_update_cflag8(cpu, v, cpu->regs.a, 0);
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
			cpu->regs.a -= mem_get8(cpu->mem, cpu->regs.pc + 1) - CPU_GET_FLAG_C(cpu);
			CPU_SET_FLAG_Z(cpu, !cpu->regs.a);
			CPU_SET_FLAG_N(cpu, 1);
			cpu_update_hflag8(cpu, v, cpu->regs.a, 0);
			cpu_update_cflag8(cpu, v, cpu->regs.a, 0);
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
			uint8_t v = cpu->regs.a - cpu->regs.r; \
			CPU_SET_FLAG_Z(cpu, !v); \
			CPU_SET_FLAG_N(cpu, 1); \
			cpu_update_hflag8(cpu, cpu->regs.a, v, 0); \
			cpu_update_cflag8(cpu, cpu->regs.a, v, 0); \
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
			uint8_t v = cpu->regs.a - mem_get8(cpu->mem, cpu->regs.hl);
			CPU_SET_FLAG_Z(cpu, !v);
			CPU_SET_FLAG_N(cpu, 1);
			cpu_update_hflag8(cpu, cpu->regs.a, v, 0);
			cpu_update_cflag8(cpu, cpu->regs.a, v, 0);
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
			uint8_t v = cpu->regs.a - mem_get8(cpu->mem, cpu->regs.pc + 1);
			CPU_SET_FLAG_Z(cpu, !v);
			CPU_SET_FLAG_N(cpu, 1);
			cpu_update_hflag8(cpu, cpu->regs.a, v, 0);
			cpu_update_cflag8(cpu, cpu->regs.a, v, 0);
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
			CPU_SET_FLAG_Z(cpu, !(v | (v << 8)));
			CPU_SET_FLAG_N(cpu, 0);
			cpu_update_hflag8(cpu, cpu->instr_tmp.u8[0], v, 1);
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
			CPU_SET_FLAG_Z(cpu, !(v | (v << 8)));
			CPU_SET_FLAG_N(cpu, 1);
			cpu_update_hflag8(cpu, cpu->instr_tmp.u8[0], v, 0);
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
