/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_column.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 10:56:37 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/07 11:16:51 by go-donne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// column section helpers

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
