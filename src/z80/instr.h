#ifndef Z80_INSTR_H
#define Z80_INSTR_H

#include <stdbool.h>
#include <stdint.h>

typedef struct z80_s z80_t;

typedef struct z80_instr_s
{
	const char *name;
	bool (*fn)(z80_t *z80, uint32_t count);
} z80_instr_t;

extern z80_instr_t g_z80_instructions[256];
extern z80_instr_t g_z80_cb_instructions[256];

#define INSTR_NAME(name) \
	z80_instr_##name

#define INSTR_DECL(name) \
	extern bool INSTR_NAME(name)(z80_t *z80, uint32_t count)

#define INSTR_DEF(name) \
	bool INSTR_NAME(name)(z80_t *z80, uint32_t count)

#endif
