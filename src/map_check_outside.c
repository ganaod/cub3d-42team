/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_check_outside.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 15:36:03 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/25 15:39:12 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static void	out_enqueue_if_void(t_outctx *c, int idx, int *tail)
{
	if (!c->vis[idx] && c->m->grid[idx] == CELL_VOID)
	{
		c->vis[idx] = 1;
		c->q[(*tail)++] = idx;
	}
}

static void	out_seed_top_bottom(t_outctx *c, int *tail)
{
	int	x;

	x = 0;
	while (x < c->w)
	{
		out_enqueue_if_void(c, 0 * c->w + x, tail);
		out_enqueue_if_void(c, (c->h - 1) * c->w + x, tail);
		x++;
	}
}

static void	out_seed_left_right(t_outctx *c, int *tail)
{
	int	y;

	y = 0;
	while (y < c->h)
	{
		out_enqueue_if_void(c, y * c->w + 0, tail);
		out_enqueue_if_void(c, y * c->w + (c->w - 1), tail);
		y++;
	}
}

static void	out_expand_queue(t_outctx *c, int *head, int *tail)
{
	int	cur;
	int	cx;
	int	cy;

	while (*head != *tail)
	{
		cur = c->q[(*head)++];
		cx = cur % c->w;
		cy = cur / c->w;
		if (cx + 1 < c->w)
			out_enqueue_if_void(c, cur + 1, tail);
		if (cx - 1 >= 0)
			out_enqueue_if_void(c, cur - 1, tail);
		if (cy + 1 < c->h)
			out_enqueue_if_void(c, cur + c->w, tail);
		if (cy - 1 >= 0)
			out_enqueue_if_void(c, cur - c->w, tail);
	}
}

void	mark_outside_void_pure(const t_map *m, char *vis_out, int *q)
{
	t_outctx	c;
	int			head;
	int			tail;

	if (!m || !m->grid)
		return ;
	c.m = m;
	c.vis = vis_out;
	c.q = q;
	c.w = m->width;
	c.h = m->height;
	head = 0;
	tail = 0;
	out_seed_top_bottom(&c, &tail);
	out_seed_left_right(&c, &tail);
	out_expand_queue(&c, &head, &tail);
}
