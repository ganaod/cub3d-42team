/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_column.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 10:54:44 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/19 10:07:26 by go-donne         ###   ########.fr       */
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
only in render_wall_section() */

#include "../inc/cub3d.h"

void	render_ceiling_section(int screen_column_x, int wall_start_y_pixel)
{
	int	current_pixel_y;

	current_pixel_y = 0;
	while (current_pixel_y < wall_start_y_pixel)
	{
		put_pixel(screen_column_x, current_pixel_y, g_game.map.ceiling_color);
		current_pixel_y++;
	}
}

void	render_wall_section(int screen_column_x, int wall_start_y_pixel,
						int wall_end_y_pixel, t_ray_result *wall_hit_data)
{
	int					current_pixel_y;
	uint32_t			pixel_color;
	t_texture_context	texture_mapping_info;

	texture_mapping_info.world_wall_face = wall_hit_data->world_wall_face;
	texture_mapping_info.world_wall_intersection_x = wall_hit_data->world_intersection_x;
	texture_mapping_info.world_wall_intersection_y = wall_hit_data->world_intersection_y;
	texture_mapping_info.screen_wall_height = wall_end_y_pixel - wall_start_y_pixel;
	current_pixel_y = wall_start_y_pixel;
	while (current_pixel_y < wall_end_y_pixel)
	{
		pixel_color = screen_pixel_texture_colour(&texture_mapping_info,
						current_pixel_y);
		put_pixel(screen_column_x, current_pixel_y, pixel_color);
		current_pixel_y++;
	}
}

void	render_floor_section(int screen_column_x, int wall_end_y_pixel)
{
	int	current_pixel_y;

	current_pixel_y = wall_end_y_pixel;
	while (current_pixel_y < g_game.graphics.screen_height)
	{
		put_pixel(screen_column_x, current_pixel_y, g_game.map.floor_color);
		current_pixel_y++;
	}
}
