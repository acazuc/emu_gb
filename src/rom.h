#ifndef ROM_H
#define ROM_H

#include <stddef.h>

typedef struct rom_s
{
	void *data;
	size_t size;
} rom_t;

rom_t *rom_new(const void *data, size_t len);
void rom_del(rom_t *rom);

#endif
