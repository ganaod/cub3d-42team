/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_column.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 10:54:44 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/24 15:55:39 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* screen-frame with its 3 zones:

┌─────────────────┐
│     CEILING     │ (rays above horizontal centreline)
│                 │
├╴╴╴╴╴╴╴╴╴╴╴╴╴╴╴╴╴┤ ← screen_y = wall_start_y_pixel
│      WALL       │ (ray intersection determines height)
│                 │
├╴╴╴╴╴╴╴╴╴╴╴╴╴╴╴╴╴┤ ← screen_y = wall_end_y_pixel
│      FLOOR      │
│                 │	(rays below horizontal centreline)
└─────────────────┘

re. texture, from subject:
	"display different wall textures"
	"set the floor and ceiling to two different colours"

hence
1 helper per section, texturing
only in	render_wall_section(void) */

#include "../inc/render.h"

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

void	render_wall_section(t_game *g, int screen_column_x, int wall_start_y_pixel,
		int wall_end_y_pixel, t_ray_result *wall_hit_data)
{
	int					current_pixel_y;
	uint32_t			pixel_color;
	t_texture_context	texture_mapping_info;

	texture_mapping_info.world_wall_face = wall_hit_data->world_wall_face;
	texture_mapping_info.world_wall_intersection_x = wall_hit_data->world_intersection_x;
	texture_mapping_info.world_wall_intersection_y = wall_hit_data->world_intersection_y;
	texture_mapping_info.screen_wall_height = wall_end_y_pixel
		- wall_start_y_pixel;
	current_pixel_y = wall_start_y_pixel;
	while (current_pixel_y < wall_end_y_pixel)
	{
		pixel_color = screen_pixel_texture_colour(g, &texture_mapping_info,
				current_pixel_y);
		put_pixel(g, screen_column_x, current_pixel_y, pixel_color);
		current_pixel_y++;
	}
}

void	render_floor_section(t_game *g, int screen_column_x, int wall_end_y_pixel)
{
	int y = wall_end_y_pixel;

	while (y < g->graphics.screen_height)
	{
		put_pixel(g, screen_column_x, y, g->map.floor_color);
		y++;
	}
}
