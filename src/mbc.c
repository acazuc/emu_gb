#include "mbc.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

mbc_t *mbc_new(const void *data, size_t len)
{
	if (len < 0x150)
		return NULL;

	fprintf(stderr, "%s\n", &((char*)data)[0x134]);
	if ((size_t)((32 * 1024) << ((uint8_t*)data)[0x148]) != len)
		return NULL;

	uint8_t rombanks = (2 << ((uint8_t*)data)[0x148]) - 2;;
	uint8_t rambanks;
	switch (((uint8_t*)data)[0x149])
	{
		case 0x0:
		case 0x1:
			rambanks = 0;
			break;
		case 0x2:
			rambanks = 1;
			break;
		case 0x3:
			rambanks = 4;
			break;
		case 0x4:
			rambanks = 16;
			break;
		case 0x5:
			rambanks = 8;
			break;
		default:
			return NULL;
	}

	enum mbc_type mbctype = MBC_ROM;
	uint8_t options = MBC_OPT_NONE;
	switch (((uint8_t*)data)[0x147])
	{
		case 0x0: /* rom only */
			mbctype = MBC_ROM;
			options = MBC_OPT_NONE;
			break;
		case 0x1: /* mbc1 */
			mbctype = MBC1;
			options = MBC_OPT_NONE;
			break;
		case 0x2: /* mbc1 + ram */
			mbctype = MBC1;
			options = MBC_OPT_RAM;
			break;
		case 0x3: /* mbc1 + ram + battery */
			mbctype = MBC1;
			options = MBC_OPT_RAM | MBC_OPT_BATTERY;
			break;
		case 0x5: /* mbc2 */
			mbctype = MBC2;
			options = MBC_OPT_NONE;
			break;
		case 0x6: /* mbc2 + battery */
			mbctype = MBC2;
			options = MBC_OPT_BATTERY;
			break;
		case 0x8: /* rom + ram */
			return NULL; //unsupported
		case 0x9: /* rom + ram + battery */
			return NULL; //unsupported
		case 0xB: /* mmm1 */
			mbctype = MBC_MMM1;
			options = MBC_OPT_NONE;
			break;
		case 0xC: /* mmm1 + ram */
			mbctype = MBC_MMM1;
			options = MBC_OPT_RAM;
			break;
		case 0xD: /* mmm1 + ram + battery */
			mbctype = MBC_MMM1;
			options = MBC_OPT_RAM | MBC_OPT_BATTERY;
			break;
		case 0xF: /* mbc3 + timer + battery */
			mbctype = MBC3;
			options = MBC_OPT_TIMER | MBC_OPT_BATTERY;
			break;
		case 0x10: /* mbc3 + timer + ram + battery */
			mbctype = MBC3;
			options = MBC_OPT_TIMER | MBC_OPT_RAM | MBC_OPT_BATTERY;
			break;
		case 0x11: /* mbc3 */
			mbctype = MBC3;
			options = MBC_OPT_NONE;
			break;
		case 0x12: /* mbc3 + ram */
			mbctype = MBC3;
			options = MBC_OPT_RAM;
			break;
		case 0x13: /* mbc3 + ram + battery */
			mbctype = MBC3;
			options = MBC_OPT_RAM | MBC_OPT_BATTERY;
			break;
		case 0x19: /* mbc5 */
			mbctype = MBC5;
			options = MBC_OPT_NONE;
			break;
		case 0x1A: /* mbc5 + ram */
			mbctype = MBC5;
			options = MBC_OPT_RAM;
			break;
		case 0x1B: /* mbc5 + ram + battery */
			mbctype = MBC5;
			options = MBC_OPT_RAM | MBC_OPT_BATTERY;
			break;
		case 0x1C: /* mbc5 + rumble */
			mbctype = MBC5;
			options = MBC_OPT_RUMBLE;
			break;
		case 0x1D: /* mbc5 + rumble + ram */
			mbctype = MBC5;
			options = MBC_OPT_RUMBLE | MBC_OPT_RAM;
			break;
		case 0x1E: /* mbc5 + rumble + ram + battery */
			mbctype = MBC5;
			options = MBC_OPT_RUMBLE | MBC_OPT_RAM | MBC_OPT_BATTERY;
			break;
		case 0x20: /* mbc6 */
			mbctype = MBC6;
			options = MBC_OPT_NONE;
			break;
		case 0x22: /* mbc7 + sensor + rumble + ram + battery */
			mbctype = MBC7;
			options = MBC_OPT_SENSOR | MBC_OPT_RUMBLE | MBC_OPT_RAM | MBC_OPT_BATTERY;
			break;
		case 0xFC: /* pocket camera */
			mbctype = MBC_CAM;
			options = MBC_OPT_NONE;
			break;
		case 0xFD: /* bandai tama5 */
			mbctype = MBC_TAMA5;
			options = MBC_OPT_NONE;
			break;
		case 0xFE: /* HuC3 */
			mbctype = MBC_HUC3;
			options = MBC_OPT_NONE;
			break;
		case 0xFF: /* HuC1 + ram + battery */
			mbctype = MBC_HUC1;
			options = MBC_OPT_RAM | MBC_OPT_BATTERY;
			break;
		default:
			break;
	}

	mbc_t *mbc = malloc(sizeof(*mbc));
	if (mbc == NULL)
		return NULL;

	mbc->data = calloc(len, 1);
	if (mbc->data == NULL)
	{
		free(mbc);
		return NULL;
	}

	memcpy(mbc->data, data, len);
	mbc->size = len;
	mbc->rambanksnb = rambanks;
	mbc->rambanks = calloc(1, 0x8000 * rambanks);
	mbc->rombanksnb = rombanks;
	mbc->type = mbctype;
	mbc->options = options;
	if (!mbc->rambanks)
		return NULL;
	return mbc;
}

void mbc_del(mbc_t *mbc)
{
	if (mbc == NULL)
		return;

	free(mbc->rambanks);
	free(mbc->data);
	free(mbc);
}

static uint8_t rom0_get(mem_ref_t *ref)
{
	mbc_t *mbc = (mbc_t*)ref->udata;
	switch (mbc->type)
	{
		case MBC_ROM:
		case MBC1:
		case MBC2:
		case MBC3:
		case MBC5:
		case MBC6:
		case MBC7:
		case MBC_HUC1:
		case MBC_HUC3:
		case MBC_MMM1:
		case MBC_CAM:
		case MBC_TAMA5:
			if (ref->addr < mbc->size)
				return mbc->data[ref->addr];
			return 0;
	}
	return 0;
}

static void rom0_set(mem_ref_t *ref, uint8_t v)
{
	mbc_t *mbc = (mbc_t*)ref->udata;
	switch (mbc->type)
	{
		case MBC_ROM:
			return;
		case MBC1:
			if (ref->addr < 0x2000)
				mbc->ramenabled = ((v & 0x0F) == 0xA);
			else
				mbc->rombank = (mbc->rombank & 0xE0) | (v & 0x1F);
			return;
	}
}

static uint8_t romn_get(mem_ref_t *ref)
{
	mbc_t *mbc = (mbc_t*)ref->udata;
	switch (mbc->type)
	{
		case MBC_ROM:
			if (ref->addr < mbc->size)
				return mbc->data[ref->addr];
			return 0;
		case MBC1:
		case MBC2:
		case MBC3:
		case MBC5:
		case MBC6:
		case MBC7:
		case MBC_HUC1:
		case MBC_HUC3:
		case MBC_MMM1:
		case MBC_CAM:
		case MBC_TAMA5:
			if (ref->addr < mbc->size)
				return mbc->data[ref->addr + 0x4000 * (mbc->rombank & mbc->rombanksnb)];
			return 0;
	}
	return 0;
}

static void romn_set(mem_ref_t *ref, uint8_t v)
{
	mbc_t *mbc = (mbc_t*)ref->udata;
	switch (mbc->type)
	{
		case MBC_ROM:
			return;
		case MBC1:
			return;
	}
}

static uint8_t ram_get(mem_ref_t *ref)
{
}

static void ram_set(mem_ref_t *ref, uint8_t v)
{
}

mem_ref_t mbc_get_rom0(mbc_t *mbc, uint16_t addr)
{
	return MEM_REF_ADDR(addr, rom0_get, rom0_set, mbc);
}

mem_ref_t mbc_get_romn(mbc_t *mbc, uint16_t addr)
{
	return MEM_REF_ADDR(addr, romn_get, romn_set, mbc);
}

mem_ref_t mbc_get_ram(mbc_t *mbc, uint16_t addr)
{
	return MEM_REF_ADDR(addr, ram_get, ram_set, mbc);
}
