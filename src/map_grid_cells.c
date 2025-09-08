/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_grid_cells.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 16:03:35 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/08 09:47:46 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static void	dir_vec_from_char(char c, double *dx, double *dy)
{
	*dx = 1.0;
	*dy = 0.0;
	if (c == 'N')
	{
		*dx = 0.0;
		*dy = -1.0;
	}
	else if (c == 'S')
	{
		*dx = 0.0;
		*dy = 1.0;
	}
	else if (c == 'W')
	{
		*dx = -1.0;
		*dy = 0.0;
	}
}

static void	set_player_from_char(t_player *p, int x, int y, char c)
{
	double	dx;
	double	dy;

	if (!p)
		return ;
	p->pos_x = (double)x + 0.5;
	p->pos_y = (double)y + 0.5;
	dir_vec_from_char(c, &dx, &dy);
	p->dir_x = dx;
	p->dir_y = dy;
	p->camera_plane_x = -dy * FOV_PLANE;
	p->camera_plane_y = dx * FOV_PLANE;
}

int	put_cell_from_char(t_map *m, t_player *pl, int idx, char c)
{
	int	x;
	int	y;

	if (!m || !pl || !m->grid || m->width <= 0)
		return (0);
	x = idx % m->width;
	y = idx / m->width;
	if (c == ' ')
		m->grid[idx] = CELL_VOID;
	else if (c == '1')
		m->grid[idx] = CELL_WALL;
	else if (c == '0')
		m->grid[idx] = CELL_EMPTY;
	else if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
	{
		set_player_from_char(pl, x, y, c);
		m->grid[idx] = CELL_EMPTY;
		return (2);
	}
	else
		return (0);
	return (1);
}
