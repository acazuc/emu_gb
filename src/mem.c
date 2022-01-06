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

uint8_t mem_get_u8(mem_t *mem, uint16_t addr)
{
	if (addr < mem->rom->size)
		return ((uint8_t*)mem->rom->data)[addr];

	fprintf(stderr, "access to unknown addr: %" PRIx16 "\n", addr);
	return 0;
}

int8_t mem_get_i8(mem_t *mem, uint16_t addr)
{
	return (int8_t)mem_get_u8(mem, addr);
}

uint16_t mem_get_u16(mem_t *mem, uint16_t addr)
{
	return (mem_get_u8(mem, addr) << 8)
	     | (mem_get_u8(mem, addr + 1));
}

void mem_set_u8(mem_t *mem, uint16_t addr, uint8_t value)
{
}

void mem_set_i8(mem_t *mem, uint16_t addr, int8_t value)
{
	mem_set_u8(mem, addr, (uint8_t)value);
}

void mem_set_u16(mem_t *mem, uint16_t addr, uint16_t value)
{
	mem_set_u8(mem, addr, value >> 8);
	mem_set_u8(mem, addr + 1, value);
}
