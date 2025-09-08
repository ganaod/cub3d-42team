/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_grid_fill.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 09:38:11 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/08 09:38:55 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

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
