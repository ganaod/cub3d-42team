/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 11:10:54 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/09 08:37:24 by blohrer          ###   ########.fr       */
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

int	map_is_closed(const t_map *m)
{
	t_ffctx	c;
	char	*vis;
	int		*q;
	int		total;
	int		ok;

	if (!m || !m->grid || m->width <= 0 || m->height <= 0)
		return (0);
	if (!alloc_ff_buffers(m, &vis, &q, &total))
		return (0);
	ffctx_init(&c, m, vis, q);
	ok = flood_from_border(&c);
	free_ff_buffers(vis, q);
	return (ok);
}
