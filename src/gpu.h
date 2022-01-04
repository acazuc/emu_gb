#ifndef GPU_H
#define GPU_H

#include "mem.h"

#define GPU_WIDTH 160
#define GPU_HEIGHT 144

typedef struct gpu_s
{
	mem_t *mem;
} gpu_t;

gpu_t *gpu_new(mem_t *mem);
void gpu_del(gpu_t *gpu);

#endif
