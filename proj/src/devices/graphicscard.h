#ifndef GRAPHICSCARD_H
#define GRAPHICSCARD_H

#include <stdint.h>

/**
 * @brief This function is used to get information about a grapichs mode being used
 * @param mode Mode to get the information about
 * @param v pointer to store the information about the mode
 * @return 0 upon success, else 1
 */
int (vbe_get_mode_information)(uint16_t mode, vbe_mode_info_t *v);

/**
 * @brief Starts an given graphics mode and map memory to it
 * @param mode Graphic mode to initiate
 */
void * (vg_init)(uint16_t mode);

/**
 * @brief Makes a certain pixel based on its coordinates a certain color
 * @param x Horizontal coordinate
 * @param y Vertical coordinate
 * @param color Color to fill int the video memory
 * 
 * @return 0 upon success, else 1.
 */
int (color_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Using the function color_pixel, colors a line of pixels a certain color
 * 
 * @param x Horizontal coordinate
 * @param y Vertical coordinate
 * @param len Length of the line to draw
 * @param color Color to fill the rectangle
 * 
 * @return 0 upon success, else 1.
 */
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Using the function vg_draw_line, colors a group of lines a certain color to form a rectangle
 * 
 * @param x Horizontal coordinate
 * @param y Vertical coordinate
 * @param width Width of the rectangle to draw
 * @param height Height of the rectangle to draw
 * @param color Color to fill the rectangle
 * 
 * @return 0 upon success, else 1.
 */

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief Draws an image from an xmp
 * 
 */
int (vg_draw_xpm)(uint16_t x, uint16_t y, xpm_image_t img, uint8_t *map);

/**
 * @brief Makes all pixels black
 * 
 */
void (vg_clear_screen)();

/**
 * @brief replaces the content from the video memory by the contents from the vide buffer. Sets the * video buffer to 0.
 * 
 */
void (move_buffer)();

#endif
