#ifndef APU_H
#define APU_H

#define APU_FRAME_SAMPLES 804

#include "mem.h"

typedef struct apu_sample_s
{
	uint16_t l;
	uint16_t r;
} apu_sample_t;

typedef struct apu_s
{
	apu_sample_t data[APU_FRAME_SAMPLES];
	uint32_t sample;
	uint32_t clock;
	uint8_t wave1_swpstep;
	uint8_t wave1_swpval;
	uint8_t wave1_swpdir;
	uint8_t wave1_swpnb;
	uint8_t wave1_envstep;
	uint8_t wave1_envval;
	uint8_t wave1_envdir;
	uint8_t wave1_envnb;
	uint32_t wave1_val;
	uint32_t wave1_cnt;
	uint32_t wave1_nb;
	uint8_t wave2_envstep;
	uint8_t wave2_envval;
	uint8_t wave2_envdir;
	uint8_t wave2_envnb;
	uint32_t wave2_val;
	uint32_t wave2_cnt;
	uint32_t wave2_nb;
	uint32_t wave3_val;
	uint32_t wave3_cnt;
	uint32_t wave3_nb;
	uint8_t wave4_envstep;
	uint8_t wave4_envval;
	uint8_t wave4_envdir;
	uint8_t wave4_envnb;
	uint32_t wave4_cycle;
	uint32_t wave4_val;
	uint32_t wave4_cnt;
	uint32_t wave4_nb;
	mem_t *mem;
} apu_t;

apu_t *apu_new(mem_t *mem);
void apu_del(apu_t *apu);

void apu_clock(apu_t *apu);

void apu_start_channel1(apu_t *apu);
void apu_start_channel2(apu_t *apu);
void apu_start_channel3(apu_t *apu);
void apu_start_channel4(apu_t *apu);

#endif
