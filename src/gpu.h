#ifndef GPU_H
#define GPU_H

#include "mem.h"

#define GPU_WIDTH 160
#define GPU_HEIGHT 144

typedef struct gpu_s
{
	uint8_t data[144 * 160 * 4];
	uint8_t priorities[160];
	uint8_t hasprinted[160];
	uint8_t lowestx[160];
	uint8_t windowlines;
	uint8_t y;
	uint8_t x;
	mem_t *mem;
} gpu_t;

gpu_t *gpu_new(mem_t *mem);
void gpu_del(gpu_t *gpu);

void gpu_render_line(gpu_t *gpu, uint8_t y);

#endif
