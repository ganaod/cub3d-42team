/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 17:04:34 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/07 11:47:03 by go-donne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

/* execution pipeline:

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
(compose final image by adding visual elements (ceiling + wall + floor))

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
	double			ray_dir_x;
	double			ray_dir_y;
	t_column_render	col;
	
	calculate_ray_direction(screen_x, &ray_dir_x, &ray_dir_y);
	col.wall_distance = cast_ray_to_wall(ray_dir_x, ray_dir_y, &col.wall_direction);
	col.wall_height = calculate_screen_wall_height(col.wall_distance);
	render_wall_column(screen_x, col.wall_distance, col.wall_direction);
}

// STILL NEEDS TO CALL TEXTURING SUB-MODULE
/* render wall: convert dist to screen coordinates */
void	render_wall_column(int screen_x, int wall_height, int wall_direction)
{
	int	wall_start_y;
	int	wall_end_y;

	wall_start_y = (g_game.graphics.screen_height - wall_height) / 2;
	wall_end_y = wall_start_y + wall_height;
	render_ceiling_section(screen_x, wall_start_y);
	render_wall_section(screen_x, wall_start_y, wall_end_y, wall_direction);
	render_floor_section(screen_x, wall_end_y);
}
/* vertically center the wall with ceiling above & floor below
to simulate eye-level perspective:

screen coordinate sys:
y=0    ┌─────────────────┐  ← Top of screen
       │    CEILING      │
       │                 │
y=284  ├─────────────────┤  ← wall_start_y
       │                 │
       │      WALL       │  ← Wall centered on screen
       │                 │
y=484  ├─────────────────┤  ← wall_end_y  
       │                 │
       │     FLOOR       │
       │                 │
y=767  └─────────────────┘  ← Bottom of screen

screen_height = 768
wall_height = 200
wall_start_y = (768 - 200) / 2 = 284   // Center the wall
wall_end_y = 284 + 200 = 484           // Wall bottom edge

Rendering Order (top-down):
	y = 0 → 283: Paint ceiling pixels
	y = 284 → 483: Paint wall texture pixels
	y = 484 → 767: Paint floor pixels	*/
