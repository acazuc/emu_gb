#include "gb.h"
#include "mbc.h"
#include "mem.h"
#include "cpu.h"
#include "gpu.h"
#include "apu.h"

#include <stdlib.h>
#include <string.h>

struct gb_s
{
	mbc_t *mbc;
	mem_t *mem;
	cpu_t *cpu;
	gpu_t *gpu;
	apu_t *apu;
};

gb_t *gb_new(const void *mbc_data, size_t mbc_size)
{
	gb_t *gb = calloc(sizeof(*gb), 1);
	if (gb == NULL)
		return NULL;

	gb->mbc = mbc_new(mbc_data, mbc_size);
	if (!gb->mbc)
		return NULL;

	gb->mem = mem_new(gb->mbc);
	if (!gb->mem)
		return NULL;

	gb->cpu = cpu_new(gb->mem);
	if (!gb->cpu)
		return NULL;

	gb->gpu = gpu_new(gb->mem);
	if (!gb->gpu)
		return NULL;

	gb->apu = apu_new(gb->mem);
	if (!gb->apu)
		return NULL;

	return gb;
}

void gb_del(gb_t *gb)
{
	if (!gb)
		return;

	apu_del(gb->apu);
	gpu_del(gb->gpu);
	cpu_del(gb->cpu);
	mem_del(gb->mem);
	mbc_del(gb->mbc);
	free(gb);
}

static void update_input(gb_t *gb, uint32_t joypad)
{
	if ((mem_get_reg(gb->mem, MEM_REG_JOYP) & 0x10) == 0)
	{
		mem_set_reg(gb->mem, MEM_REG_JOYP,
		            (((joypad & GB_BUTTON_RIGHT) ? 0 : 1) << 0)
		          | (((joypad & GB_BUTTON_LEFT)  ? 0 : 1) << 1)
		          | (((joypad & GB_BUTTON_UP)    ? 0 : 1) << 2)
		          | (((joypad & GB_BUTTON_DOWN)  ? 0 : 1) << 3));
	}
	else if ((mem_get_reg(gb->mem, MEM_REG_JOYP) & 0x10) == 0x10)
	{
		mem_set_reg(gb->mem, MEM_REG_JOYP,
		            (((joypad & GB_BUTTON_A)      ? 0 : 1) << 0)
		          | (((joypad & GB_BUTTON_B)      ? 0 : 1) << 1)
		          | (((joypad & GB_BUTTON_SELECT) ? 0 : 1) << 2)
		          | (((joypad & GB_BUTTON_START)  ? 0 : 1) << 3));
	}
}

void gb_frame(gb_t *gb, uint8_t *video_buf, int16_t *audio_buf, size_t audio_buf_size, uint32_t joypad)
{
	for (size_t y = 0; y < GPU_HEIGHT; ++y)
	{
		for (size_t x = 0; x < GPU_WIDTH; ++x)
		{
			size_t idx = (y * GPU_WIDTH + x)* 4;
			for (size_t i = 0; i < 4; ++i)
			{
				if (((joypad & GB_BUTTON_LEFT)  && x < GPU_WIDTH / 4)
				 || ((joypad & GB_BUTTON_RIGHT) && x > GPU_WIDTH * 3 / 4)
				 || ((joypad & GB_BUTTON_UP)    && y < GPU_HEIGHT / 4)
				 || ((joypad & GB_BUTTON_DOWN)  && y > GPU_HEIGHT * 3 / 4))
					video_buf[idx + i] = rand();
				else
					video_buf[idx + i] = 0;
			}
		}
	}

	for (size_t i = 0; i < audio_buf_size; ++i)
		audio_buf[i] = 0;

#if 0
	for (size_t i = 0; i < 4; ++i)
		cpu_clock(gb->cpu);
	return;
#endif

	for (size_t y = 0; y < 144; ++y)
	{
		uint8_t lyc = mem_get_reg(gb->mem, MEM_REG_LYC);
		update_input(gb, joypad);
		mem_set_reg(gb->mem, MEM_REG_LY, y);
		if ((mem_get_reg(gb->mem, MEM_REG_STAT) & (1 << 6)) && lyc == y)
			mem_set_reg(gb->mem, MEM_REG_IF, mem_get_reg(gb->mem, MEM_REG_IF) | (1 << 1));

		if (lyc == y)
			mem_set_reg(gb->mem, MEM_REG_STAT, mem_get_reg(gb->mem, MEM_REG_STAT) | (1 << 2));
		else
			mem_set_reg(gb->mem, MEM_REG_STAT, mem_get_reg(gb->mem, MEM_REG_STAT) & ~(1 << 2));

		/* mode 2: oam */
		mem_set_reg(gb->mem, MEM_REG_STAT, (mem_get_reg(gb->mem, MEM_REG_STAT) & (~0x3)) | 2);
		if (mem_get_reg(gb->mem, MEM_REG_STAT) & (1 << 5))
			mem_set_reg(gb->mem, MEM_REG_IF, mem_get_reg(gb->mem, MEM_REG_IF) | (1 << 1));
		for (size_t i = 0; i < 80 * 4; ++i)
			cpu_clock(gb->cpu);

		/* mode 3: lcd data */
		mem_set_reg(gb->mem, MEM_REG_STAT, (mem_get_reg(gb->mem, MEM_REG_STAT) & (~0x3)) | 3);
		for (size_t i = 0; i < 172 * 4; ++i)
			cpu_clock(gb->cpu);

		gpu_render_line(gb->gpu, y);

		/* mode 0: hblank */
		mem_set_reg(gb->mem, MEM_REG_STAT, (mem_get_reg(gb->mem, MEM_REG_STAT) & (~0x3)) | 0);
		if (mem_get_reg(gb->mem, MEM_REG_STAT) & (1 << 3))
			mem_set_reg(gb->mem, MEM_REG_IF, mem_get_reg(gb->mem, MEM_REG_IF) | (1 << 1));
		for (size_t i = 0; i < 204 * 4; ++i)
			cpu_clock(gb->cpu);
	}

	/* mode 1: vblank */
	mem_set_reg(gb->mem, MEM_REG_STAT, (mem_get_reg(gb->mem, MEM_REG_STAT) & (~0x3)) | 1);
	if (mem_get_reg(gb->mem, MEM_REG_STAT) & (1 << 4))
		mem_set_reg(gb->mem, MEM_REG_IF, mem_get_reg(gb->mem, MEM_REG_IF) | (1 << 1));
	mem_set_reg(gb->mem, MEM_REG_IF, mem_get_reg(gb->mem, MEM_REG_IF) | (1 << 0));
	for (size_t y = 144; y < 154; ++y)
	{
		uint8_t lyc = mem_get_reg(gb->mem, MEM_REG_LYC);
		mem_set_reg(gb->mem, MEM_REG_LY, y);
		if (lyc == y)
			mem_set_reg(gb->mem, MEM_REG_STAT, mem_get_reg(gb->mem, MEM_REG_STAT) | (1 << 2));
		else
			mem_set_reg(gb->mem, MEM_REG_STAT, mem_get_reg(gb->mem, MEM_REG_STAT) & ~(1 << 2));

		for (size_t i = 0; i < 465 * 4; ++i)
			cpu_clock(gb->cpu);
	}

	memcpy(video_buf, gb->gpu->data, 160 * 144 * 4);
}
