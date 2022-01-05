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

static void next_instruction(z80_t *z80)
{
	uint8_t opcode = mem_u8(z80->mem, z80->regs.pc);
	if (opcode == 0xCB)
		z80->instr = &g_z80_cb_instructions[mem_u8(z80->mem, z80->regs.pc + 1)];
	else
		z80->instr = &g_z80_instructions[opcode];

	z80->instr_cycle = 0;
	fprintf(stderr, "%02" PRIx8 ": %s\n", opcode, z80->instr->name);
}

void z80_cycle(z80_t *z80)
{
	if (!z80->instr)
	{
		next_instruction(z80);
		return;
	}

	if (z80->instr->fn(z80, z80->instr_cycle))
		next_instruction(z80);
	else
		z80->instr_cycle++;
}
