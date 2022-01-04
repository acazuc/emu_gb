#ifndef Z80_H
#define Z80_H

#include <stdint.h>
#include "mem.h"

enum z80_flag
{
	Z80_FLAG_Z = (1 << 0),
	Z80_FLAG_N = (1 << 1),
	Z80_FLAG_H = (1 << 2),
	Z80_FLAG_C = (1 << 3),
};

typedef union z80_regs_s
{
	struct
	{
		uint16_t pc;
		uint16_t sp;
		uint16_t ab;
		uint16_t cd;
		uint16_t ef;
		uint16_t hl;
	};
	struct
	{
		uint8_t pcl;
		uint8_t pch;
		uint8_t spl;
		uint8_t sph;
		uint8_t a;
		uint8_t b;
		uint8_t c;
		uint8_t d;
		uint8_t e;
		uint8_t f;
		uint8_t h;
		uint8_t l;
	};
} z80_regs_t;

typedef struct z80_instr_s z80_instr_t;

typedef struct z80_s
{
	z80_regs_t regs;
	uint32_t instr_cycle;
	z80_instr_t *instr;
	mem_t *mem;
} z80_t;

z80_t *z80_new(mem_t *mem);
void z80_del(z80_t *z80);

void z80_cycle(z80_t *z80);

#endif
