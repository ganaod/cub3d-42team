/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 11:10:54 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/22 07:56:30 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

int	flood_from_border(t_ffctx *c)
{
	int	x;
	int	y;

	x = 0;
	while (x < c->w)
	{
		if (!flood_from_start(c, idx_2d_to_1d(x, 0, c->w)))
			return (0);
		if (c->h > 1 && !flood_from_start(c, idx_2d_to_1d(x, c->h - 1, c->w)))
			return (0);
		x++;
	}
	y = 0;
	while (y < c->h)
	{
		if (!flood_from_start(c, idx_2d_to_1d(0, y, c->w)))
			return (0);
		if (c->w > 1 && !flood_from_start(c, idx_2d_to_1d(c->w - 1, y, c->w)))
			return (0);
		y++;
	}
	return (1);
}

static int	alloc_ff_buffers(const t_map *m, char **vis, int **q, int *total)
{
	int	i;

	*total = m->width * m->height;
	*vis = (char *)malloc(*total);
	*q = (int *)malloc(sizeof(int) * (*total));
	if (!*vis || !*q)
	{
		free(*vis);
		free(*q);
		return (0);
	}
	i = 0;
	while (i < *total)
	{
		(*vis)[i] = 0;
		i++;
	}
	return (1);
}

static void	free_ff_buffers(char *vis, int *q)
{
	free(vis);
	free(q);
}

/* 0 am Kartenrand => Map ist offen */
static int	border_rows_have_empty(const t_map *m, int w, int h)
{
	int x;

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

/* prüft linke und rechte Spalte auf CELL_EMPTY */
static int	border_cols_have_empty(const t_map *m, int w, int h)
{
	int y;

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

static int	cell_adjacent_to_void(const t_map *m, int x, int y)
{
	int w;
	int h;
	int i;

	if (!m || !m->grid)
		return (0);
	w = m->width;
	h = m->height;
	if (x < 0 || y < 0 || x >= w || y >= h)
		return (0);
	i = y * w + x;
	if (m->grid[i] != CELL_EMPTY)
		return (0);
	/* rechts */
	if (x + 1 < w && m->grid[i + 1] == CELL_VOID)
		return (1);
	/* links */
	if (x - 1 >= 0 && m->grid[i - 1] == CELL_VOID)
		return (1);
	/* unten */
	if (y + 1 < h && m->grid[i + w] == CELL_VOID)
		return (1);
	/* oben */
	if (y - 1 >= 0 && m->grid[i - w] == CELL_VOID)
		return (1);
	return (0);
}

static int	has_empty_adjacent_to_void(const t_map *m)
{
	int w;
	int h;
	int y;
	int x;

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


static int	map_has_open_border(const t_map *m)
{
	int w;
	int h;

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


int	map_is_closed(const t_map *m)
{
	t_ffctx	c;
	char	*vis;
	int		*q;
	int		total;
	int		ok;

	if (!m || !m->grid || m->width <= 0 || m->height <= 0)
		return (0);
	if (map_has_open_border(m))
		return (0);
	if (has_empty_adjacent_to_void(m))
		return (0);
	if (!alloc_ff_buffers(m, &vis, &q, &total))
		return (0);
	ffctx_init(&c, m, vis, q);
	ok = flood_from_border(&c);
	free_ff_buffers(vis, q);
	return (ok);
}
