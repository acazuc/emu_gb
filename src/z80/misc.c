#include "instr.h"
#include "z80.h"

INSTR_DEF(nop)
{
	(void)z80;
	(void)count;
}

INSTR_DEF(stop)
{
	(void)z80;
	(void)count;
}

INSTR_DEF(halt)
{
	(void)z80;
	(void)count;
}
