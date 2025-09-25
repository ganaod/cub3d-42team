/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 10:54:39 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/25 12:31:19 by go-donne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/render.h"

/* frame orchestration
clear buffer,
process each screen col */
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
Why: ray casting gives distance per column
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

/* split col into C, W, F
bounds center wall symmetrically around screen middle 
with equal F & C margins */
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
