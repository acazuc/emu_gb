#include "instr.h"
#include "z80.h"

INSTR_DEF(nop)
{
	(void)count;
	z80->regs.pc++;
	return true;
}

INSTR_DEF(stop)
{
	(void)count;
	z80->regs.pc += 2;
	return true;
}

INSTR_DEF(halt)
{
	(void)count;
	z80->regs.pc++;
	return true;
}

INSTR_DEF(ei)
{
	(void)count;
	z80->regs.pc++;
	return true;
}

INSTR_DEF(di)
{
	(void)count;
	z80->regs.pc++;
	return true;
}
