#ifndef Z80_INSTR_H
#define Z80_INSTR_H

#include <stdint.h>

typedef struct z80_s z80_t;

typedef struct z80_instr_s
{
	const char *name;
	uint32_t length;
	uint32_t cycles;
	void (*fn)(z80_t *z80, uint32_t count);
} z80_instr_t;

extern z80_instr_t g_z80_instructions[256];

#define INSTR_NAME(name) \
	z80_instr_##name

#define INSTR_DECL(name) \
	extern void INSTR_NAME(name)(z80_t *z80, uint32_t count)

#define INSTR_DEF(name) \
	void INSTR_NAME(name)(z80_t *z80, uint32_t count)

#endif
