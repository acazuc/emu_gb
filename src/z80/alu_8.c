#include "instr.h"
#include "z80.h"

#define INSTR_DEF_INC_R(r) \
INSTR_DEF(inc_##r) \
{ \
	switch (count) \
	{ \
		case 0: \
			z80->regs.pc += 1; \
			return true; \
	} \
	return true; \
}

#define INSTR_DEF_DEC_R(r) \
INSTR_DEF(dec_##r) \
{ \
	switch (count) \
	{ \
		case 0: \
			z80->regs.pc += 1; \
			return true; \
	} \
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
			z80->regs.pc += 1; \
			return true; \
	} \
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
			z80->regs.pc += 1;
			return true;
	}
	return true;
}

INSTR_DEF(add_a_n)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
			z80->regs.pc += 2;
			return true;
	}
	return true;
}

#define INSTR_DEF_ADC_A_R(r) \
INSTR_DEF(adc_a_##r) \
{ \
	switch (count) \
	{ \
		case 0: \
			z80->regs.pc += 1; \
			return true; \
	} \
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
			z80->regs.pc += 1;
			return true;
	}
	return true;
}

INSTR_DEF(adc_a_n)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
			z80->regs.pc += 2;
			return true;
	}
	return true;
}

#define INSTR_DEF_SUB_A_R(r) \
INSTR_DEF(sub_a_##r) \
{ \
	switch (count) \
	{ \
		case 0: \
			z80->regs.pc += 1; \
			return true; \
	} \
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
			z80->regs.pc += 1;
			return true;
	}
	return true;
}

INSTR_DEF(sub_a_n)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
			z80->regs.pc += 2;
			return true;
	}
	return true;
}

#define INSTR_DEF_SBC_A_R(r) \
INSTR_DEF(sbc_a_##r) \
{ \
	switch (count) \
	{ \
		case 0: \
			z80->regs.pc += 1; \
			return true; \
	} \
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
			z80->regs.pc += 1;
			return true;
	}
	return true;
}

INSTR_DEF(sbc_a_n)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
			z80->regs.pc += 2;
			return true;
	}
	return true;
}

#define INSTR_DEF_AND_A_R(r) \
INSTR_DEF(and_a_##r) \
{ \
	switch (count) \
	{ \
		case 0: \
			z80->regs.pc += 1; \
			return true; \
	} \
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
			z80->regs.pc += 1;
			return true;
	}
	return true;
}

INSTR_DEF(and_a_n)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
			z80->regs.pc += 2;
			return true;
	}
	return true;
}

#define INSTR_DEF_XOR_A_R(r) \
INSTR_DEF(xor_a_##r) \
{ \
	switch (count) \
	{ \
		case 0: \
			z80->regs.pc += 1; \
			return true; \
	} \
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
			z80->regs.pc += 1;
			return true;
	}
	return true;
}

INSTR_DEF(xor_a_n)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
			z80->regs.pc += 2;
			return true;
	}
	return true;
}

#define INSTR_DEF_OR_A_R(r) \
INSTR_DEF(or_a_##r) \
{ \
	switch (count) \
	{ \
		case 0: \
			z80->regs.pc += 1; \
			return true; \
	} \
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
			z80->regs.pc += 1;
			return true;
	}
	return true;
}

INSTR_DEF(or_a_n)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
			z80->regs.pc += 2;
			return true;
	}
	return true;
}

#define INSTR_DEF_CP_A_R(r) \
INSTR_DEF(cp_a_##r) \
{ \
	switch (count) \
	{ \
		case 0: \
			z80->regs.pc += 1; \
			return true; \
	} \
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
			z80->regs.pc += 1;
			return true;
	}
	return true;
}

INSTR_DEF(cp_a_n)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
			z80->regs.pc += 2;
			return true;
	}
	return true;
}

INSTR_DEF(inc_rhl)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
			return false;
		case 2:
			z80->regs.pc += 1;
			return true;
	}
	return true;
}

INSTR_DEF(dec_rhl)
{
	switch (count)
	{
		case 0:
			return false;
		case 1:
			return false;
		case 2:
			z80->regs.pc += 1;
			return true;
	}
	return true;
}

INSTR_DEF(daa)
{
	switch (count)
	{
		case 0:
			z80->regs.pc += 1;
			return true;
	}
	return true;
}

INSTR_DEF(cpl)
{
	switch (count)
	{
		case 0:
			z80->regs.pc += 1;
			return true;
	}
	return true;
}

INSTR_DEF(scf)
{
	switch (count)
	{
		case 0:
			z80->regs.pc += 1;
			return true;
	}
	return true;
}

INSTR_DEF(ccf)
{
	switch (count)
	{
		case 0:
			z80->regs.pc += 1;
			return true;
	}
	return true;
}
