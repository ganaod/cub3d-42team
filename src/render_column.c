/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_column.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 10:54:44 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/25 12:49:46 by go-donne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/render.h"

/* perspective projection boundary protection
ultra-close walls create mathematically valid
but unrenderable coords:
. wall h exceed screen: wall start -ve
. wall start above screen top: clamp to pixel row 0
. wall start below screen bottom: clamp to screen h
prevents ray access violations while
maintaining 3D perspective accuracy */
void	render_ceiling_section(t_game *g, int screen_column_x,
		int wall_start_y_pixel)
{
	int			y;
	int			h;
	uint32_t	color;

	if (!g || !g->graphics.frame)
		return ;
	h = g->graphics.screen_height;
	if (wall_start_y_pixel < 0)
		wall_start_y_pixel = 0;
	if (wall_start_y_pixel > h)
		wall_start_y_pixel = h;
	color = g->map.ceiling_color;
	y = 0;
	while (y < wall_start_y_pixel)
	{
		put_pixel(g, screen_column_x, y, color);
		y++;
	}
}

/* 3 phases:
. setup: validate txt data, calc horiz position 
(U coord) 
. stepping: pre-calculate vertical increment 
1 division per col
. render: apply texture using incremental addition
per pixel 

stepping eliminates ~1-500 divisions per col 
   Old: v = (y - start) / span  [division every pixel]
   New: v += increment          [addition every pixel] */
void	render_wall_section(t_game *g,
			t_screen_column_bounds *screen_bounds,
			t_ray_result *wall_hit_data)
{
	t_texture_step_data	data;

	if (!setup_wall_texture_data(g, screen_bounds, wall_hit_data, &data))
		return ;
	calculate_texture_stepping(&data);
	render_textured_pixels(g, screen_bounds->column_x, &data);
}

void	render_floor_section(t_game *g, int screen_column_x,
		int wall_end_y_pixel)
{
	int	y;

	y = wall_end_y_pixel;
	while (y < g->graphics.screen_height)
	{
		put_pixel(g, screen_column_x, y, g->map.floor_color);
		y++;
	}
}
