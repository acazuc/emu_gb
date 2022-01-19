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
	{
		fprintf(stderr, "allocation failed\n");
		return NULL;
	}

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
	uint8_t opcode = mem_get(cpu->mem, cpu->regs.pc);
	if (opcode == 0xCB)
		cpu->instr = &g_cpu_cb_instructions[mem_get(cpu->mem, cpu->regs.pc + 1)];
	else
		cpu->instr = &g_cpu_instructions[opcode];

	cpu->instr_cycle = 0;
}

static bool handle_interrupt(cpu_t *cpu)
{
	uint8_t reg_if = mem_get_reg(cpu->mem, MEM_REG_IF);
	if (!reg_if)
		return false;
	uint8_t ie = mem_get_reg(cpu->mem, MEM_REG_IE);
	if (!(ie & reg_if))
		return false;
	cpu->state = CPU_RUN;
	if (!cpu->ime)
		return false;
	for (uint8_t i = 0; i < 5; ++i)
	{
		uint8_t mask = 1 << i;
		if (!((reg_if & ie) & mask))
			continue;
		mem_set_reg(cpu->mem, MEM_REG_IF, reg_if & ~mask);
		cpu->instr = &g_cpu_int_instructions[i];
		cpu->instr_cycle = 0;
		return true;
	}
	return false;
}

static size_t decode_instr_param(cpu_t *cpu, char *cur, size_t len, const char *s, int l, int *n)
{
	if (l == 2 && !strncmp(s, "nn", l))
	{
		snprintf(cur, len, "%%%02x%02x", mem_get(cpu->mem, cpu->regs.pc + *n), mem_get(cpu->mem, cpu->regs.pc + *n + 1));
		*n += 2;
		return 5;
	}
	else if (l == 1 && !strncmp(s, "n", l))
	{
		snprintf(cur, len, "%%%02x", mem_get(cpu->mem, cpu->regs.pc + *n));
		*n += 1;
		return 3;
	}
	else if (l == 1 && !strncmp(s, "d", l))
	{
		int8_t v = (int8_t)mem_get(cpu->mem, cpu->regs.pc + *n);
		snprintf(cur, len, "%c%%%02x", v < 0 ? '-' : '+', v < 0 ? -v : v);
		*n += 1;
		return 4;
	}
	else if (l == 4 && !strncmp(s, "(nn)", l))
	{
		snprintf(cur, len, "(%%%02x%02x)", mem_get(cpu->mem, cpu->regs.pc + *n), mem_get(cpu->mem, cpu->regs.pc + *n + 1));
		*n += 2;
		return 7;
	}
	else if (l == 6 && !strncmp(s, "(ff+n)", l))
	{
		snprintf(cur, len, "(%%ff00+%%%02x)", mem_get(cpu->mem, cpu->regs.pc + *n));
		*n += 1;
		return 11;
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
			int n = 1;
			size_t r = decode_instr_param(cpu, cur, len, name, tmp - name, &n);
			cur += r;
			len -= r;
			snprintf(cur, len, ", ");
			cur += 2;
			len -= 2;
			r = decode_instr_param(cpu, cur, len, tmp + 2, strlen(tmp) - 2, &n);
		}
		else
		{
			int n = 1;
			decode_instr_param(cpu, cur, len, name, strlen(name), &n);
		}
	}
	else
	{
		snprintf(cur, len, "%s", name);
	}
	fprintf(stderr, "[%1d] %-20s A=%02x B=%02x C=%02x D=%02x E=%02x H=%02x L=%02x PC=%04x SP=%04x [%c%c%c%c] IME=%d\n",
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
	        cpu->regs.f & CPU_FLAG_C ? 'C' : '-',
	        cpu->ime ? 1 : 0);
}

static void cpu_cycle(cpu_t *cpu)
{
	static bool debug = false;

#if 0
	if (cpu->regs.pc == 0x4174)
		debug = true;
#endif

#if 0
	next_instruction(cpu);
	print_instr(cpu);
	cpu->instr->fn(cpu, 0xff);
	return;
#endif

	if (!cpu->instr)
		next_instruction(cpu);

	if (cpu->state == CPU_HALT)
	{
		if (!handle_interrupt(cpu))
			return;
		if (debug)
			print_instr(cpu);
		cpu->state = CPU_RUN;
	}

	bool end = cpu->instr->fn(cpu, cpu->instr_cycle);
	if (end)
	{
		switch (cpu->ei)
		{
			case 0:
				break;
			case 1:
				cpu->ime = true;
				/* FALLTHROUGH */
			default:
				cpu->ei--;
				break;
		}
		if (!handle_interrupt(cpu))
			next_instruction(cpu);
		if (debug)
			print_instr(cpu);
	}
	else
	{
		cpu->instr_cycle++;
	}

	if (cpu->mem->dmatransfer)
	{
		mem_dmatransfer(cpu->mem);
		cpu->mem->dmatransfer--;
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
