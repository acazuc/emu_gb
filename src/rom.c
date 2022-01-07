#include "rom.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

rom_t *rom_new(const void *data, size_t len)
{
	rom_t *rom = malloc(sizeof(*rom));
	if (rom == NULL)
		return NULL;

	rom->data = malloc(len);
	if (rom->data == NULL)
	{
		free(rom);
		return NULL;
	}

	fprintf(stderr, "%s\n", &((char*)data)[0x134]);
	memcpy(rom->data, data, len);
	rom->size = len;
	return rom;
}

void rom_del(rom_t *rom)
{
	if (rom == NULL)
		return;

	free(rom->data);
	free(rom);
}

uint8_t *rom_get_u8_rb0(rom_t *rom, uint16_t addr)
{
	if (addr < rom->size)
		return &rom->data[addr];
	return NULL;
}

uint8_t *rom_get_u8_rbn(rom_t *rom, uint16_t addr)
{
	(void)rom;
	(void)addr;
	return NULL;
}
