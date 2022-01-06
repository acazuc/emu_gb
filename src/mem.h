#ifndef MEM_H
#define MEM_H

#include "rom.h"
#include <stdint.h>

typedef struct mem_s
{
	rom_t *rom;
} mem_t;

mem_t *mem_new(rom_t *rom);
void mem_del(mem_t *mem);

uint8_t mem_get_u8(mem_t *mem, uint16_t addr);
 int8_t mem_get_i8(mem_t *mem, uint16_t addr);
uint16_t mem_get_u16(mem_t *mem, uint16_t addr);
void mem_set_u8(mem_t *mem, uint16_t addr, uint8_t value);
void mem_set_i8(mem_t *mem, uint16_t addr,  int8_t value);
void mem_set_u16(mem_t *mem, uint16_t addr, uint16_t value);

#endif
