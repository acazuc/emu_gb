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
	uint8_t nr11 = mem_get_reg(apu->mem, MEM_REG_NR11);
	uint8_t nr12 = mem_get_reg(apu->mem, MEM_REG_NR12);
	uint8_t nr13 = mem_get_reg(apu->mem, MEM_REG_NR13);
	uint8_t nr14 = mem_get_reg(apu->mem, MEM_REG_NR14);
	float freq = 131072. / (2048 - (nr13 | ((nr14 & 0x7) << 8)));
	uint32_t p = (apu->sample / (float)APU_FRAME_SAMPLES) * (freq / 59.72750056960583276373) * 2;
	uint8_t duty = (nr11 & 0xC0) >> 6;
	uint8_t gain = nr12 & 0xF0;
	gain |= gain >> 4;
	switch (duty)
	{
		case 0:
			return ((p & 0x7) == 0) ? gain : 0;
		case 1:
			return ((p & 0x3) == 0) ? gain : 0;
		case 2:
			return ((p & 0x1) == 0) ? gain : 0;
		case 3:
			return ((p & 0x3) != 0) ? gain : 0;
	}
	return 0;
}

static uint8_t channel1(apu_t *apu)
{
	uint8_t nr21 = mem_get_reg(apu->mem, MEM_REG_NR21);
	uint8_t nr22 = mem_get_reg(apu->mem, MEM_REG_NR22);
	uint8_t nr23 = mem_get_reg(apu->mem, MEM_REG_NR23);
	uint8_t nr24 = mem_get_reg(apu->mem, MEM_REG_NR24);
	float freq = 131072. / (2048 - (nr23 | ((nr24 & 0x7) << 8)));
	uint32_t p = (apu->sample / (float)APU_FRAME_SAMPLES) * (freq / 59.72750056960583276373) * 2;
	uint8_t duty = (nr21 & 0xC0) >> 6;
	uint8_t gain = nr22 & 0xF0;
	gain |= gain >> 4;
	switch (duty)
	{
		case 0:
			return ((p & 0x7) == 0) ? gain : 0;
		case 1:
			return ((p & 0x3) == 0) ? gain : 0;
		case 2:
			return ((p & 0x1) == 0) ? gain : 0;
		case 3:
			return ((p & 0x3) != 0) ? gain : 0;
	}
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
	uint8_t nr50 = mem_get_reg(apu->mem, MEM_REG_NR50);
	uint8_t nr51 = mem_get_reg(apu->mem, MEM_REG_NR51);
	uint16_t l = 0;
	uint16_t r = 0;
	//if (nr52 & (1 << 0))
	{
		uint8_t v = channel0(apu) >> 2;
		if (nr51 & (1 << 4))
			l += v;
		if (nr51 & (1 << 0))
			r += v;
	}
	//if (nr52 & (1 << 1))
	{
		uint8_t v = channel1(apu) >> 2;
		if (nr51 & (1 << 5))
			l += v;
		if (nr51 & (1 << 1))
			r += v;
	}
	//if (nr52 & (1 << 2))
	{
		uint8_t v = channel2(apu) >> 2;
		if (nr51 & (1 << 6))
			l += v;
		if (nr51 & (1 << 2))
			r += v;
	}
	//if (nr52 & (1 << 3))
	{
		uint8_t v = channel3(apu) >> 2;
		if (nr51 & (1 << 7))
			l += v;
		if (nr51 & (1 << 3))
			r += v;
	}
	l *= (nr50 & 0x70) >> 4;
	r *= (nr50 & 0x07) >> 0;
	return APU_SAMPLE(l << 5, r << 5);
}

static void length_tick(apu_t *apu)
{
}

void apu_clock(apu_t *apu)
{
	if (apu->clock % 0x3FFF == 0)
		length_tick(apu);

	if (apu->clock % 70224 == (apu->sample % APU_FRAME_SAMPLES) * 70224 / APU_FRAME_SAMPLES)
	{
		apu->data[apu->sample % APU_FRAME_SAMPLES] = gen_sample(apu);
		apu->sample++;
	}

	apu->clock++;
}
