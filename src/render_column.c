/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_column.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 10:56:37 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/09 09:38:35 by go-donne         ###   ########.fr       */
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
		put_pixel(screen_x, y, g_game.map.ceiling_color);
		y++;
	}
}

/* render wall texture pixels */
void	render_wall_section(int screen_x, int wall_start_y, int wall_end_y,
							int wall_direction)
{
	int	y;
	int	texture_color;

	y = wall_start_y;
	while (y < g_game.graphics.screen_height)
	{
		texture_color = get_wall_texture_color(wall_direction, y,
						wall_end_y - wall_start_y);
		put_pixel(screen_x, y, g_game.map.floor_color);
		y++;
	}
}

/* render floor pixels below wall */
void	render_floor_section(int screen_x, int wall_end_y)
{
	int	y;
	
	y = wall_end_y;
	while (y < g_game.graphics.screen_height)
	{
		put_pixel(screen_x, y, g_game.map.floor_color);
		y++;
	}
}
