#include "instr.h"
#include "z80.h"

#define INSTR_DEF_INC_RR(rr) \
INSTR_DEF(inc_##rr) \
{ \
	(void)z80; \
	(void)count; \
}

#define INSTR_DEF_DEC_RR(rr) \
INSTR_DEF(dec_##rr) \
{ \
	(void)z80; \
	(void)count; \
}

#define INSTR_DEF_ADD_HL_RR(rr) \
INSTR_DEF(add_hl_##rr) \
{ \
	(void)z80; \
	(void)count; \
}

#define INSTR_DEF_INC_DEC_RR(rr) \
INSTR_DEF_INC_RR(rr) \
INSTR_DEF_DEC_RR(rr) \
INSTR_DEF_ADD_HL_RR(rr)

INSTR_DEF_INC_DEC_RR(bc);
INSTR_DEF_INC_DEC_RR(de);
INSTR_DEF_INC_DEC_RR(hl);
INSTR_DEF_INC_DEC_RR(sp);
