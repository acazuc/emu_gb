#include "z80.h"
#include "z80/instr.h"
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>

z80_t *z80_new(mem_t *mem)
{
	z80_t *z80 = calloc(sizeof(*z80), 1);
	if (!z80)
		return NULL;
	z80->mem = mem;
	return z80;
}

void z80_del(z80_t *z80)
{
	if (!z80)
		return;

	free(z80);
}

void z80_cycle(z80_t *z80)
{
	if (z80->instr_cycle == 0)
	{
		uint8_t opcode = mem_u8(z80->mem, z80->regs.pc);
		z80->instr = &g_z80_instructions[opcode];
		if (!z80->instr->name)
		{
			fprintf(stderr, "unknown instruction: %02" PRIx8 " as %04" PRIx16 " \n", opcode, z80->regs.pc);
			z80->regs.pc++;
			return;
		}

		fprintf(stderr, "found instruction: %02" PRIx8 " (%s)\n", opcode, z80->instr->name);
		z80->instr_cycle = z80->instr->cycles;
	}

	z80->instr->fn(z80, z80->instr->cycles - z80->instr_cycle);
	z80->instr_cycle--;
	if (z80->instr_cycle == 0)
		z80->regs.pc += z80->instr->length;
}
