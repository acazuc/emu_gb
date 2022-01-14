#ifndef APU_H
#define APU_H

#include "mem.h"

typedef struct apu_sample_s
{
	uint16_t l;
	uint16_t r;
} apu_sample_t;

typedef struct apu_s
{
	apu_sample_t data[4389];
	uint32_t sample;
	uint16_t clock;
	mem_t *mem;
} apu_t;

apu_t *apu_new(mem_t *mem);
void apu_del(apu_t *apu);

void apu_clock(apu_t *apu);

#endif
