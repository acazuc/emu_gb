#include "mem.h"
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>

mem_t *mem_new(rom_t *rom)
{
	mem_t *mem = calloc(sizeof(*mem), 1);
	if (!mem)
		return NULL;

	mem->rom = rom;
	return mem;
}

void mem_del(mem_t *mem)
{
	if (!mem)
		return;
	free(mem);
}

uint8_t mem_u8(mem_t *mem, uint16_t addr)
{
	if (addr < mem->rom->size)
		return ((uint8_t*)mem->rom->data)[addr];

	fprintf(stderr, "access to unknown addr: %" PRIx16 "\n", addr);
	return 0;
}

uint16_t mem_u16(mem_t *mem, uint16_t addr)
{
	if (addr < mem->rom->size)
		return ((uint16_t*)mem->rom->data)[addr];

	fprintf(stderr, "access to unknown addr: %" PRIx16 "\n", addr);
	return 0;
}
