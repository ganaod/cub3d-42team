/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_region_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 15:11:28 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/26 12:54:23 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static int	ff_visit_idx(t_ffctx *c, int ni)
{
	if (c->vis[ni])
		return (1);
	if (c->m->grid[ni] == CELL_EMPTY)
	{
		c->vis[ni] = 1;
		c->q[c->tail++] = ni;
		return (1);
	}
	if (c->m->grid[ni] == CELL_VOID)
	{
		if (c->vis_void[ni])
			return (0);
		c->vis[ni] = 1;
		c->q[c->tail++] = ni;
		return (1);
	}
	return (1);
}

static int	ff_neighbors_h(t_ffctx *c, int i, int x)
{
	if (x + 1 < c->w)
	{
		if (!ff_visit_idx(c, i + 1))
			return (0);
	}
	else
		return (0);
	if (x - 1 >= 0)
	{
		if (!ff_visit_idx(c, i - 1))
			return (0);
	}
	else
		return (0);
	return (1);
}

static int	ff_neighbors_v(t_ffctx *c, int i, int y)
{
	if (y + 1 < c->h)
	{
		if (!ff_visit_idx(c, i + c->w))
			return (0);
	}
	else
		return (0);
	if (y - 1 >= 0)
	{
		if (!ff_visit_idx(c, i - c->w))
			return (0);
	}
	else
		return (0);
	return (1);
}

static int	ff_process_neighbors(t_ffctx *c, int i, int x, int y)
{
	if (!ff_neighbors_h(c, i, x))
		return (0);
	if (!ff_neighbors_v(c, i, y))
		return (0);
	return (1);
}

int	ff_region_scan(t_ffctx *c)
{
	int	i;
	int	x;
	int	y;

	while (c->head != c->tail)
	{
		i = c->q[c->head++];
		x = i % c->w;
		y = i / c->w;
		if (!ff_process_neighbors(c, i, x, y))
			return (0);
	}
	return (1);
}
