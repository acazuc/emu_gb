#include "mbc.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

mbc_t *mbc_new(const void *data, size_t len)
{
	if (len < 0x150)
	{
		fprintf(stderr, "rom too short\n");
		return NULL;
	}

	fprintf(stderr, "%s\n", &((char*)data)[0x134]);
	if ((size_t)((32 * 1024) << ((uint8_t*)data)[0x148]) < len)
	{
		fprintf(stderr, "rom has invalid size: got %u, expected %u\n", (unsigned)((32 * 1024) << ((uint8_t*)data)[0x148]), (unsigned)len);
		return NULL;
	}

	uint8_t rombanks = (2 << ((uint8_t*)data)[0x148]) - 2;
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
			fprintf(stderr, "invalid rambanks count\n");
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
			fprintf(stderr, "unsupported rom type\n");
			return NULL; //unsupported
		case 0x9: /* rom + ram + battery */
			fprintf(stderr, "unsupported rom type\n");
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
			fprintf(stderr, "unknown rom type: %d\n", ((uint8_t*)data)[0x147]);
			return NULL;
	}

	mbc_t *mbc = malloc(sizeof(*mbc));
	if (mbc == NULL)
	{
		fprintf(stderr, "allocation failed\n");
		return NULL;
	}

	mbc->data = calloc(len, 1);
	if (mbc->data == NULL)
	{
		fprintf(stderr, "allocation failed\n");
		free(mbc);
		return NULL;
	}

	memcpy(mbc->data, data, len);
	mbc->size = len;
	mbc->rambanksnb = rambanks;
	mbc->rombanksnb = rombanks;
	mbc->type = mbctype;
	mbc->options = options;
	mbc->rambanks = calloc(1, 0x2000 * rambanks);
	if (!mbc->rambanks)
	{
		fprintf(stderr, "allocation failed\n");
		return NULL;
	}
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
		case MBC3:
			if (ref->addr < 0x2000)
			{
				fprintf(stderr, "mbc[%04x] = %02x\n", ref->addr, v);
				mbc->ramenabled = ((v & 0x0F) == 0xA);
			}
			else
			{
				mbc->rombank = v & 0x7F;
				if (mbc->rombank > 0)
					mbc->rombank--;
			}
			return;
		case MBC2:
			if (ref->addr & 0x100)
			{
				mbc->ramenabled = ((v & 0x0F) == 0xA);
			}
			else
			{
				mbc->rombank = v & 0xF;
				if (mbc->rombank != 0)
					mbc->rombank--;
			}
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
		{
			uint8_t rombank = mbc->rombank;
			if (rombank >= mbc->rombanksnb)
			{
				fprintf(stderr, "invalid rom bank: %x / %x", rombank, mbc->rombanksnb);
				return 0;
			}
			uint32_t addr = ref->addr + 0x4000 * rombank;
			if (addr < mbc->size)
				return mbc->data[addr];
			fprintf(stderr, "invalid rom size: %x / %x", addr, (unsigned)mbc->size);
			return 0;
		}
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
		case MBC3:
			fprintf(stderr, "mbc[%04x] = %02x\n", ref->addr, v);
			if (ref->addr < 0x6000)
				mbc->rambank = v;
			else
				return; //XXX: latch clock
			return;
	}
}

static uint8_t ram_get(mem_ref_t *ref)
{
	mbc_t *mbc = (mbc_t*)ref->udata;
	if (!(mbc->options & MBC_OPT_RAM))
		return 0;
	switch (mbc->type)
	{
		case MBC3:
			if (mbc->rambank > 0x3)	
			{
				fprintf(stderr, "unsupported clock get\n");
				return 0; //XXX: clock
			}
			break;
		default:
			break;
	}
	if (mbc->rambank >= mbc->rambanksnb)
	{
		fprintf(stderr, "invalid ram bank: %x / %x\n", mbc->rambank, mbc->rambanksnb);
		return 0;
	}
	return mbc->rambanks[ref->addr - 0xA000 + 0x2000 * mbc->rambank];
}

static void ram_set(mem_ref_t *ref, uint8_t v)
{
	mbc_t *mbc = (mbc_t*)ref->udata;
	if (!(mbc->options & MBC_OPT_RAM))
		return;
	switch (mbc->type)
	{
		case MBC3:
			if (mbc->rambank > 0x3)
			{
				fprintf(stderr, "unsupported clock set\n");
				return;
			}
			break;
	}
	if (mbc->rambank >= mbc->rambanksnb)
	{
		fprintf(stderr, "invalid ram bank: %x / %x\n", mbc->rambank, mbc->rambanksnb);
		return;
	}
	mbc->rambanks[ref->addr - 0xA000 + 0x2000 * mbc->rambank] = v;
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
