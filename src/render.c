/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 10:54:39 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/25 09:37:53 by blohrer          ###   ########.fr       */
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
	7. Pixel Drawing:    Colour → Screen buffer */
/* 1. initialise buffer
. clear screen buff to known state (black pixels)
. prepare canvas for additive composition

2. raycasting core loop
. read screen_width from graphics sys, process that many cols
. for each screen col: cast ray, find wall, draw vert strip
. screen width determines ray count (1 ray / col)
. rays spread across player's FOV */
void	render_complete_frame(t_game *g)
{
	int	x;
	int	w;

	if (!g || !g->graphics.frame)
		return ;
	clear_screen_buffer(g);
	w = g->graphics.screen_width;
	x = 0;
	while (x < w)
	{
		render_single_column(g, x);
		x++;
	}
}

/* column-based rendering
		Why: ray casting gives distance per column, not per pixel
		necessity: 1 ray → 1 distance → entire vertical strip */
void	render_single_column(t_game *g, int screen_column_x)
{
	double			world_ray_dir_x;
	double			world_ray_dir_y;
	t_ray_result	wall_intersection_data;
	int				projected_wall_height;

	calculate_ray_direction(g, screen_column_x, &world_ray_dir_x,
		&world_ray_dir_y);
	wall_intersection_data = cast_ray_to_wall(g, world_ray_dir_x,
			world_ray_dir_y);
	projected_wall_height = calculate_screen_wall_height(g,
			wall_intersection_data.world_perpendicular_distance);
	render_wall_column(g, screen_column_x, &wall_intersection_data,
		projected_wall_height);
}

/* WALL COLUMN RENDERING COORDINATION
Manages complete vertical strip composition of the 3D perspective view

bridge between the continuous world distance and the discrete screen pixels
receives ray intersection data, adds wall_height, passes both forward

Mathematical boundaries:
	wall_start = (screen_height - wall_height) / 2
	wall_end = wall_start + wall_height */
void	render_wall_column(t_game *g, int screen_column_x,
		t_ray_result *wall_intersection_data, int projected_wall_height)
{
	t_screen_column_bounds	bounds;

	bounds.column_x = screen_column_x;
	bounds.wall_start_y = (g->graphics.screen_height - projected_wall_height)
		/ 2;
	bounds.wall_end_y = bounds.wall_start_y + projected_wall_height;
	render_ceiling_section(g, screen_column_x, bounds.wall_start_y);
	render_wall_section(g, &bounds, wall_intersection_data);
	render_floor_section(g, screen_column_x, bounds.wall_end_y);
}
