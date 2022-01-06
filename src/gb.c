#include "gb.h"
#include "rom.h"
#include "mem.h"
#include "z80.h"
#include "gpu.h"
#include "apu.h"

#include <stdlib.h>

struct gb_s
{
	rom_t *rom;
	mem_t *mem;
	z80_t *z80;
	gpu_t *gpu;
	apu_t *apu;
};

gb_t *gb_new(const void *rom_data, size_t rom_size)
{
	gb_t *gb = calloc(sizeof(*gb), 1);
	if (gb == NULL)
		return NULL;

	gb->rom = rom_new(rom_data, rom_size);
	if (!gb->rom)
		return NULL;

	gb->mem = mem_new(gb->rom);
	if (!gb->mem)
		return NULL;

	gb->z80 = z80_new(gb->mem);
	if (!gb->z80)
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
	z80_del(gb->z80);
	mem_del(gb->mem);
	rom_del(gb->rom);
	free(gb);
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
		audio_buf[i] = ((int16_t)rand()) / 20; //not too loud

	for (size_t i = 0; i < 4 * 20; ++i)
		z80_clock(gb->z80);
}
