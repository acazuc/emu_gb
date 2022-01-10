#include "mem.h"
#include "mbc.h"
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>

extern uint8_t _binary_dmgbios_bin_start;
extern uint8_t _binary_dmgbios_bin_end;

mem_t *mem_new(mbc_t *mbc)
{
	if (&_binary_dmgbios_bin_end - &_binary_dmgbios_bin_start != 0x100)
	{
		fprintf(stderr, "invalid dmgbios data: %u\n", (unsigned)(&_binary_dmgbios_bin_end - &_binary_dmgbios_bin_start));
		return NULL;
	}

	mem_t *mem = calloc(sizeof(*mem), 1);
	if (!mem)
		return NULL;

	mem->mbc = mbc;
	return mem;
}

void mem_del(mem_t *mem)
{
	if (!mem)
		return;
	free(mem);
}

static uint8_t simple_get(mem_ref_t *ref)
{
	return *ref->ptr;
}

static void simple_set(mem_ref_t *ref, uint8_t v)
{
	*ref->ptr = v;
}

static uint8_t empty_get(mem_ref_t *ref)
{
	(void)ref;
	return 0;
}

static void empty_set(mem_ref_t *ref, uint8_t v)
{
	(void)ref;
	(void)v;
}

static uint8_t oam_get(mem_ref_t *ref)
{
	(void)ref;
	return 0;
}

static void oam_set(mem_ref_t *ref, uint8_t v)
{
	(void)ref;
	(void)v;
}

static uint8_t reg_get(mem_ref_t *ref)
{
	mem_t *mem = (mem_t*)ref->udata;
	return mem_get_reg(mem, ref->addr);
}

static uint8_t joyp_get(mem_ref_t *ref)
{
	mem_t *mem = (mem_t*)ref->udata;
	if (mem_get_reg(mem, MEM_REG_JOYP) & 0x10)
		return 0x10 | ((~mem->joyp) & 0xF);
	return ~(mem->joyp >> 4);
}

static void joyp_set(mem_ref_t *ref, uint8_t v)
{
	mem_t *mem = (mem_t*)ref->udata;
	mem_set_reg(mem, MEM_REG_JOYP, (mem_get_reg(mem, MEM_REG_JOYP) & 0x0F) | (v & 0x10));
}

static void stat_set(mem_ref_t *ref, uint8_t v)
{
	mem_t *mem = (mem_t*)ref->udata;
	mem_set_reg(mem, MEM_REG_JOYP, (mem_get_reg(mem, MEM_REG_JOYP) & 0x03) | (v & 0xF40));
}

static void div_set(mem_ref_t *ref, uint8_t v)
{
	(void)v;
	mem_t *mem = (mem_t*)ref->udata;
	mem_set_reg(mem, MEM_REG_DIV, 0);
}

static mem_ref_t mem_u8(mem_t *mem, uint16_t addr)
{
	if (addr < 0x100)
	{
		if (mem_get_reg(mem, MEM_REG_BOOT))
			return mbc_get_rom0(mem->mbc, addr);
		else
			return MEM_REF_PTR(&(&_binary_dmgbios_bin_start)[addr], simple_get, empty_set);
	}

	if (addr < 0x4000)
		return mbc_get_rom0(mem->mbc, addr);

	if (addr < 0x8000)
		return mbc_get_romn(mem->mbc, addr);

	if (addr < 0xA000) /* vram */
		return MEM_REF_PTR(&mem->vram[addr - 0x8000], simple_get, simple_set);

	if (addr < 0xC000) /* external ram */
		return mbc_get_ram(mem->mbc, addr);

	if (addr < 0xD000) /* work ram bank 0 */
		return MEM_REF_PTR(&mem->workram0[addr - 0xC000], simple_get, simple_set);

	if (addr < 0xE000) /* work ram bank n */
		return MEM_REF_PTR(&mem->workram1[addr - 0xD000], simple_get, simple_set);

	if (addr < 0xF000) /* echo ram C000-D000 */
		return MEM_REF_PTR(&mem->workram0[addr - 0xE000], simple_get, simple_set);

	if (addr < 0xFE00) /* echo ram D000-DDFF */
		return MEM_REF_PTR(&mem->workram1[addr - 0xF000], simple_get, simple_set);

	if (addr < 0xFEA0) /* OAM */
		return MEM_REF_PTR(&mem->oam[addr - 0xFE00], oam_get, oam_set);

	if (addr < 0xFF00) /* unmapped */
		return MEM_REF_PTR(NULL, empty_get, empty_set);

	switch (addr)
	{
		case MEM_REG_JOYP:
			return MEM_REF_ADDR(0, joyp_get, joyp_set, mem);
		case MEM_REG_LY:
			return MEM_REF_ADDR(addr, reg_get, empty_set, mem);
		case MEM_REG_STAT:
			return MEM_REF_ADDR(addr, reg_get, stat_set, mem);
		case MEM_REG_DIV:
			return MEM_REF_ADDR(addr, reg_get, div_set, mem);
	}
	return MEM_REF_PTR(&mem->highram[addr - 0xFF00], simple_get, simple_set);
}

uint8_t mem_get_reg(mem_t *mem, uint16_t addr)
{
	return mem->highram[addr - 0xFF00];
}

void mem_set_reg(mem_t *mem, uint16_t addr, uint8_t v)
{
	mem->highram[addr - 0xFF00] = v;
}

uint8_t mem_get8(mem_t *mem, uint16_t addr)
{
	mem_ref_t ref = mem_u8(mem, addr);
	if (ref.get)
		return ref.get(&ref);
	return 0;
}

void mem_set8(mem_t *mem, uint16_t addr, uint8_t v)
{
	mem_ref_t ref = mem_u8(mem, addr);
	if (ref.set)
		ref.set(&ref, v);
}

uint16_t mem_get16(mem_t *mem, uint16_t addr)
{
	return mem_get8(mem, addr + 0) << 0
	     | mem_get8(mem, addr + 1) << 8;
}

void mem_set16(mem_t *mem, uint16_t addr, uint16_t v)
{
	mem_set8(mem, addr + 0, v >> 0);
	mem_set8(mem, addr + 1, v >> 8);
}
