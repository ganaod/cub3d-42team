/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_grid_fill.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 09:38:11 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/25 20:48:26 by blohrer          ###   ########.fr       */
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
	double			dx;
	double			dy;
	const double	fov_radians = FOV_DEGREES * (M_PI / 180.0);
	const double	fov_half_angle = fov_radians / 2.0;
	const double	camera_plane_magnitude = tan(fov_half_angle);

	if (!p)
		return ;
	p->world_pos_x = (double)x + 0.5;
	p->world_pos_y = (double)y + 0.5;
	dir_vec_from_char(c, &dx, &dy);
	p->world_dir_x = dx;
	p->world_dir_y = dy;
	p->world_camera_plane_x = -dy * camera_plane_magnitude;
	p->world_camera_plane_y = dx * camera_plane_magnitude;
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

static int	fill_row(t_map *m, t_player *pl, char *line, int y)
{
	int	x;
	int	idx;
	int	found;
	int	res;

	if (!m || !pl || !line || m->width <= 0)
		return (-1);
	x = 0;
	idx = y * m->width;
	found = 0;
	while (x < m->width)
	{
		res = put_cell_from_char(m, pl, idx, line[x]);
		if (res == 0)
			return (-1);
		if (res == 2)
		{
			found = found + 1;
			if (found > 1)
				return (-1);
		}
		x++;
		idx++;
	}
	return (found);
}

int	fill_grid(t_map *m, t_player *pl, char **lines, int *count)
{
	int	y;
	int	total;
	int	r;

	if (!m || !pl || !lines || !count || m->height <= 0 || m->width <= 0)
		return (0);
	y = 0;
	total = 0;
	while (y < m->height)
	{
		r = fill_row(m, pl, lines[y], y);
		if (r < 0)
			return (0);
		total = total + r;
		if (total > 1)
			return (0);
		y++;
	}
	*count = total;
	return (1);
}
