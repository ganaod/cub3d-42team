/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_pixel_buffer.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 14:04:27 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/15 18:55:10 by go-donne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

/* CLEAR SCREEN BUFFER
Initialize frame buffer to known state for additive composition
Performance: Fast memset operation, executes once per frame 

Mathematical decomposition:
1. Calculate total pixel count (2D screen area)
2. Calculate bytes per pixel (data type size)  
3. Calculate total buffer size (pixel count × bytes per pixel)
4. Initialize all bytes to 0 (black pixels in RGBA format) */
void	clear_screen_buffer(void)
{
	size_t	screen_pixel_count;
	size_t	bytes_per_pixel;
	size_t	total_buffer_size_bytes;

	// Step 1: Calculate total pixels (2D area)
	screen_pixel_count = g_game.graphics.screen_width * g_game.graphics.screen_height;
	
	// Step 2: Calculate memory per pixel (RGBA32 format)
	bytes_per_pixel = sizeof(uint32_t);
	
	// Step 3: Calculate total memory required
	total_buffer_size_bytes = screen_pixel_count * bytes_per_pixel;
	
	// Step 4: Initialize all memory to 0 (black pixels)
	ft_memset(g_game.graphics.frame->pixels, 0, total_buffer_size_bytes);
}

/* direct pixel access to image buffer */
void	put_pixel(int screen_x, int screen_y, int pixel_color)
{
	int		screen_pixel_index;

	// Bounds checking
	if (screen_x < 0 || screen_x >= g_game.graphics.screen_width ||
		screen_y < 0 || screen_y >= g_game.graphics.screen_height)
		return;

	// Calculate pixel position in buffer
	screen_pixel_index = screen_y * g_game.graphics.screen_width + screen_x;

	// Set pixel color in MLX42 frame buffer
	g_game.graphics.frame->pixels[screen_pixel_index] = pixel_color;
}
