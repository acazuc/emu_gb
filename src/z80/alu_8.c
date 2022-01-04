#include "instr.h"
#include "z80.h"

#define INSTR_DEF_INC_R(r) \
INSTR_DEF(inc_##r) \
{ \
	(void)z80; \
	(void)count; \
}

#define INSTR_DEF_DEC_R(r) \
INSTR_DEF(dec_##r) \
{ \
	(void)z80; \
	(void)count; \
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

#define INSTR_DEF_OP_A_R(op, r) \
INSTR_DEF(op##_a_##r) \
{ \
	(void)z80; \
	(void)count; \
}

#define INSTR_DEF_OP_A(op) \
INSTR_DEF_OP_A_R(op, a) \
INSTR_DEF_OP_A_R(op, b) \
INSTR_DEF_OP_A_R(op, c) \
INSTR_DEF_OP_A_R(op, d) \
INSTR_DEF_OP_A_R(op, e) \
INSTR_DEF_OP_A_R(op, h) \
INSTR_DEF_OP_A_R(op, l) \
INSTR_DEF(op##_a_hl) \
{ \
	(void)z80; \
	(void)count; \
}

INSTR_DEF_OP_A(add);
INSTR_DEF_OP_A(adc);
INSTR_DEF_OP_A(sub);
INSTR_DEF_OP_A(sbc);
INSTR_DEF_OP_A(and);
INSTR_DEF_OP_A(xor);
INSTR_DEF_OP_A(or);
INSTR_DEF_OP_A(cp);

INSTR_DEF(inc_rhl)
{
	(void)z80;
	(void)count;
}

INSTR_DEF(dec_rhl)
{
	(void)z80;
	(void)count;
}

INSTR_DEF(daa)
{
	(void)z80;
	(void)count;
}

INSTR_DEF(cpl)
{
	(void)z80;
	(void)count;
}

INSTR_DEF(scf)
{
	(void)z80;
	(void)count;
}

INSTR_DEF(ccf)
{
	(void)z80;
	(void)count;
}
