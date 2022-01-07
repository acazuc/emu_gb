#ifndef Z80_H
#define Z80_H

#include <stdint.h>
#include <stdbool.h>
#include "mem.h"

enum z80_flag
{
	Z80_FLAG_Z = (1 << 7),
	Z80_FLAG_N = (1 << 6),
	Z80_FLAG_H = (1 << 5),
	Z80_FLAG_C = (1 << 4),
};

#define Z80_GET_FLAG(z80, flag) (((z80)->regs.f & flag) ? 1 : 0)
#define Z80_GET_FLAG_Z(z80) Z80_GET_FLAG(z80, Z80_FLAG_Z)
#define Z80_GET_FLAG_N(z80) Z80_GET_FLAG(z80, Z80_FLAG_N)
#define Z80_GET_FLAG_H(z80) Z80_GET_FLAG(z80, Z80_FLAG_H)
#define Z80_GET_FLAG_C(z80) Z80_GET_FLAG(z80, Z80_FLAG_C)

#define Z80_SET_FLAG(z80, flag, v) \
do \
{ \
	if (v) \
		z80->regs.f |=  flag; \
	else \
		z80->regs.f &= ~flag; \
} while (0)

#define Z80_SET_FLAG_Z(z80, v) Z80_SET_FLAG(z80, Z80_FLAG_Z, v)
#define Z80_SET_FLAG_N(z80, v) Z80_SET_FLAG(z80, Z80_FLAG_N, v)
#define Z80_SET_FLAG_H(z80, v) Z80_SET_FLAG(z80, Z80_FLAG_H, v)
#define Z80_SET_FLAG_C(z80, v) Z80_SET_FLAG(z80, Z80_FLAG_C, v)

typedef union z80_regs_s
{
	struct
	{
		uint16_t pc;
		uint16_t sp;
		uint16_t pad16; /* a + pad */
		uint16_t bc;
		uint16_t de;
		uint16_t hl;
	};
	struct
	{
		uint8_t pcl;
		uint8_t pch;
		uint8_t p;
		uint8_t s;
		uint8_t f;
		uint8_t a;
		uint8_t c;
		uint8_t b;
		uint8_t e;
		uint8_t d;
		uint8_t l;
		uint8_t h;
	};
} z80_regs_t;

typedef struct z80_instr_s z80_instr_t;

typedef struct z80_s
{
	z80_regs_t regs;
	uint32_t instr_cycle;
	uint32_t clock_count;
	union
	{
		uint8_t  u8[16];
		 int8_t  i8[16];
		uint16_t u16[8];
	} instr_tmp;
	z80_instr_t *instr;
	mem_t *mem;
	bool ime;
} z80_t;

z80_t *z80_new(mem_t *mem);
void z80_del(z80_t *z80);

void z80_clock(z80_t *z80);
void z80_update_hflag(z80_t *z80, uint8_t v1, uint8_t v2);
void z80_update_cflag(z80_t *z80, uint8_t v1, uint8_t v2);

#endif
