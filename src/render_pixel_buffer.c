/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_pixel_buffer.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 15:51:35 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/21 14:57:41 by go-donne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/render.h"

/* clear screen buffer to known state for frame composition
1. calculate total pixel count (2d screen area)
2. calculate memory size (pixel count * bytes per pixel)
3. initialize all bytes to 0 (black rgba pixels) */
// void	clear_screen_buffer(void)
// {
// 	size_t	screen_pixel_count;
// 	size_t	bytes_per_pixel;
// 	size_t	total_buffer_size_bytes;

// 	screen_pixel_count = g_game.graphics.screen_width
// 		* g_game.graphics.screen_height;
// 	bytes_per_pixel = sizeof(uint32_t);
// 	total_buffer_size_bytes = screen_pixel_count * bytes_per_pixel;
// 	ft_memset(g_game.graphics.frame->pixels, 0xFF, total_buffer_size_bytes);
// }

void	clear_screen_buffer_white(void)
{
	size_t i;
	size_t pixel_count = g_game.graphics.screen_width
		* g_game.graphics.screen_height;

	uint32_t *pixels = (uint32_t *)g_game.graphics.frame->pixels;
	for (i = 0; i < pixel_count; i++)
		pixels[i] = 0xFFFFFFFF; // full white
}


/* direct pixel write to mlx42 frame buffer

endianness context:
mlx42 abstracts platform byte order through void* pixel buffer
prog parser creates argb: 0xaarrggbb (alpha, red, green, blue)
uint32_t casting ensures 32-bit atomic writes regardless of endianness
intel x86: little-endian (least significant byte first in memory)
arm: configurable endianness, usually little-endian
mlx42 handles endian conversion internally for opengl/graphics drivers

pixel format expectations:
prog parser: (0xff << 24) | (r << 16) | (g << 8) | b = 0xaarrggbb
mlx42 buffer: expects 32-bit rgba values, handles platform specifics
graphics driver: converts to gpu-native format before rendering

casting rationale:
void* buffer -> uint32_t* for atomic 32-bit pixel writes
prevents partial writes that could create visual artifacts
maintains colour channel alignment across byte boundaries 

mem layout:
Single pixel = 32 bits = 4 bytes in memory
	Byte 0: Alpha   (bits 24-31)
	Byte 1: Red     (bits 16-23) 
	Byte 2: Green   (bits 8-15)
	Byte 3: Blue    (bits 0-7)

Without uint32_t casting (dangerous):
	char *buffer = pixels;
	buffer[index*4 + 0] = alpha;   // Write 1: partial pixel state
	buffer[index*4 + 1] = red;     // Write 2: still partial  
	buffer[index*4 + 2] = green;   // Write 3: still partial
	buffer[index*4 + 3] = blue;    // Write 4: finally complete

Problem: Between writes 1-3, pixel contains mixed old/new data. 
Another process reading during this sequence gets corrupted colour.

With uint32_t casting (safe):
uint32_t *buffer = (uint32_t*)pixels;
buffer[index] = 0xAARRGGBB;
// Single atomic write: all 4 channels updated simultaneously */
void	put_pixel(int screen_x, int screen_y, int pixel_color)
{
	uint32_t	*pixel_buffer;
	int			pixel_index;

	if (screen_x < 0 || screen_x >= g_game.graphics.screen_width)
		return ;
	if (screen_y < 0 || screen_y >= g_game.graphics.screen_height)
		return ;
	pixel_buffer = (uint32_t *)g_game.graphics.frame->pixels;
	pixel_index = screen_y * g_game.graphics.screen_width + screen_x;
	pixel_buffer[pixel_index] = (uint32_t)pixel_color;
}
