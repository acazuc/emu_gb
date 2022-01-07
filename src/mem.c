#include "mem.h"
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>

extern uint8_t _binary_dmgbios_bin_start;
extern uint8_t _binary_dmgbios_bin_end;

mem_t *mem_new(rom_t *rom)
{
	if (&_binary_dmgbios_bin_end - &_binary_dmgbios_bin_start != 0x100)
	{
		fprintf(stderr, "invalid dmgbios data: %u\n", (unsigned)(&_binary_dmgbios_bin_end - &_binary_dmgbios_bin_start));
		return NULL;
	}

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

static uint8_t *mem_u8(mem_t *mem, uint16_t addr)
{
	if (addr < 0x100)
	{
		if (mem->highram[MEM_REG_RBK - 0xFF00])
			return rom_get_u8_rb0(mem->rom, addr);
		else
			return &(&_binary_dmgbios_bin_start)[addr];
	}

	if (addr < 0x4000)
		return rom_get_u8_rb0(mem->rom, addr);

	if (addr < 0x8000)
		return rom_get_u8_rbn(mem->rom, addr - 0x4000);

	if (addr < 0xA000) /* vram */
		return &mem->vram[addr - 0x8000];

	if (addr < 0xC000) /* external ram */
		return NULL;

	if (addr < 0xD000) /* work bank 0 */
		return &mem->workram0[addr - 0xC000];

	if (addr < 0xE000) /* work ram bank n */
		return &mem->workram1[addr - 0xD000];

	if (addr < 0xF000) /* echo ram C000-D000 */
		return &mem->workram0[addr - 0xE000];

	if (addr < 0xFE00) /* echo ram D000-DDFF */
		return &mem->workram1[addr - 0xF000];

	if (addr < 0xFEA0) /* OAM */
		return NULL;

	if (addr < 0xFF00) /* unmapped */
		return NULL;

	return &mem->highram[addr - 0xFF00];
}

uint8_t mem_gu8(mem_t *mem, uint16_t addr)
{
	uint8_t *ptr = mem_u8(mem, addr);
	if (!ptr)
		return 0;
	return *ptr;
}

void mem_su8(mem_t *mem, uint16_t addr, uint8_t v)
{
	uint8_t *ptr = mem_u8(mem, addr);
	if (!ptr)
		return;
	if (addr >= 0x8000 && addr < 0xA000)
		printf("setting %x to %x\n", addr, v);
	*ptr = v;
}

void mem_xu8(mem_t *mem, uint16_t addr, uint8_t v)
{
	uint8_t *ptr = mem_u8(mem, addr);
	if (!ptr)
		return;
	*ptr ^= v;
}

void mem_ou8(mem_t *mem, uint16_t addr, uint8_t v)
{
	uint8_t *ptr = mem_u8(mem, addr);
	if (!ptr)
		return;
	*ptr |= v;
}

void mem_au8(mem_t *mem, uint16_t addr, uint8_t v)
{
	uint8_t *ptr = mem_u8(mem, addr);
	if (!ptr)
		return;
	*ptr &= v;
}

int8_t *mem_i8(mem_t *mem, uint16_t addr)
{
	return (int8_t*)mem_u8(mem, addr);
}

int8_t mem_gi8(mem_t *mem, uint16_t addr)
{
	int8_t *ptr = mem_i8(mem, addr);
	if (!ptr)
		return 0;
	return *ptr;
}

void mem_si8(mem_t *mem, uint16_t addr, int8_t v)
{
	int8_t *ptr = mem_i8(mem, addr);
	if (!ptr)
		return;
	*ptr = v;
}

void mem_xi8(mem_t *mem, uint16_t addr, int8_t v)
{
	int8_t *ptr = mem_i8(mem, addr);
	if (!ptr)
		return;
	*ptr ^= v;
}

void mem_oi8(mem_t *mem, uint16_t addr, int8_t v)
{
	int8_t *ptr = mem_i8(mem, addr);
	if (!ptr)
		return;
	*ptr |= v;
}

void mem_ai8(mem_t *mem, uint16_t addr, int8_t v)
{
	int8_t *ptr = mem_i8(mem, addr);
	if (!ptr)
		return;
	*ptr &= v;
}

uint16_t mem_gu16(mem_t *mem, uint16_t addr)
{
	return mem_gu8(mem, addr + 0) << 0
	     | mem_gu8(mem, addr + 1) << 8;
}

void mem_su16(mem_t *mem, uint16_t addr, uint16_t v)
{
	mem_su8(mem, addr + 0, v >> 0);
	mem_su8(mem, addr + 1, v >> 8);
}

void mem_xu16(mem_t *mem, uint16_t addr, uint16_t v)
{
	mem_xu8(mem, addr + 0, v >> 0);
	mem_xu8(mem, addr + 1, v >> 8);
}

void mem_ou16(mem_t *mem, uint16_t addr, uint16_t v)
{
	mem_xu8(mem, addr + 0, v >> 0);
	mem_xu8(mem, addr + 1, v >> 8);
}

void mem_au16(mem_t *mem, uint16_t addr, uint16_t v)
{
	mem_au8(mem, addr + 0, v >> 0);
	mem_au8(mem, addr + 1, v >> 8);
}
