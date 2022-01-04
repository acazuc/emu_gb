#include "instr.h"
#include "z80.h"

#define INSTR_DEF_LD_RR_NN(rr) \
INSTR_DEF(ld_##rr##_nn) \
{ \
	(void)z80; \
	(void)count; \
}

INSTR_DEF_LD_RR_NN(bc);
INSTR_DEF_LD_RR_NN(de);
INSTR_DEF_LD_RR_NN(hl);
INSTR_DEF_LD_RR_NN(sp);

INSTR_DEF(ld_nn_sp)
{
	(void)z80;
	(void)count;
}
