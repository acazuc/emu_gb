#ifndef MEM_H
#define MEM_H

#include <stdbool.h>
#include <stdint.h>

#define MEM_REG_JOYP 0xFF00
#define MEM_REG_DIV  0xFF04
#define MEM_REG_IF   0xFF0F
#define MEM_REG_LCDC 0xFF40
#define MEM_REG_STAT 0xFF41
#define MEM_REG_SCY  0xFF42
#define MEM_REG_SCX  0xFF43
#define MEM_REG_LY   0xFF44
#define MEM_REG_LYC  0xFF45
#define MEM_REG_DMA  0xFF46
#define MEM_REG_BGP  0xFF47
#define MEM_REG_OBP0 0xFF48
#define MEM_REG_OBP1 0xFF49
#define MEM_REG_WY   0xFF4A
#define MEM_REG_WX   0xFF4B
#define MEM_REG_BOOT 0xFF50
#define MEM_REG_IE   0xFFFF

typedef struct mbc_s mbc_t;

typedef struct mem_s
{
	uint8_t bios[0x100];
	uint8_t vram[0x2000];
	uint8_t workram0[0x1000];
	uint8_t workram1[0x1000];
	uint8_t oam[0xA0];
	uint8_t highram[0x100];
	uint8_t joyp;
	uint8_t dmatransfer;
	mbc_t *mbc;
} mem_t;

typedef struct mem_ref_s mem_ref_t;
typedef uint8_t (*mem_ref_get_t)(mem_ref_t *ref);
typedef void (*mem_ref_set_t)(mem_ref_t *ref, uint8_t v);

struct mem_ref_s
{
	union
	{
		uint8_t *ptr;
		uint16_t addr;
	};
	mem_ref_get_t get;
	mem_ref_set_t set;
	void *udata;
};

#define MEM_REF_NULL() ((mem_ref_t){.ptr = NULL, NULL, NULL, NULL})
#define MEM_REF_PTR(p, get, set) ((mem_ref_t){.ptr = p, get, set, NULL})
#define MEM_REF_ADDR(a, get, set, udata) ((mem_ref_t){.addr = a, get, set, udata})

mem_t *mem_new(mbc_t *mbc);
void mem_del(mem_t *mem);

void mem_set_joyp(mem_t *mem, uint8_t joyp);

void mem_dmatransfer(mem_t *mem);

uint8_t mem_get_reg(mem_t *mem, uint16_t addr);
void    mem_set_reg(mem_t *mem, uint16_t addr, uint8_t v);

uint8_t mem_get8(mem_t *mem, uint16_t addr);
void    mem_set8(mem_t *mem, uint16_t addr, uint8_t v);

uint16_t mem_get16(mem_t *mem, uint16_t addr);
void     mem_set16(mem_t *mem, uint16_t addr, uint16_t v);

#endif
