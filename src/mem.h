#ifndef MEM_H
#define MEM_H

#include "rom.h"
#include <stdint.h>

#define MEM_REG_JOYP 0xFF00
#define MEM_REG_IF   0xFF0F
#define MEM_REG_LCDC 0xFF40
#define MEM_REG_SCY  0xFF42
#define MEM_REG_SCX  0xFF43
#define MEM_REG_LY   0xFF44
#define MEM_REG_WY   0xFF4A
#define MEM_REG_WX   0xFF4B
#define MEM_REG_RBK  0xFF50
#define MEM_REG_IE   0xFFFF

typedef struct mem_s
{
	uint8_t bios[0x100];
	uint8_t vram[0x2000];
	uint8_t workram0[0x1000];
	uint8_t workram1[0x1000];
	uint8_t highram[0x100];
	rom_t *rom;
} mem_t;

mem_t *mem_new(rom_t *rom);
void mem_del(mem_t *mem);

uint8_t mem_gu8(mem_t *mem, uint16_t addr);
void    mem_su8(mem_t *mem, uint16_t addr, uint8_t v);
void    mem_xu8(mem_t *mem, uint16_t addr, uint8_t v);
void    mem_ou8(mem_t *mem, uint16_t addr, uint8_t v);
void    mem_au8(mem_t *mem, uint16_t addr, uint8_t v);

int8_t mem_gi8(mem_t *mem, uint16_t addr);
void   mem_si8(mem_t *mem, uint16_t addr, int8_t v);
void   mem_xi8(mem_t *mem, uint16_t addr, int8_t v);
void   mem_oi8(mem_t *mem, uint16_t addr, int8_t v);
void   mem_ai8(mem_t *mem, uint16_t addr, int8_t v);

uint16_t mem_gu16(mem_t *mem, uint16_t addr);
void     mem_su16(mem_t *mem, uint16_t addr, uint16_t v);
void     mem_xu16(mem_t *mem, uint16_t addr, uint16_t v);
void     mem_ou16(mem_t *mem, uint16_t addr, uint16_t v);
void     mem_au16(mem_t *mem, uint16_t addr, uint16_t v);

#endif
