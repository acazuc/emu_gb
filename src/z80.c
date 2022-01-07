#include "z80.h"
#include "z80/instr.h"
#include <inttypes.h>
#include <string.h>
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
	uint8_t opcode = mem_gu8(z80->mem, z80->regs.pc);
	if (opcode == 0xCB)
		z80->instr = &g_z80_cb_instructions[mem_gu8(z80->mem, z80->regs.pc + 1)];
	else
		z80->instr = &g_z80_instructions[opcode];

	z80->instr_cycle = 0;
}

static void handle_interrupt(z80_t *z80)
{
	if (!z80->ime)
		return;
	uint8_t reg_if = mem_gu8(z80->mem, MEM_REG_IF);
	if (!reg_if)
		return;
	uint8_t ie = reg_if & mem_gu8(z80->mem, MEM_REG_IE);
	if (!ie)
		return;
	for (uint8_t i = 0; i < 8; ++i)
	{
		if (!(ie & (1 << i)))
			continue;
		mem_su8(z80->mem, --z80->regs.sp, z80->regs.pc);
		mem_su8(z80->mem, --z80->regs.sp, z80->regs.pc >> 8);
		mem_su8(z80->mem, MEM_REG_IF, ie & ~(1 << i));
		z80->regs.pc = 0x40 + 8 * i;
		z80->ime = false;
	}
}

static size_t decode_instr_param(z80_t *z80, char *cur, size_t len, const char *s, int l, int n)
{
	if (l == 2 && !strncmp(s, "nn", l))
	{
		snprintf(cur, len, "%%%04x", mem_gu16(z80->mem, z80->regs.pc + n));
		return 5;
	}
	else if (l == 1 && !strncmp(s, "n", l))
	{
		snprintf(cur, len, "%%%02x", mem_gu8(z80->mem, z80->regs.pc + n));
		return 3;
	}
	else if (l == 4 && !strncmp(s, "(nn)", l))
	{
		snprintf(cur, len, "(%%%04x)", mem_gu16(z80->mem, z80->regs.pc + n));
		return 7;
	}
	else if (l == 6 && !strncmp(s, "(ff+n)", l))
	{
		snprintf(cur, len, "(ff00+%%%02x)", mem_gu8(z80->mem, z80->regs.pc + n));
		return 10;
	}
	else
	{
		snprintf(cur, len, "%.*s", (int)l, s);
		return l;
	}
	return 0;
}

static void print_instr(z80_t *z80)
{
	const char *name = z80->instr->name;
	char decoded[100] = "";
	char *cur = decoded;
	size_t len = sizeof(decoded);
	char *tmp;

	tmp = strchr(name, ' ');
	if (tmp)
	{
		snprintf(cur, len, "%.*s ", (int)(tmp - name), name);
		len -= tmp - name + 1;
		cur += tmp - name + 1;
		name = tmp + 1;
		tmp = strstr(name, ", ");
		if (tmp != NULL)
		{
			size_t r = decode_instr_param(z80, cur, len, name, tmp - name, 1);
			cur += r;
			len -= r;
			snprintf(cur, len, ", ");
			cur += 2;
			len -= 2;
			r = decode_instr_param(z80, cur, len, tmp + 2, strlen(tmp) - 2, 2);
		}
		else
		{
			decode_instr_param(z80, cur, len, name, strlen(name), 1);
		}
	}
	else
	{
		snprintf(cur, len, "%s", name);
	}
	fprintf(stderr, "[%1d] %-20s A=%02x B=%02x C=%02x D=%02x E=%02x H=%02x L=%02x PC=%04x SP=%04x [%c%c%c%c]\n",
	        z80->instr_cycle,
	        decoded,
	        z80->regs.a,
	        z80->regs.b,
	        z80->regs.c,
	        z80->regs.d,
	        z80->regs.e,
	        z80->regs.h,
	        z80->regs.l,
	        z80->regs.pc,
	        z80->regs.sp,
	        z80->regs.f & Z80_FLAG_Z ? 'Z' : '-',
	        z80->regs.f & Z80_FLAG_N ? 'N' : '-',
	        z80->regs.f & Z80_FLAG_H ? 'H' : '-',
	        z80->regs.f & Z80_FLAG_C ? 'C' : '-');
}

static void z80_cycle(z80_t *z80)
{
#if 0
	next_instruction(z80);
	print_instr(z80);
	z80->instr->fn(z80, 0xff);
	return;
#endif

	if (!z80->instr)
		next_instruction(z80);

	bool end = z80->instr->fn(z80, z80->instr_cycle);
#if 0
	print_instr(z80);
#endif
	if (end)
	{
		handle_interrupt(z80);
		next_instruction(z80);
	}
	else
	{
		z80->instr_cycle++;
	}
}

void z80_clock(z80_t *z80)
{
	if (++z80->clock_count == 4)
	{
		z80_cycle(z80);
		z80->clock_count = 0;
	}
}

void z80_update_hflag(z80_t *z80, uint8_t v1, uint8_t v2)
{
	(void)z80;
	(void)v1;
	(void)v2;
}

void z80_update_cflag(z80_t *z80, uint8_t v1, uint8_t v2)
{
	(void)z80;
	(void)v1;
	(void)v2;
}
