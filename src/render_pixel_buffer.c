/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_pixel_buffer.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 14:04:27 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/15 17:00:27 by go-donne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Rasterization and pixel buffer operations
direct screen buffer ops */

#include "../inc/cub3d.h"

/* CLEAR SCREEN BUFFER
Initialize frame buffer to known state for additive composition
Performance: Fast memset operation, executes once per frame */
void	clear_screen_buffer(void)
{
	ft_memset(g_game.graphics.frame->pixels, 0, 
		g_game.graphics.screen_width * g_game.graphics.screen_height * sizeof(uint32_t));
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