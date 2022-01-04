#include "apu.h"
#include <stdlib.h>

apu_t *apu_new(mem_t *mem)
{
	apu_t *apu = calloc(sizeof(*mem), 1);
	if (!apu)
		return NULL;

	apu->mem = mem;
	return apu;
}

void apu_del(apu_t *apu)
{
	if (!apu)
		return;
	free(apu);
}
