/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 10:54:39 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/23 11:28:35 by go-donne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/render.h"

/* rendering module

2D map data → 3D visual representation

core operations:

	1. Ray Direction:    Screen column → World ray vector
	2. Ray Intersection: Ray vector → Wall hit point + distance
	3. Projection:       Distance → Screen wall height
	4. Boundaries:       Wall height → Pixel start/end positions
	5. Texture Mapping:  Hit point + screen pixel → Texture coordinates
	6. Texture Sampling: Texture coordinates → Colour value
	7. Pixel Drawing:    Colour → Screen buffer



	





COLUMN-BASED RENDERING: MATHEMATICAL NECESSITY
Core Insight: One ray cast = One distance measurement = One entire vertical wall strip

Why Columns, Not Rows:

	Ray-to-distance mapping: Each ray gives distance to nearest wall
	Wolfenstein constraint: All walls same height → distance determines entire column height
	Perspective projection: Wall height = screen_height / distance (applies to whole column)
	Texture efficiency: Sample texture once per column, not per pixel

Visual Reality:
	Column 0: Ray → Distance 5.2 → Wall height 150px → Draw 150px vertical strip
	Column 1: Ray → Distance 4.8 → Wall height 162px → Draw 162px vertical strip
	Column 2: Ray → Distance 6.1 → Wall height 127px → Draw 127px vertical strip

Mathematical Truth: Each ray provides sufficient information to render an entire vertical screen column.
Performance: 320 ray calculations instead of 320×240 = 76,800 pixel calculations.

The column approach exploits the constraint that walls are uniform height -
one distance measurement determines the entire vertical appearance.



rendering execution pipeline:

	render_complete_frame()           // Frame iteration
		render_single_column()        // Ray → geometry coordination
			calculate_ray_direction() // Screen → ray transform
			cast_ray_to_wall()        // Ray → intersection
			calculate_screen_wall_height() // Distance → pixels
			render_wall_column()      // Visual composition
				render_wall_section() // Texture context + pixel iteration
					get_wall_texture_colour() // UV calculation + sampling
						put_pixel()   // Buffer write
*/


/* 1. initialise buffer
. clear screen buff to known state (black pixels)
. prepare canvas for additive composition

2. raycasting core loop
. read screen_width from graphics sys, process that many cols
. for each screen col: cast ray, find wall, draw vert strip
. screen width determines ray count (1 ray / col)
. rays spread across player's FOV */
void	render_complete_frame(void)
{
	int	screen_column_x;
	int	screen_width;

	clear_screen_buffer();
	screen_width = g_game.graphics.screen_width;
	screen_column_x = 0;
	while (screen_column_x < screen_width)
	{
		render_single_column(screen_column_x);
		screen_column_x++;
	}
}

/* SINGLE COLUMN RENDERING PIPELINE

column-based rendering
		Why: Ray casting gives distance per column, not per pixel
		Mathematical necessity: One ray → one distance → entire vertical strip

Core raycasting sequence for one vertical screen strip

Mathematical transformations:
	1. Ray Direction:    Screen column → World ray vector
	2. Ray Intersection: Ray vector → Wall hit point + distance
	3. Projection:       Distance → Screen wall height
	4. Rendering:        Wall data → Textured vertical pixels

Input: Screen column index [0 to screen_width-1]
Output: One complete vertical strip in frame buffer

demos - drop-in replacement:
calculate_ray_direction_tunnel_vision(screen_column_x, &world_ray_direction_x, &world_ray_direction_y);
calculate_ray_direction_narrow_fov(screen_column_x, &world_ray_dir_x, &world_ray_dir_y); */
void	render_single_column(int screen_column_x)
{
	double			world_ray_direction_x;
	double			world_ray_direction_y;
	t_ray_result	wall_intersection_data;
	int				projected_wall_height;

	calculate_ray_direction(screen_column_x, &world_ray_direction_x, &world_ray_direction_y);
	wall_intersection_data = cast_ray_to_wall(world_ray_direction_x, world_ray_direction_y);
	projected_wall_height = 
		calculate_screen_wall_height(wall_intersection_data.world_perpendicular_distance);
	render_wall_column(screen_column_x, &wall_intersection_data, projected_wall_height);
}

/* render wall: 1 complete vertical strip/slice of the 3D perspective view

bridge between the continuous world distance and the discrete screen pixels
receives ray intersection data, adds wall_height, passes both forward */

/* WALL COLUMN RENDERING COORDINATION
Manages complete vertical strip composition of the 3D perspective view:

Visual structure:
	├── Ceiling section (solid color, top to wall_start)
	├── Wall section (textured, wall_start to wall_end)
	└── Floor section (solid color, wall_end to bottom)

bridge between the continuous world distance and the discrete screen pixels
receives ray intersection data, adds wall_height, passes both forward

Mathematical boundaries:
	wall_start = (screen_height - wall_height) / 2
	wall_end = wall_start + wall_height */
void	render_wall_column(int screen_column_x, t_ray_result *wall_intersection_data,
		int projected_wall_height)
{
	int	screen_wall_start_y_pixel;
	int	screen_wall_end_y_pixel;

	// Calculate wall boundaries for centered projection
	screen_wall_start_y_pixel = (g_game.graphics.screen_height
		- projected_wall_height) / 2;
	screen_wall_end_y_pixel = screen_wall_start_y_pixel + projected_wall_height;
	render_ceiling_section(screen_column_x, screen_wall_start_y_pixel);
	render_wall_section(screen_column_x, screen_wall_start_y_pixel,
		screen_wall_end_y_pixel, wall_intersection_data);
	render_floor_section(screen_column_x, screen_wall_end_y_pixel);
}
