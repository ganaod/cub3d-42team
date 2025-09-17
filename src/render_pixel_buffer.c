/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_pixel_buffer.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 14:04:27 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/17 15:16:38 by go-donne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

/* CLEAR SCREEN BUFFER
Initialize frame buffer to known state for additive composition
Performance: Fast memset operation, executes once per frame 

Mathematical decomposition:
1. Calculate total pixel count (2D screen area, RGBA32 format)
2. Calculate mem bytes per pixel (data type size)  
3. Calculate total buffer mem size (pixel count × bytes per pixel)
4. Initialize all bytes to 0 (black pixels in RGBA format) */
void	clear_screen_buffer(void)
{
	size_t	screen_pixel_count;
	size_t	bytes_per_pixel;
	size_t	total_buffer_size_bytes;

	screen_pixel_count = g_game.graphics.screen_width
		* g_game.graphics.screen_height;
	bytes_per_pixel = sizeof(uint32_t);
	total_buffer_size_bytes = screen_pixel_count * bytes_per_pixel;
	ft_memset(g_game.graphics.frame->pixels, 0, total_buffer_size_bytes);
}

/* direct pixel access to image buffer 

. Calculate pixel position in buffer
. Set pixel color in MLX42 frame buffer */
void	put_pixel(int screen_x, int screen_y, int pixel_colour)
{
	int		screen_pixel_index;

	// BOUNDS PROTECTION: Prevent buffer overflow
	if (screen_x < 0 || screen_x >= (int)g_game.graphics.frame->width || 
	screen_y < 0 || screen_y >= (int)g_game.graphics.frame->height)
	return;

	int screen_pixel_index = screen_y * (int)g_game.graphics.frame->width + screen_x;
    g_game.graphics.frame->pixels[screen_pixel_index] = pixel_color;
}
