#ifndef ROM_H
#define ROM_H

#include <stdint.h>
#include <stddef.h>

typedef struct rom_s
{
	uint8_t *data;
	size_t size;
} rom_t;

rom_t *rom_new(const void *data, size_t len);
void rom_del(rom_t *rom);

uint8_t *rom_get_u8_rb0(rom_t *rom, uint16_t addr);
uint8_t *rom_get_u8_rbn(rom_t *rom, uint16_t addr);

#endif
