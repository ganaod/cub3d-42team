/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_collides_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 20:23:10 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/25 20:23:57 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static int	cell_is_solid_or_outside(const t_map *m, int ix, int iy)
{
	if (ix < 0 || iy < 0 || ix >= m->width || iy >= m->height)
		return (1);
	return (m->grid[iy * m->width + ix] == CELL_WALL);
}

static int	pack_cell(int ix, int iy)
{
	return (((iy & 0xFFFF) << 16) | (ix & 0xFFFF));
}

static int	circle_overlaps_cell4(double x, double y, double r, int packed)
{
	double	nx;
	double	ny;

	nx = clampd(x, (double)(short)(packed & 0xFFFF),
			(double)(short)(packed & 0xFFFF) + 1.0);
	ny = clampd(y, (double)(short)((packed >> 16) & 0xFFFF),
			(double)(short)((packed >> 16) & 0xFFFF) + 1.0);
	return ((((x - nx) * (x - nx)) + ((y - ny) * (y - ny))) < (r * r));
}

int	cell_hits(const t_collide_ctx *c, int ix, int iy)
{
	if (cell_is_solid_or_outside(c->m, ix, iy) && circle_overlaps_cell4(c->x,
			c->y, c->r, pack_cell(ix, iy)))
		return (1);
	return (0);
}
