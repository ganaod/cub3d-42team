/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_check_flood.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 08:29:57 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/09 14:13:33 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

/* Init Context (w,h, head, tail) */
void	ffctx_init(t_ffctx *c, const t_map *m, char *vis, int *q)
{
	c->m = m;
	c->vis = vis;
	c->q = q;
	c->w = m->width;
	c->h = m->height;
	c->head = 0;
	c->tail = 0;
}

/* Start vorbereiten: nur starten, wenn Startzelle VOID ist.
 * return 0 -> nichts zu tun (nicht VOID)
 * return 1 -> initialisiert + enqueued
 */
static int	ff_init_start(t_ffctx *c, int start_idx)
{
	if (c->m->grid[start_idx] != CELL_VOID)
		return (0);
	c->vis[start_idx] = 1;
	queue_push(c->q, &c->tail, start_idx);
	return (1);
}

/* Versucht (nx,ny) zu besuchen.
 * return 0 -> EMPTY gefunden (offen)
 * return 1 -> ok/weiter
 */
static int	ff_try_neighbor(t_ffctx *c, int nx, int ny)
{
	int	n_idx;

	if (!in_bounds(nx, ny, c->w, c->h))
		return (1);
	n_idx = idx_2d_to_1d(nx, ny, c->w);
	if (c->vis[n_idx])
		return (1);
	if (c->m->grid[n_idx] == CELL_EMPTY)
		return (0);
	if (c->m->grid[n_idx] == CELL_VOID)
	{
		c->vis[n_idx] = 1;
		queue_push(c->q, &c->tail, n_idx);
	}
	return (1);
}

/* Poppt ein Element und prüft 4 Nachbarn.
 * return 0 -> offen gefunden
 * return 1 -> verarbeitet, weiter
 * return 2 -> queue leer
 */
static int	ff_process_one(t_ffctx *c)
{
	int	i;
	int	x;
	int	y;

	if (!queue_pop(c->q, &c->head, c->tail, &i))
		return (2);
	x = i % c->w;
	y = i / c->w;
	if (ff_try_neighbor(c, x + 1, y) == 0)
		return (0);
	if (ff_try_neighbor(c, x - 1, y) == 0)
		return (0);
	if (ff_try_neighbor(c, x, y + 1) == 0)
		return (0);
	if (ff_try_neighbor(c, x, y - 1) == 0)
		return (0);
	return (1);
}

/* Startet Flood ab start_idx (nur wenn VOID).
 * return 0 -> offen
 * return 1 -> geschlossen/kein EMPTY erreichbar
 */
int	flood_from_start(t_ffctx *c, int start_idx)
{
	int	rc;

	if (!ff_init_start(c, start_idx))
		return (1);
	while (1)
	{
		rc = ff_process_one(c);
		if (rc == 0)
			return (0);
		if (rc == 2)
			break ;
	}
	return (1);
}
