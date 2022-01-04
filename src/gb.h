#ifndef GB_H
#define GB_H

#include <stddef.h>
#include <stdint.h>

typedef struct gb_s gb_t;

enum gb_button
{
	GB_BUTTON_LEFT   = (1 << 0),
	GB_BUTTON_RIGHT  = (1 << 1),
	GB_BUTTON_UP     = (1 << 2),
	GB_BUTTON_DOWN   = (1 << 3),
	GB_BUTTON_A      = (1 << 4),
	GB_BUTTON_B      = (1 << 5),
	GB_BUTTON_START  = (1 << 6),
	GB_BUTTON_SELECT = (1 << 7),
};

gb_t *gb_new(const void *rom_data, size_t rom_size);
void gb_del(gb_t *gb);
void gb_frame(gb_t *gb, uint8_t *video_buf, int16_t *audio_buf, size_t audio_buf_size, uint32_t joypad);

#endif
