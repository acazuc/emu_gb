#ifndef MBC_H
#define MBC_H

#include "mem.h"
#include <stdint.h>
#include <stddef.h>

enum mbc_type
{
	MBC_ROM,
	MBC1,
	MBC2,
	MBC3,
	MBC5,
	MBC6,
	MBC7,
	MBC_HUC1,
	MBC_HUC3,
	MBC_MMM1,
	MBC_CAM,
	MBC_TAMA5,
};

enum mbc_option
{
	MBC_OPT_NONE    = 0x00,
	MBC_OPT_RAM     = 0x01,
	MBC_OPT_BATTERY = 0x02,
	MBC_OPT_TIMER   = 0x04,
	MBC_OPT_RUMBLE  = 0x08,
	MBC_OPT_SENSOR  = 0x10,
};

typedef struct mbc_s
{
	uint8_t *data;
	size_t size;
	enum mbc_type type;
	uint8_t rambanksnb;
	uint8_t *rambanks;
	uint8_t ramenabled;
	uint8_t rambank;
	uint8_t rombank;
	uint8_t options;
} mbc_t;

mbc_t *mbc_new(const void *data, size_t len);
void mbc_del(mbc_t *mbc);

mem_ref_t mbc_get_rom0(mbc_t *mbc, uint16_t addr);
mem_ref_t mbc_get_romn(mbc_t *mbc, uint16_t addr);
mem_ref_t mbc_get_ram(mbc_t *mbc, uint16_t addr);

#endif
