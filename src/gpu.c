#include "gpu.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

gpu_t *gpu_new(mem_t *mem)
{
	gpu_t *gpu = calloc(sizeof(*gpu), 1);
	if (!gpu)
	{
		fprintf(stderr, "allocation failed\n");
		return NULL;
	}

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
	uint8_t coloridx = (((mem_get_vram(gpu->mem, addr + by * 2 + 0) >> (7 - bx)) & 1) << 0)
	                 | (((mem_get_vram(gpu->mem, addr + by * 2 + 1) >> (7 - bx)) & 1) << 1);
	static const uint8_t values[4] = {0xFF, 0xAA, 0x55, 0x00};
	uint8_t color = values[(bgp >> (coloridx << 1)) & 0x3];
	memset(&gpu->data[(y * 160 + x) * 4], color, 4);
	gpu->priorities[x] = 0;
	gpu->hasprinted[x] = coloridx != 0;
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
		uint8_t charcode = mem_get_vram(gpu->mem, baseaddr + rx / 8 + ry / 8 * 32);
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
		uint16_t charcode = mem_get_vram(gpu->mem, baseaddr + ((rx - bx) + (ry - by) * 32) / 8);
		uint16_t addr = charaddr + (haddr ? charcode * 16 : (int8_t)charcode * 16);
		render_tile(gpu, addr, x, y, bx, by, bgp);
	}
}

static void render_object(gpu_t *gpu, uint8_t x, uint8_t y, uint8_t bx, uint8_t by, uint8_t charcode, uint8_t attr, bool height16)
{
	uint8_t orgbx = bx;
	bool palette = (attr & (1 << 4)) >> 4;
	bool hflip = (attr & (1 << 5)) >> 5;
	bool vflip = (attr & (1 << 6)) >> 6;
	bool prio = (attr & (1 << 7)) >> 7;
	if (hflip)
		bx = 7 - bx;
	if (vflip)
		by = (height16 ? 15 : 7) - by;
	if (height16)
		charcode &= ~1;
	uint16_t charaddr = 0x8000 + charcode * 16;
	uint8_t pixel = (((mem_get_vram(gpu->mem, charaddr + by * 2 + 0) >> (7 - bx)) & 1) << 0)
	              | (((mem_get_vram(gpu->mem, charaddr + by * 2 + 1) >> (7 - bx)) & 1) << 1);
	if (!pixel)
		return;
	if ((gpu->priorities[x] || prio) && gpu->hasprinted[x])
		return;
	if (orgbx >= gpu->lowestx[x])
		return;
	gpu->lowestx[x] = orgbx;
	static const uint8_t values[4] = {0xFF, 0xAA, 0x55, 0x00};
	uint8_t color = (mem_get_reg(gpu->mem, palette ? MEM_REG_OBP1 : MEM_REG_OBP0) >> (pixel << 1)) & 3;
	color = values[color];
	memset(&gpu->data[(y * 160 + x) * 4], color, 4);
}

static void render_objects(gpu_t *gpu, uint8_t y)
{
	memset(gpu->lowestx, 0xFF, sizeof(gpu->lowestx));
	bool height16 = mem_get_reg(gpu->mem, MEM_REG_LCDC) & (1 << 2);
	uint8_t spritecount = 0;
	for (uint8_t i = 0; i < 40; ++i)
	{
		uint16_t addr = 0xFE00 + 4 * i;
		uint8_t cy = mem_get_oam(gpu->mem, addr + 0);
		if (cy == 0 || cy >= 160 || cy > y + 16 || cy + (height16 ? 16 : 8) <= y + 16)
			continue;
		uint8_t cx = mem_get_oam(gpu->mem, addr + 1);
		uint8_t charcode = mem_get_oam(gpu->mem, addr + 2);
		uint8_t cattr = mem_get_oam(gpu->mem, addr + 3);
		for (uint8_t x = 0; x < 8; ++x)
		{
			uint8_t tx = cx + x;
			if (tx < 8 || tx >= 160)
				continue;
			tx -= 8;
			render_object(gpu, tx, y, x, y - cy + 16, charcode, cattr, height16);
		}
		if (++spritecount == 10)
			return;
	}
}

void gpu_render_line(gpu_t *gpu, uint8_t y)
{
	if (!(mem_get_reg(gpu->mem, MEM_REG_LCDC) & (1 << 7)))
	{
		memset(&gpu->data[160 * y * 4], 0xff, 160 * 4);
		return;
	}

	memset(gpu->priorities, 0, sizeof(gpu->priorities));
	memset(gpu->hasprinted, 0, sizeof(gpu->hasprinted));
	if (mem_get_reg(gpu->mem, MEM_REG_LCDC) & (1 << 0))
		render_background(gpu, y);

	if (mem_get_reg(gpu->mem, MEM_REG_LCDC) & (1 << 5))
		render_window(gpu, y);

	if (!gpu->mem->dmatransfer && mem_get_reg(gpu->mem, MEM_REG_LCDC) & (1 << 1))
		render_objects(gpu, y);

}
