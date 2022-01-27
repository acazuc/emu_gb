#include "instr.h"
#include "cpu.h"

INSTR_DEF(nop)
{
	cpu->regs.pc++;
	return true;
}

INSTR_DEF(stop)
{
	switch (cpu->instr_cycle)
	{
		case 0:
		{
			cpu->state = CPU_STOP;
			uint8_t key1 = mem_get_reg(cpu->mem, MEM_REG_KEY1);
			if (key1 & 1)
			{
				cpu->mem->doublespeed = !cpu->mem->doublespeed;
				mem_set_reg(cpu->mem, MEM_REG_KEY1, key1 & 0xFE);
			}
			break;
		}
	}
	cpu->regs.pc += 2;
	return true;
}

INSTR_DEF(halt)
{
	switch (cpu->instr_cycle)
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
	switch (cpu->instr_cycle)
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
	switch (cpu->instr_cycle)
	{
		case 0:
			cpu->ime = false;
			break;
	}
	cpu->regs.pc++;
	return true;
}
