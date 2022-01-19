#ifndef GB_H
#define GB_H

#include <stddef.h>
#include <stdint.h>

typedef struct mbc_s mbc_t;
typedef struct mem_s mem_t;
typedef struct cpu_s cpu_t;
typedef struct gpu_s gpu_t;
typedef struct apu_s apu_t;

enum gb_button
{
	GB_BUTTON_RIGHT  = (1 << 0),
	GB_BUTTON_LEFT   = (1 << 1),
	GB_BUTTON_UP     = (1 << 2),
	GB_BUTTON_DOWN   = (1 << 3),
	GB_BUTTON_A      = (1 << 4),
	GB_BUTTON_B      = (1 << 5),
	GB_BUTTON_SELECT = (1 << 6),
	GB_BUTTON_START  = (1 << 7),
};

typedef struct gb_s
{
	mbc_t *mbc;
	mem_t *mem;
	cpu_t *cpu;
	gpu_t *gpu;
	apu_t *apu;
	uint8_t lasttimer;
	uint8_t timerint;
	uint32_t frame;
} gb_t;

gb_t *gb_new(const void *rom_data, size_t rom_size);
void gb_del(gb_t *gb);

void gb_frame(gb_t *gb, uint8_t *video_buf, int16_t *audio_buf, uint32_t joypad);

#endif
