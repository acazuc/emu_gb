#ifndef APU_H
#define APU_H

#include "mem.h"

typedef struct apu_s
{
	mem_t *mem;
} apu_t;

apu_t *apu_new(mem_t *mem);
void apu_del(apu_t *apu);

#endif
