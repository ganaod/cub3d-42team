/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_visual_column_sections.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 10:56:37 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/09 12:07:50 by go-donne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* column with its sections:

re. texture, from subject:
	"display different wall textures"
	"set the floor and ceiling to two different colours"

hence 
1 helper per section, texturing
only in render_wall_section() */

#include "../inc/cub3d.h"

/* render ceiling pixels above wall */
void	render_ceiling_section(int screen_x, int wall_start_y)
{
	int	y;

	y = 0;
	while (y < wall_start_y)
	{
		put_pixel(screen_x, y, g_game.map.ceiling_colour);
		y++;
	}
}

/* render wall texture pixels */
void	render_wall_section(int screen_x, int wall_start_y, int wall_end_y,
							t_ray_result *ray_result)
{
	int					y;
	int					texture_colour;
	t_texture_context	ctx;

	ctx.wall_direction = ray_result->wall_face;
	ctx.wall_hit_x = ray_result->hit_x;
	ctx.wall_hit_y = ray_result->hit_y;
	ctx.wall_height = wall_end_y - wall_start_y;
	y = wall_start_y;
	while (y < wall_end_y)
	{
		texture_colour = get_wall_texture_colour(&ctx, y);
		put_pixel(screen_x, y, texture_colour);
		y++;
	}
}
// DEMO: texture_colour = get_wall_hardcoded_color(wall_direction);

/* render floor pixels below wall */
void	render_floor_section(int screen_x, int wall_end_y)
{
	int	y;
	
	y = wall_end_y;
	while (y < g_game.graphics.screen_height)
	{
		put_pixel(screen_x, y, g_game.map.floor_colour);
		y++;
	}
}
