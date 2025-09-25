/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_collides.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 14:12:01 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/25 14:49:50 by blohrer          ###   ########.fr       */
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

static int	cell_hits(const t_collide_ctx *c, int ix, int iy)
{
	if (cell_is_solid_or_outside(c->m, ix, iy) && circle_overlaps_cell4(c->x,
			c->y, c->r, pack_cell(ix, iy)))
		return (1);
	return (0);
}

static int	row_has_collision(const t_collide_ctx *c, int iy)
{
	int	ix;

	ix = c->min_x;
	while (ix <= c->max_x)
	{
		if (cell_hits(c, ix, iy))
			return (1);
		ix++;
	}
	return (0);
}

static int	scan_box_for_collision(const t_collide_ctx *c)
{
	int	iy;

	iy = c->min_y;
	while (iy <= c->max_y)
	{
		if (row_has_collision(c, iy))
			return (1);
		iy++;
	}
	return (0);
}

int	collides_with_wall(const t_map *m, double x, double y, double r)
{
	t_collide_ctx	c;

	c.m = m;
	c.min_x = (int)(x - r);
	c.max_x = (int)(x + r);
	c.min_y = (int)(y - r);
	c.max_y = (int)(y + r);
	c.x = x;
	c.y = y;
	c.r = r;
	return (scan_box_for_collision(&c));
}
