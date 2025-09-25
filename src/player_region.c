/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_region.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 20:02:05 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/25 20:08:46 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static void	ff_ctx_init_player(t_ffctx *c, const t_map *m, const t_player *pl,
		const char *vis_void)
{
	c->m = m;
	c->vis = NULL;
	c->q = NULL;
	c->w = m->width;
	c->h = m->height;
	c->head = 0;
	c->tail = 0;
	c->pl = pl;
	c->vis_void = vis_void;
}

static int	ff_cleanup_return(t_ffctx *c, int ret)
{
	free(c->q);
	free(c->vis);
	return (ret);
}

static int	ff_alloc_vis_q(t_ffctx *c)
{
	int	total;

	total = c->w * c->h;
	c->q = (int *)malloc(sizeof(int) * total);
	c->vis = (char *)malloc(total);
	if (!c->q || !c->vis)
	{
		free(c->q);
		free(c->vis);
		return (0);
	}
	ft_bzero(c->vis, total);
	return (1);
}

static int	ff_start_ok_prepare(t_ffctx *c, int *start)
{
	int	sx;
	int	sy;
	int	total;

	sx = (int)c->pl->world_pos_x;
	sy = (int)c->pl->world_pos_y;
	total = c->w * c->h;
	*start = sy * c->w + sx;
	if (*start < 0 || *start >= total)
		return (0);
	if (c->m->grid[*start] != CELL_EMPTY)
		return (0);
	c->head = 0;
	c->tail = 0;
	c->vis[*start] = 1;
	c->q[c->tail++] = *start;
	return (1);
}

int	player_region_is_closed(const t_map *m, const t_player *pl,
		const char *vis_void)
{
	t_ffctx	c;
	int		start;

	if (!m || !pl || !m->grid)
		return (0);
	ff_ctx_init_player(&c, m, pl, vis_void);
	if (!ff_alloc_vis_q(&c))
		return (0);
	if (!ff_start_ok_prepare(&c, &start))
		return (ff_cleanup_return(&c, 0));
	if (!ff_region_scan(&c))
		return (ff_cleanup_return(&c, 0));
	return (ff_cleanup_return(&c, 1));
}
