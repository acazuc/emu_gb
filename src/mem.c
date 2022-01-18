#include "mem.h"
#include "mbc.h"
#include "apu.h"
#include "gb.h"

#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>

extern uint8_t _binary_dmgbios_bin_start;
extern uint8_t _binary_dmgbios_bin_end;

mem_t *mem_new(gb_t *gb, mbc_t *mbc)
{
	if (&_binary_dmgbios_bin_end - &_binary_dmgbios_bin_start != 0x100)
	{
		fprintf(stderr, "invalid dmgbios data: %u\n", (unsigned)(&_binary_dmgbios_bin_end - &_binary_dmgbios_bin_start));
		return NULL;
	}

	mem_t *mem = calloc(sizeof(*mem), 1);
	if (!mem)
	{
		fprintf(stderr, "allocation failed\n");
		return NULL;
	}

	mem->mbc = mbc;
	mem->gb = gb;
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

static uint8_t reg_get(mem_ref_t *ref)
{
	mem_t *mem = (mem_t*)ref->udata;
	return mem_get_reg(mem, ref->addr);
}

static void reg_set(mem_ref_t *ref, uint8_t v)
{
	mem_t *mem = (mem_t*)ref->udata;
	mem_set_reg(mem, ref->addr, v);
}

static uint8_t joyp_get(mem_ref_t *ref)
{
	mem_t *mem = (mem_t*)ref->udata;
	uint8_t joyp = mem_get_reg(mem, MEM_REG_JOYP);
	uint8_t v = 0x3F;
	if (!(joyp & (1 << 4)))
		v &= ~((1 << 4) | ((mem->joyp >> 0) & 0xF));
	if (!(joyp & (1 << 5)))
		v &= ~((1 << 5) | ((mem->joyp >> 4) & 0xF));
	return v;
}

static void joyp_set(mem_ref_t *ref, uint8_t v)
{
	mem_t *mem = (mem_t*)ref->udata;
	mem_set_reg(mem, MEM_REG_JOYP, v & 0x30);
}

static void stat_set(mem_ref_t *ref, uint8_t v)
{
	mem_t *mem = (mem_t*)ref->udata;
	mem_set_reg(mem, MEM_REG_STAT, (mem_get_reg(mem, MEM_REG_STAT) & 0x03) | (v & 0x74));
}

static uint8_t div_get(mem_ref_t *ref)
{
	mem_t *mem = (mem_t*)ref->udata;
	return mem->timer >> 8;
}

static void div_set(mem_ref_t *ref, uint8_t v)
{
	(void)v;
	mem_t *mem = (mem_t*)ref->udata;
	mem->timer = 0;
}

static void dma_set(mem_ref_t *ref, uint8_t v)
{
	mem_t *mem = (mem_t*)ref->udata;
	mem_set_reg(mem, MEM_REG_DMA, v);
	mem->dmatransfer = 0x9F;
}

static uint8_t nr11_get(mem_ref_t *ref)
{
	mem_t *mem = (mem_t*)ref->udata;
	return mem_get_reg(mem, MEM_REG_NR11) & 0xC0;
}

static void nr12_set(mem_ref_t *ref, uint8_t v)
{
	mem_t *mem = (mem_t*)ref->udata;
	if (!(v & 0xF8))
		mem_set_reg(mem, MEM_REG_NR52, mem_get_reg(mem, MEM_REG_NR52) & ~(1 << 0));
	mem_set_reg(mem, MEM_REG_NR12, v);
}

static uint8_t nr14_get(mem_ref_t *ref)
{
	mem_t *mem = (mem_t*)ref->udata;
	return mem_get_reg(mem, MEM_REG_NR14) & (1 << 6);
}

static void nr14_set(mem_ref_t *ref, uint8_t v)
{
	mem_t *mem = (mem_t*)ref->udata;
	mem_set_reg(mem, MEM_REG_NR14, v);
	if (v & (1 << 7))
		apu_start_channel1(mem->gb->apu);
}

static uint8_t nr21_get(mem_ref_t *ref)
{
	mem_t *mem = (mem_t*)ref->udata;
	return mem_get_reg(mem, MEM_REG_NR21) & 0xC0;
}

static void nr22_set(mem_ref_t *ref, uint8_t v)
{
	mem_t *mem = (mem_t*)ref->udata;
	if (!(v & 0xF8))
		mem_set_reg(mem, MEM_REG_NR52, mem_get_reg(mem, MEM_REG_NR52) & ~(1 << 1));
	mem_set_reg(mem, MEM_REG_NR22, v);
}

static uint8_t nr24_get(mem_ref_t *ref)
{
	mem_t *mem = (mem_t*)ref->udata;
	return mem_get_reg(mem, MEM_REG_NR24) & (1 << 6);
}

static void nr24_set(mem_ref_t *ref, uint8_t v)
{
	mem_t *mem = (mem_t*)ref->udata;
	mem_set_reg(mem, MEM_REG_NR24, v);
	if (v & (1 << 7))
		apu_start_channel2(mem->gb->apu);
}

static uint8_t nr30_get(mem_ref_t *ref)
{
	mem_t *mem = (mem_t*)ref->udata;
	return mem_get_reg(mem, MEM_REG_NR30) & 0x80;
}

static void nr30_set(mem_ref_t *ref, uint8_t v)
{
	mem_t *mem = (mem_t*)ref->udata;
	mem_set_reg(mem, MEM_REG_NR30, v & 0x80);
}

static uint8_t nr34_get(mem_ref_t *ref)
{
	mem_t *mem = (mem_t*)ref->udata;
	return mem_get_reg(mem, MEM_REG_NR34) & (1 << 6);
}

static void nr34_set(mem_ref_t *ref, uint8_t v)
{
	mem_t *mem = (mem_t*)ref->udata;
	mem_set_reg(mem, MEM_REG_NR34, v);
	if (v & (1 << 7))
		apu_start_channel3(mem->gb->apu);
}

static uint8_t nr44_get(mem_ref_t *ref)
{
	mem_t *mem = (mem_t*)ref->udata;
	return mem_get_reg(mem, MEM_REG_NR24) & (1 << 6);
}

static void nr44_set(mem_ref_t *ref, uint8_t v)
{
	mem_t *mem = (mem_t*)ref->udata;
	mem_set_reg(mem, MEM_REG_NR24, v);
	if (v & (1 << 7))
		apu_start_channel4(mem->gb->apu);
}

static void nr52_set(mem_ref_t *ref, uint8_t v)
{
	mem_t *mem = (mem_t*)ref->udata;
	if (v & (1 << 7))
		mem_set_reg(mem, MEM_REG_NR52, (mem_get_reg(mem, MEM_REG_NR52) & 0x7F) | (1 << 7));
	else
		mem_set_reg(mem, MEM_REG_NR52, 0);
}

static mem_ref_t mem_ref(mem_t *mem, uint16_t addr, bool dmabypass)
{
	if (mem->dmatransfer && !dmabypass)
	{
		if (addr >= 0xFF80 && addr < 0xFFFF)
			return MEM_REF_PTR(&mem->highram[addr - 0xFF00], simple_get, simple_set);
		return MEM_REF_PTR(NULL, empty_get, empty_set);
	}

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
		return MEM_REF_PTR(&mem->oam[addr - 0xFE00], simple_get, simple_set);

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
			return MEM_REF_ADDR(addr, div_get, div_set, mem);
		case MEM_REG_DMA:
			return MEM_REF_ADDR(addr, reg_get, dma_set, mem);
		case MEM_REG_NR11:
			return MEM_REF_ADDR(addr, nr11_get, reg_set, mem);
		case MEM_REG_NR12:
			return MEM_REF_ADDR(addr, reg_get, nr12_set, mem);
		case MEM_REG_NR14:
			return MEM_REF_ADDR(addr, nr14_get, nr14_set, mem);
		case MEM_REG_NR21:
			return MEM_REF_ADDR(addr, nr21_get, reg_set, mem);
		case MEM_REG_NR22:
			return MEM_REF_ADDR(addr, reg_get, nr22_set, mem);
		case MEM_REG_NR24:
			return MEM_REF_ADDR(addr, nr24_get, nr24_set, mem);
		case MEM_REG_NR30:
			return MEM_REF_ADDR(addr, nr30_get, nr30_set, mem);
		case MEM_REG_NR34:
			return MEM_REF_ADDR(addr, nr34_get, nr34_set, mem);
		case MEM_REG_NR44:
			return MEM_REF_ADDR(addr, nr44_get, nr44_set, mem);
		case MEM_REG_NR13:
		case MEM_REG_NR23:
		case MEM_REG_NR33:
			return MEM_REF_ADDR(addr, empty_get, reg_set, mem);
		case MEM_REG_NR52:
			return MEM_REF_ADDR(addr, reg_get, nr52_set, mem);
	}
	return MEM_REF_PTR(&mem->highram[addr - 0xFF00], simple_get, simple_set);
}

void mem_dmatransfer(mem_t *mem)
{
	uint8_t i = 0x9F - mem->dmatransfer;
	mem_ref_t ref = mem_ref(mem, mem->highram[MEM_REG_DMA - 0xFF00] * 0x100 + i, true);
	uint8_t v = ref.get ? ref.get(&ref) : 0;
	mem->oam[i] = v;
}

uint8_t mem_get(mem_t *mem, uint16_t addr)
{
	mem_ref_t ref = mem_ref(mem, addr, false);
	if (ref.get)
		return ref.get(&ref);
	return 0;
}

void mem_set(mem_t *mem, uint16_t addr, uint8_t v)
{
	mem_ref_t ref = mem_ref(mem, addr, false);
	if (ref.set)
		ref.set(&ref, v);
}
