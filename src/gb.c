#include "gb.h"
#include "mbc.h"
#include "mem.h"
#include "cpu.h"
#include "gpu.h"
#include "apu.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

gb_t *gb_new(const void *mbc_data, size_t mbc_size)
{
	gb_t *gb = calloc(sizeof(*gb), 1);
	if (gb == NULL)
	{
		fprintf(stderr, "allocation failed\n");
		return NULL;
	}

	gb->mbc = mbc_new(mbc_data, mbc_size);
	if (!gb->mbc)
	{
		fprintf(stderr, "can't create mbc\n");
		return NULL;
	}

	gb->mem = mem_new(gb, gb->mbc);
	if (!gb->mem)
	{
		fprintf(stderr, "can't create memory\n");
		return NULL;
	}

	gb->cpu = cpu_new(gb->mem);
	if (!gb->cpu)
	{
		fprintf(stderr, "can't create cpu\n");
		return NULL;
	}

	gb->gpu = gpu_new(gb->mem);
	if (!gb->gpu)
	{
		fprintf(stderr, "can't create gpu\n");
		return NULL;
	}

	gb->apu = apu_new(gb->mem);
	if (!gb->apu)
	{
		fprintf(stderr, "can't create apu\n");
		return NULL;
	}

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

static void timer_clock(gb_t *gb)
{
	if (gb->timerint && !--gb->timerint)
	{
		mem_set_reg(gb->mem, MEM_REG_TIMA, mem_get_reg(gb->mem, MEM_REG_TMA));
		mem_set_reg(gb->mem, MEM_REG_IF, mem_get_reg(gb->mem, MEM_REG_IF) | (1 << 2));
		gb->timerint = false;
	}

	gb->mem->timer++;
	uint8_t tac = mem_get_reg(gb->mem, MEM_REG_TAC);
	uint16_t tmp;
	if (tac & (1 << 2))
	{
		static const uint16_t masks[] = {1 << 9, 1 << 3, 1 << 5, 1 << 7};
		tmp = gb->mem->timer & masks[tac & 0x3];
	}
	else
	{
		tmp = 0;
	}

	if (!tmp && gb->lasttimer)
	{
		uint8_t tima = mem_get_reg(gb->mem, MEM_REG_TIMA);
		if (tima == 0xff)
			gb->timerint = 0x4;
		mem_set_reg(gb->mem, MEM_REG_TIMA, tima + 1);
	}

	gb->lasttimer = tmp;
}

static void gb_clock(gb_t *gb, size_t cycles)
{
	for (size_t i = 0; i < cycles; ++i)
	{
		if (gb->mem->doublespeed)
			timer_clock(gb);
		timer_clock(gb);

		if (gb->mem->doublespeed)
			cpu_clock(gb->cpu);
		cpu_clock(gb->cpu);

		apu_clock(gb->apu);
	}
}

void gb_frame(gb_t *gb, uint8_t *video_buf, int16_t *audio_buf, uint32_t joypad)
{
	gb->frame++;

	if (gb->mem->joyp != joypad)
	{
		gb->mem->joyp = joypad;
		mem_set_reg(gb->mem, MEM_REG_IF, mem_get_reg(gb->mem, MEM_REG_IF) | (1 << 4));
	}

	gb->gpu->windowlines = 0;

	for (size_t y = 0; y < 144; ++y)
	{
		if (mem_get_reg(gb->mem, MEM_REG_LCDC) & (1 << 7))
		{
			uint8_t lyc = mem_get_reg(gb->mem, MEM_REG_LYC);
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
		}

		gb_clock(gb, 80);

		if (mem_get_reg(gb->mem, MEM_REG_LCDC) & (1 << 7))
		{
			/* mode 3: lcd data */
			mem_set_reg(gb->mem, MEM_REG_STAT, (mem_get_reg(gb->mem, MEM_REG_STAT) & (~0x3)) | 3);
		}

		gb_clock(gb, 172);

		gpu_render_line(gb->gpu, y);

		if (mem_get_reg(gb->mem, MEM_REG_LCDC) & (1 << 7))
		{
			/* mode 0: hblank */
			mem_hdmatransfer(gb->mem);
			mem_set_reg(gb->mem, MEM_REG_STAT, (mem_get_reg(gb->mem, MEM_REG_STAT) & (~0x3)) | 0);
			if (mem_get_reg(gb->mem, MEM_REG_STAT) & (1 << 3))
				mem_set_reg(gb->mem, MEM_REG_IF, mem_get_reg(gb->mem, MEM_REG_IF) | (1 << 1));
		}

		gb_clock(gb, 204);
	}

	if (mem_get_reg(gb->mem, MEM_REG_LCDC) & (1 << 7))
	{
		/* mode 1: vblank */
		mem_set_reg(gb->mem, MEM_REG_STAT, (mem_get_reg(gb->mem, MEM_REG_STAT) & (~0x3)) | 1);
		if (mem_get_reg(gb->mem, MEM_REG_STAT) & (1 << 4))
			mem_set_reg(gb->mem, MEM_REG_IF, mem_get_reg(gb->mem, MEM_REG_IF) | (1 << 1));
		mem_set_reg(gb->mem, MEM_REG_IF, mem_get_reg(gb->mem, MEM_REG_IF) | (1 << 0));
	}

	for (size_t y = 144; y < 154; ++y)
	{
		if (mem_get_reg(gb->mem, MEM_REG_LCDC) & (1 << 7))
		{
			uint8_t lyc = mem_get_reg(gb->mem, MEM_REG_LYC);
			mem_set_reg(gb->mem, MEM_REG_LY, y);

			if ((mem_get_reg(gb->mem, MEM_REG_STAT) & (1 << 6)) && lyc == y)
				mem_set_reg(gb->mem, MEM_REG_IF, mem_get_reg(gb->mem, MEM_REG_IF) | (1 << 1));

			if (lyc == y)
				mem_set_reg(gb->mem, MEM_REG_STAT, mem_get_reg(gb->mem, MEM_REG_STAT) | (1 << 2));
			else
				mem_set_reg(gb->mem, MEM_REG_STAT, mem_get_reg(gb->mem, MEM_REG_STAT) & ~(1 << 2));
		}

		gb_clock(gb, 456);
	}

	memcpy(video_buf, gb->gpu->data, 160 * 144 * 4);
	memcpy(audio_buf, gb->apu->data, sizeof(gb->apu->data));
}
