#include "apu.h"

#include <stdlib.h>
#include <stdio.h>

#define APU_SAMPLE(l, r) ((apu_sample_t){l, r})

apu_t *apu_new(mem_t *mem)
{
	apu_t *apu = calloc(sizeof(*apu), 1);
	if (!apu)
	{
		fprintf(stderr, "allocation failed\n");
		return NULL;
	}

	apu->mem = mem;
	return apu;
}

void apu_del(apu_t *apu)
{
	if (!apu)
		return;
	free(apu);
}

static uint8_t channel0(apu_t *apu)
{
	uint8_t nr13 = mem_get_reg(apu->mem, MEM_REG_NR13);
	uint8_t nr14 = mem_get_reg(apu->mem, MEM_REG_NR14);
	uint16_t freq = nr13 | ((nr14 & 0x7) << 8);
	freq = 131072 / (2048 - freq);
	return (apu->clock & 0xfff) < 0x800 ? 0xff : 0;
	//return ((apu->clock / 262144.) * freq) > .5 ? 0xff : 0;
}

static uint8_t channel1(apu_t *apu)
{
	return 0;
}

static uint8_t channel2(apu_t *apu)
{
	return 0;
}

static uint8_t channel3(apu_t *apu)
{
	return 0;
}

static apu_sample_t gen_sample(apu_t *apu)
{
	uint8_t nr52 = mem_get_reg(apu->mem, MEM_REG_NR52);
	if (!(nr52 & (1 << 7)))
		return APU_SAMPLE(0, 0);
	uint8_t nr51 = mem_get_reg(apu->mem, MEM_REG_NR51);
	uint8_t l = 0;
	uint8_t r = 0;
	//if (nr52 & (1 << 0))
	{
		uint8_t v = channel0(apu) >> 2;
		if (nr51 & (1 << 0))
			l += v;
		if (nr51 & (1 << 4))
			r += v;
	}
	//if (nr52 & (1 << 1))
	{
		uint8_t v = channel1(apu) >> 2;
		if (nr51 & (1 << 1))
			l += v;
		if (nr51 & (1 << 5))
			r += v;
	}
	//if (nr52 & (1 << 2))
	{
		uint8_t v = channel2(apu) >> 2;
		if (nr51 & (1 << 2))
			l += v;
		if (nr51 & (1 << 6))
			r += v;
	}
	//if (nr52 & (1 << 3))
	{
		uint8_t v = channel3(apu) >> 2;
		if (nr51 & (1 << 3))
			l += v;
		if (nr51 & (1 << 7))
			r += v;
	}
	return APU_SAMPLE(l | (l << 8), r | (r << 8));
}

void apu_clock(apu_t *apu)
{
	if (!(apu->clock & 0xF))
	{
		fprintf(stderr, "gen sample %x\n", apu->sample);
		apu->data[apu->sample++] = gen_sample(apu);
		apu->sample %= 4389;
	}

	apu->clock++;
}
