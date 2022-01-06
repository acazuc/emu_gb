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
	uint8_t opcode = mem_get_u8(z80->mem, z80->regs.pc);
	if (opcode == 0xCB)
		z80->instr = &g_z80_cb_instructions[mem_get_u8(z80->mem, z80->regs.pc + 1)];
	else
		z80->instr = &g_z80_instructions[opcode];

	z80->instr_cycle = 0;
}

static void z80_cycle(z80_t *z80)
{
	if (!z80->instr)
		next_instruction(z80);

	fprintf(stderr, "[%1d] %-20s (a: %02x; b: %02x, c: %02x, d: %02x, e: %02x, f: %02x, h: %02x, l: %02x, pc: %04x, sp: %04x) %c%c%c%c\n",
	        z80->instr_cycle,
	        z80->instr->name,
	        z80->regs.a,
	        z80->regs.b,
	        z80->regs.c,
	        z80->regs.d,
	        z80->regs.e,
	        z80->regs.f,
	        z80->regs.h,
	        z80->regs.l,
	        z80->regs.pc,
	        z80->regs.sp,
	        z80->regs.f & Z80_FLAG_Z ? 'Z' : '-',
	        z80->regs.f & Z80_FLAG_N ? 'N' : '-',
	        z80->regs.f & Z80_FLAG_H ? 'H' : '-',
	        z80->regs.f & Z80_FLAG_C ? 'C' : '-');
	if (z80->instr->fn(z80, z80->instr_cycle))
		next_instruction(z80);
	else
		z80->instr_cycle++;
}

void z80_clock(z80_t *z80)
{
	if (++z80->clock_count == 4)
	{
		z80_cycle(z80);
		z80->clock_count = 0;
	}
}

void z80_update_zflag(z80_t *z80, uint8_t v)
{
	if (v == 0)
		z80->regs.f |=  Z80_FLAG_Z;
	else
		z80->regs.f &= ~Z80_FLAG_Z;
}

void z80_update_hflag(z80_t *z80, uint8_t v1, uint8_t v2)
{
}

void z80_update_cflag(z80_t *z80, uint8_t v1, uint8_t v2)
{
}
