/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 17:04:34 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/05 17:11:39 by go-donne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

/* responsibility:
. Implement core raycasting mathematics
. Create 3D visual illusion from 2D map data
. Manage rendering pipeline from player view to pixels
. Handle wall projection and texture mapping
	 

execution pipeline:

render.c (render_complete_frame)
	↓
render.c (render_single_column)  
	↓
ray_math.c (calculate_ray_direction)
	↓
dda.c (cast_ray_to_wall)
	↓
projection.c (calculate_wall_height)
	↓  
texture.c (get_wall_texture_color)
	↓
screen_buffer.c (put_pixel)

RESULT: 2D map data → 3D visual representation */

/* 1. initialise buffer
. clear screen buff to known state (black pixels)
. prepare canvas for additive composition

2. raycasting core loop
. for each screen col: cast ray, find wall, draw vert strip
. screen width determines ray count (1 ray / col)
. rays spread across player's FOV */
void	render_complete_frame(void)
{
	int	screen_x;
	int	screen_width;

	clear_screen_buffer();
	screen_width = g_game.graphics.screen_width;
	screen_x = 0;
	while (screen_x < screen_width)
	{
		render_single_column(screen_x);
		screen_x++;
	}
}

/* ray processing for 1 screen col
flow: Ray Direction → Wall Distance → Screen Height → Pixels */
void	render_single_column(int screen_x)
{
	double	ray_dir_x;
	double	ray_dir_y;
	double	wall_distance;
	int		wall_direction;

	calculate_ray_direction(screen_x, &ray_dir_x, &ray_dir_y);
	wall_distance = cast_ray_to_wall(ray_dir_x, ray_dir_y, &wall_direction);
	render_wall_column(screen_x, wall_distance, wall_direction);
}






/*
WALL RENDERING: Convert distance to screen coordinates
*/
void render_wall_column(int screen_x, int wall_height, int wall_direction)
{
    // Calculate wall strip boundaries on screen
    int wall_start_y = (screen_height - wall_height) / 2;
    int wall_end_y = wall_start_y + wall_height;

    // CEILING RENDERING: Above wall
    for (int y = 0; y < wall_start_y; y++)
        put_pixel(screen_x, y, ceiling_color);

    // WALL RENDERING: Apply texture if available
    for (int y = wall_start_y; y < wall_end_y; y++) {
        int texture_color = get_wall_texture_color(wall_direction, y, wall_height);
        put_pixel(screen_x, y, texture_color);
    }

    // FLOOR RENDERING: Below wall
    for (int y = wall_end_y; y < screen_height; y++)
        put_pixel(screen_x, y, floor_color);
}
