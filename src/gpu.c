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

static void render_tile(gpu_t *gpu, uint16_t addr, uint8_t x, uint8_t y, uint8_t bx, uint8_t by, uint8_t bgp)
{
	uint8_t coloridx = (((mem_get8(gpu->mem, addr + by * 2 + 0) >> (7 - bx)) & 1) << 0)
		         | (((mem_get8(gpu->mem, addr + by * 2 + 1) >> (7 - bx)) & 1) << 1);
	static const uint8_t values[4] = {0xFF, 0xAA, 0x55, 0x00};
	uint8_t color = values[(bgp >> (coloridx << 1)) & 0x3];
	memset(&gpu->data[(y * 160 + x) * 4], color, 4);
}

static void render_background(gpu_t *gpu, uint8_t y)
{
	uint8_t haddr = mem_get_reg(gpu->mem, MEM_REG_LCDC) & (1 << 4);
	uint16_t baseaddr = (mem_get_reg(gpu->mem, MEM_REG_LCDC) & (1 << 3)) ? 0x9C00 : 0x9800;
	uint16_t charaddr = haddr ? 0x8000 : 0x9000;
	uint8_t scx = mem_get_reg(gpu->mem, MEM_REG_SCX);
	uint8_t scy = mem_get_reg(gpu->mem, MEM_REG_SCY);
	uint8_t bgp = mem_get_reg(gpu->mem, MEM_REG_BGP);
	uint8_t ry = scy + y;
	uint8_t by = ry % 8;
	for (uint8_t x = 0; x < 160; ++x)
	{
		uint8_t rx = scx + x;
		uint8_t bx = rx % 8;
		uint8_t charcode = mem_get8(gpu->mem, baseaddr + rx / 8 + ry / 8 * 32);
		uint16_t addr = charaddr + (haddr ? charcode * 16 : (int8_t)charcode * 16);
		render_tile(gpu, addr, x, y, bx, by, bgp);
	}
}

static void render_window(gpu_t *gpu, uint8_t y)
{
	uint8_t wx = mem_get_reg(gpu->mem, MEM_REG_WX);
	uint8_t wy = mem_get_reg(gpu->mem, MEM_REG_WY);
	if (wx > 166 || wy > y)
		return;
	uint8_t haddr = mem_get_reg(gpu->mem, MEM_REG_LCDC) & (1 << 4);
	uint16_t baseaddr = (mem_get_reg(gpu->mem, MEM_REG_LCDC) & (1 << 6)) ? 0x9C00 : 0x9800;
	uint16_t charaddr = haddr ? 0x8000 : 0x9000;
	uint8_t bgp = mem_get_reg(gpu->mem, MEM_REG_BGP);
	for (uint8_t x = 0; x < 160; ++x)
	{
		if (wx >= 7 && x < wx - 7u)
			continue;
		uint8_t rx = x - wx + 7;
		uint8_t ry = y - wy;
		uint8_t bx = rx % 8;
		uint8_t by = ry % 8;
		uint16_t charcode = mem_get8(gpu->mem, baseaddr + ((rx - bx) + (ry - by) * 32) / 8);
		uint16_t addr = charaddr + (haddr ? charcode * 16 : (int8_t)charcode * 16);
		render_tile(gpu, addr, x, y, bx, by, bgp);
	}
}

void gpu_render_line(gpu_t *gpu, uint8_t y)
{
	if (!(mem_get_reg(gpu->mem, MEM_REG_LCDC) & (1 << 7)))
	{
		memset(&gpu->data[160 * y * 4], 0xff, 160 * 4);
		return;
	}

	if (mem_get_reg(gpu->mem, MEM_REG_LCDC) & (1 << 0))
		render_background(gpu, y);

	if (mem_get_reg(gpu->mem, MEM_REG_LCDC) & (1 << 5))
		render_window(gpu, y);
}
