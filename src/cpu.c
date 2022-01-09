#include "cpu.h"
#include "cpu/instr.h"
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

cpu_t *cpu_new(mem_t *mem)
{
	cpu_t *cpu = calloc(sizeof(*cpu), 1);
	if (!cpu)
		return NULL;
	cpu->mem = mem;
	return cpu;
}

void cpu_del(cpu_t *cpu)
{
	if (!cpu)
		return;

	free(cpu);
}

static void next_instruction(cpu_t *cpu)
{
	uint8_t opcode = mem_get8(cpu->mem, cpu->regs.pc);
	if (opcode == 0xCB)
		cpu->instr = &g_cpu_cb_instructions[mem_get8(cpu->mem, cpu->regs.pc + 1)];
	else
		cpu->instr = &g_cpu_instructions[opcode];

	cpu->instr_cycle = 0;
}

static void handle_interrupt(cpu_t *cpu)
{
	if (!cpu->ime)
		return;
	uint8_t reg_if = mem_get8(cpu->mem, MEM_REG_IF);
	if (!reg_if)
		return;
	uint8_t ie = reg_if & mem_get8(cpu->mem, MEM_REG_IE);
	if (!ie)
		return;
	for (uint8_t i = 0; i < 8; ++i)
	{
		if (!(ie & (1 << i)))
			continue;
		mem_set8(cpu->mem, --cpu->regs.sp, cpu->regs.pc);
		mem_set8(cpu->mem, --cpu->regs.sp, cpu->regs.pc >> 8);
		mem_set8(cpu->mem, MEM_REG_IF, ie & ~(1 << i));
		cpu->regs.pc = 0x40 + 8 * i;
		cpu->ime = false;
	}
}

static size_t decode_instr_param(cpu_t *cpu, char *cur, size_t len, const char *s, int l, int n)
{
	if (l == 2 && !strncmp(s, "nn", l))
	{
		snprintf(cur, len, "%%%04x", mem_get16(cpu->mem, cpu->regs.pc + n));
		return 5;
	}
	else if (l == 1 && !strncmp(s, "n", l))
	{
		snprintf(cur, len, "%%%02x", mem_get8(cpu->mem, cpu->regs.pc + n));
		return 3;
	}
	else if (l == 4 && !strncmp(s, "(nn)", l))
	{
		snprintf(cur, len, "(%%%04x)", mem_get16(cpu->mem, cpu->regs.pc + n));
		return 7;
	}
	else if (l == 6 && !strncmp(s, "(ff+n)", l))
	{
		snprintf(cur, len, "(ff00+%%%02x)", mem_get8(cpu->mem, cpu->regs.pc + n));
		return 10;
	}
	else
	{
		snprintf(cur, len, "%.*s", (int)l, s);
		return l;
	}
	return 0;
}

static void print_instr(cpu_t *cpu)
{
	const char *name = cpu->instr->name;
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
			size_t r = decode_instr_param(cpu, cur, len, name, tmp - name, 1);
			cur += r;
			len -= r;
			snprintf(cur, len, ", ");
			cur += 2;
			len -= 2;
			r = decode_instr_param(cpu, cur, len, tmp + 2, strlen(tmp) - 2, 2);
		}
		else
		{
			decode_instr_param(cpu, cur, len, name, strlen(name), 1);
		}
	}
	else
	{
		snprintf(cur, len, "%s", name);
	}
	fprintf(stderr, "[%1d] %-20s A=%02x B=%02x C=%02x D=%02x E=%02x H=%02x L=%02x PC=%04x SP=%04x [%c%c%c%c]\n",
	        cpu->instr_cycle,
	        decoded,
	        cpu->regs.a,
	        cpu->regs.b,
	        cpu->regs.c,
	        cpu->regs.d,
	        cpu->regs.e,
	        cpu->regs.h,
	        cpu->regs.l,
	        cpu->regs.pc,
	        cpu->regs.sp,
	        cpu->regs.f & CPU_FLAG_Z ? 'Z' : '-',
	        cpu->regs.f & CPU_FLAG_N ? 'N' : '-',
	        cpu->regs.f & CPU_FLAG_H ? 'H' : '-',
	        cpu->regs.f & CPU_FLAG_C ? 'C' : '-');
}

static void cpu_cycle(cpu_t *cpu)
{
	static bool debug = false;
	if (cpu->regs.pc == 0x100)
		debug = true;
	if (cpu->regs.pc == 0x38)
		debug = false;

#if 0
	next_instruction(cpu);
	print_instr(cpu);
	cpu->instr->fn(cpu, 0xff);
	return;
#endif

	if (!cpu->instr)
		next_instruction(cpu);

	bool end = cpu->instr->fn(cpu, cpu->instr_cycle);
	if (end)
	{
		handle_interrupt(cpu);
		next_instruction(cpu);
		if (debug)
			print_instr(cpu);
	}
	else
	{
		cpu->instr_cycle++;
	}
}

void cpu_clock(cpu_t *cpu)
{
	if (++cpu->clock_count == 4)
	{
		cpu_cycle(cpu);
		cpu->clock_count = 0;
	}
}

void cpu_update_hflag16(cpu_t *cpu, uint16_t v1, uint16_t v2, uint8_t add)
{
	if (add)
	{
		CPU_SET_FLAG_C(cpu, v1 < v2 || (v1 & 0xF0) != (v2 & 0xF0));
	}
	else
	{
		CPU_SET_FLAG_C(cpu, v1 > v2 || (v1 & 0xF0) != (v2 & 0xF0));
	}
}

void cpu_update_hflag8(cpu_t *cpu, uint8_t v1, uint8_t v2, uint8_t add)
{
	if (add)
	{
		CPU_SET_FLAG_C(cpu, v1 < v2 || (v1 & 0xF0) != (v2 & 0xF0));
	}
	else
	{
		CPU_SET_FLAG_C(cpu, v1 > v2 || (v1 & 0xF0) != (v2 & 0xF0));
	}
}

void cpu_update_cflag16(cpu_t *cpu, uint16_t v1, uint16_t v2, uint8_t add)
{
	if (add)
	{
		CPU_SET_FLAG_C(cpu, v1 < v2);
	}
	else
	{
		CPU_SET_FLAG_C(cpu, v1 > v2);
	}
}

void cpu_update_cflag8(cpu_t *cpu, uint8_t v1, uint8_t v2, uint8_t add)
{
	if (add)
	{
		CPU_SET_FLAG_C(cpu, v1 < v2);
	}
	else
	{
		CPU_SET_FLAG_C(cpu, v1 > v2);
	}
}
