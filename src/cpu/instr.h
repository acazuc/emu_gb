#ifndef CPU_INSTR_H
#define CPU_INSTR_H

#include <stdbool.h>
#include <stdint.h>

typedef struct cpu_s cpu_t;

typedef struct cpu_instr_s
{
	const char *name;
	bool (*fn)(cpu_t *cpu);
} cpu_instr_t;

extern cpu_instr_t g_cpu_instructions[256];
extern cpu_instr_t g_cpu_cb_instructions[256];
extern cpu_instr_t g_cpu_int_instructions[5];

#define INSTR_NAME(name) \
	cpu_instr_##name

#define INSTR_DECL(name) \
	extern bool INSTR_NAME(name)(cpu_t *cpu)

#define INSTR_DEF(name) \
	bool INSTR_NAME(name)(cpu_t *cpu)

#endif
