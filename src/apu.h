#ifndef APU_H
#define APU_H

#include "mem.h"

typedef struct apu_s
{
	char data[48000 / 59];
	mem_t *mem;
} apu_t;

apu_t *apu_new(mem_t *mem);
void apu_del(apu_t *apu);

void apu_clock(apu_t *apu);

#endif
