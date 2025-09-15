/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_pixel_buffer.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 14:04:27 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/15 17:21:28 by go-donne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Rasterization and pixel buffer operations
direct screen buffer ops */

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

void present_frame_to_screen(void)
{
    // Display the completed frame buffer to the window
    mlx_put_image_to_window(
        g_game.graphics.mlx_ptr,
        g_game.graphics.window_ptr,
        g_game.graphics.image_ptr,
        0, 0
    );
}

/* direct pixel access to image buffer */
void put_pixel(int x, int y, int color)
{
    int pixel_index;
    int *pixel_data;

    // Bounds checking
    if (x < 0 || x >= g_game.graphics.screen_width ||
        y < 0 || y >= g_game.graphics.screen_height)
        return;

    // Calculate pixel position in buffer
    pixel_index = y * g_game.graphics.screen_width + x;

    // Get pixel data as integer array
    pixel_data = (int *)g_game.graphics.image_data;

    // Set pixel color
    pixel_data[pixel_index] = color;
}