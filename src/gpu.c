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

static void render_tile_dmg(gpu_t *gpu, uint16_t addr, uint8_t x, uint8_t y, uint8_t bx, uint8_t by)
{
	uint8_t coloridx = (((mem_get_vram0(gpu->mem, addr + by * 2 + 0) >> (7 - bx)) & 1) << 0)
	                 | (((mem_get_vram0(gpu->mem, addr + by * 2 + 1) >> (7 - bx)) & 1) << 1);
	static const uint8_t values[4] = {0xFF, 0xAA, 0x55, 0x00};
	uint8_t color = values[(mem_get_reg(gpu->mem, MEM_REG_BGP) >> (coloridx << 1)) & 0x3];
	memset(&gpu->data[(y * 160 + x) * 4], color, 4);
	gpu->priorities[x] = 0;
	gpu->hasprinted[x] = coloridx != 0;
}

static void render_tile_cgb(gpu_t *gpu, uint16_t addr, uint8_t x, uint8_t y, uint8_t bx, uint8_t by, uint8_t attr)
{
	uint8_t palette = attr & 0x7;
	uint8_t coloridx;
	if (attr & (1 << 5))
		bx = 7 - bx;
	if (attr & (1 << 6))
		by = 7 - by;
	if (attr & (1 << 3))
	{
		coloridx = (((mem_get_vram1(gpu->mem, addr + by * 2 + 0) >> (7 - bx)) & 1) << 0)
	             | (((mem_get_vram1(gpu->mem, addr + by * 2 + 1) >> (7 - bx)) & 1) << 1);
	}
	else
	{
		coloridx = (((mem_get_vram0(gpu->mem, addr + by * 2 + 0) >> (7 - bx)) & 1) << 0)
	             | (((mem_get_vram0(gpu->mem, addr + by * 2 + 1) >> (7 - bx)) & 1) << 1);
	}
	uint8_t *palptr = &gpu->mem->bgpalette[palette * 8 + coloridx * 2];
#define TO8(v) (((uint32_t)(v) * 527 + 23) >> 6)
	uint8_t color[4] =
	{
		TO8((palptr[1] & 0x7C) >> 2),
		TO8((palptr[0] & 0xE0 >> 5) | ((palptr[1] & 3) << 3)),
		TO8(palptr[0] & 0x1F),
		0xff,
	};
#undef TO8
	memcpy(&gpu->data[(y * 160 + x) * 4], color, 4);
	gpu->priorities[x] = (attr >> 7);
	gpu->hasprinted[x] = coloridx != 0;
}

static void render_background(gpu_t *gpu, uint8_t y)
{
	uint8_t haddr = mem_get_reg(gpu->mem, MEM_REG_LCDC) & (1 << 4);
	uint16_t baseaddr = (mem_get_reg(gpu->mem, MEM_REG_LCDC) & (1 << 3)) ? 0x9C00 : 0x9800;
	uint16_t charaddr = haddr ? 0x8000 : 0x9000;
	uint8_t scx = mem_get_reg(gpu->mem, MEM_REG_SCX);
	uint8_t scy = mem_get_reg(gpu->mem, MEM_REG_SCY);
	uint8_t ry = scy + y;
	uint8_t by = ry % 8;
	for (uint8_t x = 0; x < 160; ++x)
	{
		uint8_t rx = scx + x;
		uint8_t bx = rx % 8;
		uint8_t charcode = mem_get_vram0(gpu->mem, baseaddr + rx / 8 + ry / 8 * 32);
		uint16_t addr = charaddr + (haddr ? charcode * 16 : (int8_t)charcode * 16);
		if (gpu->mem->cgb)
			render_tile_cgb(gpu, addr, x, y, bx, by, mem_get_vram1(gpu->mem, baseaddr + rx / 8 + ry / 8 * 32));
		else
			render_tile_dmg(gpu, addr, x, y, bx, by);
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
	for (uint8_t x = 0; x < 160; ++x)
	{
		if (wx >= 7 && x < wx - 7u)
			continue;
		uint8_t rx = x - wx + 7;
		uint8_t ry = y - wy;
		uint8_t bx = rx % 8;
		uint8_t by = ry % 8;
		uint16_t charcode = mem_get_vram0(gpu->mem, baseaddr + ((rx - bx) + (ry - by) * 32) / 8);
		uint16_t addr = charaddr + (haddr ? charcode * 16 : (int8_t)charcode * 16);
		if (gpu->mem->cgb)
			render_tile_cgb(gpu, addr, x, y, bx, by, mem_get_vram1(gpu->mem, baseaddr + ((rx - bx) + (ry - by) * 32) / 8));
		else
			render_tile_dmg(gpu, addr, x, y, bx, by);
	}
}

static void render_object_dmg(gpu_t *gpu, uint8_t x, uint8_t y, uint8_t bx, uint8_t by, uint8_t charcode, uint8_t attr, bool height16)
{
	uint8_t orgbx = bx;
	bool palette = (attr >> 4) & 1;
	bool prio = (attr >> 7) & 1;
	if (attr & (1 << 5))
		bx = 7 - bx;
	if (attr & (1 << 6))
		by = (height16 ? 15 : 7) - by;
	if (height16)
		charcode &= ~1;
	uint16_t charaddr = 0x8000 + charcode * 16;
	uint8_t pixel = (((mem_get_vram0(gpu->mem, charaddr + by * 2 + 0) >> (7 - bx)) & 1) << 0)
	              | (((mem_get_vram0(gpu->mem, charaddr + by * 2 + 1) >> (7 - bx)) & 1) << 1);
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

static void render_object_cgb(gpu_t *gpu, uint8_t x, uint8_t y, uint8_t bx, uint8_t by, uint8_t charcode, uint8_t attr, bool height16)
{
	uint8_t orgbx = bx;
	bool palette = attr & 0x3;
	bool prio = (attr >> 7) & 1;
	if (attr & (1 << 5))
		bx = 7 - bx;
	if (attr & (1 << 6))
		by = (height16 ? 15 : 7) - by;
	if (height16)
		charcode &= ~1;
	uint16_t charaddr = 0x8000 + charcode * 16;
	uint8_t coloridx;
	if (attr & (1 << 3))
	{
		coloridx = (((mem_get_vram1(gpu->mem, charaddr + by * 2 + 0) >> (7 - bx)) & 1) << 0)
	             | (((mem_get_vram1(gpu->mem, charaddr + by * 2 + 1) >> (7 - bx)) & 1) << 1);
	}
	else
	{
		coloridx = (((mem_get_vram0(gpu->mem, charaddr + by * 2 + 0) >> (7 - bx)) & 1) << 0)
	             | (((mem_get_vram0(gpu->mem, charaddr + by * 2 + 1) >> (7 - bx)) & 1) << 1);
	}
	if (!coloridx)
		return;
	if ((gpu->priorities[x] || prio) && gpu->hasprinted[x])
		return;
	if (orgbx >= gpu->lowestx[x])
		return;
	uint8_t *palptr = &gpu->mem->objpalette[palette * 8 + coloridx * 2];
#define TO8(v) (((uint32_t)(v) * 527 + 23) >> 6)
	uint8_t color[4] =
	{
		TO8((palptr[1] & 0x7C) >> 2),
		TO8((palptr[0] & 0xE0 >> 5) | ((palptr[1] & 3) << 3)),
		TO8(palptr[0] & 0x1F),
		0xff,
	};
#undef TO8
	memcpy(&gpu->data[(y * 160 + x) * 4], color, 4);
	gpu->lowestx[x] = orgbx;
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
			if (tx < 8 || tx >= 168)
				continue;
			tx -= 8;
			if (gpu->mem->cgb)
				render_object_cgb(gpu, tx, y, x, y - cy + 16, charcode, cattr, height16);
			else
				render_object_dmg(gpu, tx, y, x, y - cy + 16, charcode, cattr, height16);
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
	if (gpu->mem->cgb || mem_get_reg(gpu->mem, MEM_REG_LCDC) & (1 << 0))
	{
		render_background(gpu, y);
		if (mem_get_reg(gpu->mem, MEM_REG_LCDC) & (1 << 5))
			render_window(gpu, y);
	}

	if (!gpu->mem->dmatransfer && mem_get_reg(gpu->mem, MEM_REG_LCDC) & (1 << 1))
		render_objects(gpu, y);
}
