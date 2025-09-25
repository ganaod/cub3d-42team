/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_column.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 10:54:44 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/25 11:16:37 by go-donne         ###   ########.fr       */
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
