#include "gpu.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

gpu_t *gpu_new(mem_t *mem)
{
	gpu_t *gpu = calloc(sizeof(*gpu), 1);
	if (!gpu)
		return NULL;

	gpu->mem = mem;
	return gpu;
}

void gpu_del(gpu_t *gpu)
{
	if (!gpu)
		return;
	free(gpu);
}

static void render_bg(gpu_t *gpu, uint8_t y)
{
	uint16_t base_addr = (mem_gu8(gpu->mem, MEM_REG_LCDC) & 0x80) ? 0x9C00 : 0x9800;
	uint8_t scx = mem_gu8(gpu->mem, MEM_REG_SCX);
	uint8_t scy = mem_gu8(gpu->mem, MEM_REG_SCY);
	uint8_t ry = scy + y;
	uint8_t by = ry % 8;
	for (uint8_t x = 0; x < 160; ++x)
	{
		uint8_t rx = scx + x;
		uint8_t bx = rx % 8;
		uint16_t addr = base_addr + rx / 8 + ry / 8 * 32;
		//uint8_t charcode = mem_gu8(gpu->mem, addr);
		//if (charcode)
		//	fprintf(stderr, "rendering char %d\n", charcode);
	}
}

void gpu_render_line(gpu_t *gpu, uint8_t y)
{
	printf("scx: %x, scy: %x, wx: %x, wy: %x\n",
	       mem_gu8(gpu->mem, MEM_REG_SCX),
	       mem_gu8(gpu->mem, MEM_REG_SCY),
	       mem_gu8(gpu->mem, MEM_REG_WX),
	       mem_gu8(gpu->mem, MEM_REG_WY));
	if (!(mem_gu8(gpu->mem, MEM_REG_LCDC) & 0x80))
	{
		memset(&gpu->data[160 * y * 4], 0xff, 160 * 4);
		return;
	}

	if (mem_gu8(gpu->mem, MEM_REG_LCDC) & 0x1)
		render_bg(gpu, y);
}
