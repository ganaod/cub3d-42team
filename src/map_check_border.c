/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_check_border.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 10:52:51 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/23 10:56:40 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

int	border_rows_have_empty(const t_map *m, int w, int h)
{
	int	x;

	x = 0;
	while (x < w)
	{
		if (m->grid[0 * w + x] == CELL_EMPTY)
			return (1);
		if (m->grid[(h - 1) * w + x] == CELL_EMPTY)
			return (1);
		x++;
	}
	return (0);
}

int	border_cols_have_empty(const t_map *m, int w, int h)
{
	int	y;

	y = 0;
	while (y < h)
	{
		if (m->grid[y * w + 0] == CELL_EMPTY)
			return (1);
		if (m->grid[y * w + (w - 1)] == CELL_EMPTY)
			return (1);
		y++;
	}
	return (0);
}

int	cell_adjacent_to_void(const t_map *m, int x, int y)
{
	int	w;
	int	h;
	int	i;

	if (!m || !m->grid)
		return (0);
	w = m->width;
	h = m->height;
	if (x < 0 || y < 0 || x >= w || y >= h)
		return (0);
	i = y * w + x;
	if (m->grid[i] != CELL_EMPTY)
		return (0);
	if (x + 1 < w && m->grid[i + 1] == CELL_VOID)
		return (1);
	if (x - 1 >= 0 && m->grid[i - 1] == CELL_VOID)
		return (1);
	if (y + 1 < h && m->grid[i + w] == CELL_VOID)
		return (1);
	if (y - 1 >= 0 && m->grid[i - w] == CELL_VOID)
		return (1);
	return (0);
}

int	has_empty_adjacent_to_void(const t_map *m)
{
	int	w;
	int	h;
	int	y;
	int	x;

	if (!m || !m->grid || m->width <= 0 || m->height <= 0)
		return (1);
	w = m->width;
	h = m->height;
	y = 0;
	while (y < h)
	{
		x = 0;
		while (x < w)
		{
			if (cell_adjacent_to_void(m, x, y))
				return (1);
			x++;
		}
		y++;
	}
	return (0);
}

int	map_has_open_border(const t_map *m)
{
	int	w;
	int	h;

	if (!m || !m->grid || m->width <= 0 || m->height <= 0)
		return (1);
	w = m->width;
	h = m->height;
	if (border_rows_have_empty(m, w, h))
		return (1);
	if (border_cols_have_empty(m, w, h))
		return (1);
	return (0);
}
