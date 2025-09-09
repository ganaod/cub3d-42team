/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   screen_buffer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 14:04:27 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/05 13:38:33 by go-donne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Rasterization and pixel buffer operations
direct screen buffer ops */

#include "../inc/cub3d.h"

/* screen buffer management:
clear & present frame buffer ops

. calc total pixels in screen buffer
. get ptr to image data as int array for faster clearing
. clear entire screen to black (0x000000) */
void	clear_screen_buffer(void)
{
	int	total_pixels;
	int	*pixel_data;
	int	i;

	total_pixels = g_game.graphics.screen_width
					* g_game.graphics.screen_height;
	pixel_data = (int *)g_game.graphics.image_data;
	i = 0;
	while (i < total_pixels)
	{
		pixel_data[i] = 0x000000;
		i++;
	}
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