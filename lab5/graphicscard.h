#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <lcom/lcf.h>

int (set_graphics_card_mode)(uint16_t mode);

void * (vg_init)(uint16_t mode);

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);
  
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

void from_buffer();

int(vg_draw_sprite)(uint8_t *sprite, xpm_image_t img, uint16_t x, uint16_t y);

#endif


