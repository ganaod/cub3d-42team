/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_collides.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 14:12:01 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/25 20:23:42 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

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
