#include "instr.h"
#include "cpu.h"

INSTR_DEF(nop)
{
	(void)count;
	cpu->regs.pc++;
	return true;
}

INSTR_DEF(stop)
{
	switch (count)
	{
		case 0:
			cpu->state = CPU_STOP;
			break;
	}
	cpu->regs.pc += 2;
	return true;
}

INSTR_DEF(halt)
{
	switch (count)
	{
		case 0:
			cpu->state = CPU_HALT;
			break;
	}
	cpu->regs.pc++;
	return true;
}

INSTR_DEF(ei)
{
	switch (count)
	{
		case 0:
			cpu->ei = 2;
			break;
	}
	cpu->regs.pc++;
	return true;
}

INSTR_DEF(di)
{
	switch (count)
	{
		case 0:
			cpu->ime = false;
			break;
	}
	cpu->regs.pc++;
	return true;
}
