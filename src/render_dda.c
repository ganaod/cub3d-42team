/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_dda.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 09:35:42 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/25 14:14:12 by go-donne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/render.h"

/* step 4: parametric ray eq
ray starts @ P's pos and goes infinitely in ray dir
. start at P pos (world_pos_x & _y)
. scale ray dir vector by perpendic. dist
. add to P's pos: gives exact intersectn pt
geometrically:
walk along ray dir vector from P until hit W

need perpendicular dist, not slanted (raw dda step len)
: fish-eye distortion correction */
t_ray_result	cast_ray_to_wall(t_game *g, double world_ray_dir_x,
		double world_ray_dir_y)
{
	t_dda_state		dda_state;
	t_ray_result	res;

	setup_dda_vars(g, world_ray_dir_x, world_ray_dir_y, &dda_state);
	execute_dda_traversal(g, &dda_state, &res.world_wall_side);
	res.world_perpendicular_distance = calculate_wall_distance(g, &dda_state,
			res.world_wall_side);
	res.world_intersection_x = g->player.world_pos_x
		+ res.world_perpendicular_distance * world_ray_dir_x;
	res.world_intersection_y = g->player.world_pos_y
		+ res.world_perpendicular_distance * world_ray_dir_y;
	res.world_wall_face = determine_intersected_wall_face(g, &res);
	return (res);
}

/* walk along grid

The ray is a continuous line in 2D space.

The grid is discrete (cells, walls).

We want to know which cell the ray hits first.

The ray can cross:

A vertical grid line (moving east/west to next column), or

A horizontal grid line (moving north/south to next row).

So at each iteration we ask:
👉 Which boundary (vertical or horizontal) is closer along the ray’s direction?

always advance along the smaller distance → ensures we move to the closest grid boundary the ray actually hits next.
After moving:

We update that distance (add one cell’s worth, delta_dist_x or delta_dist_y),

And we step the map coordinates (map_x or map_y).

Repeat until you hit a wall cell.


Why is this efficient?

Constant time per step – no trigonometry inside the loop. Distances were precomputed (delta_dist_x, delta_dist_y).

Only necessary cells are visited – we don’t “overstep” or sample points along the ray. We only ever check cells the ray actually enters.

Guarantees correct order – at each iteration, we know which boundary comes first, so we’re traversing the exact sequence of grid cells the ray passes through.

This is what makes DDA (Digital Differential Analyzer) superior to naive ray marching:
instead of incrementing in tiny floating steps along the ray, we “jump” from cell boundary to cell boundary, always along the correct direction.*/

/* */
void	execute_dda_traversal(t_game *g, t_dda_state *s, int *world_wall_side)
{
	(void)g;
	while (!s->wall_intersection_found)
	{
		if (s->world_dist_to_next_boundary_x < s->world_dist_to_next_boundary_y)
		{
			s->world_dist_to_next_boundary_x += s->delta_dist_x;
			s->map_x += s->step_x;
			*world_wall_side = VERTICAL_WALL;
		}
		else
		{
			s->world_dist_to_next_boundary_y += s->delta_dist_y;
			s->map_y += s->step_y;
			*world_wall_side = HORIZONTAL_WALL;
		}
		if (g->map.grid[s->map_y * g->map.width + s->map_x] == CELL_WALL)
			s->wall_intersection_found = 1;
	}
}

double	calculate_wall_distance(t_game *g, t_dda_state *s, int world_wall_side)
{
	(void)g;
	if (world_wall_side == VERTICAL_WALL)
		return (s->world_dist_to_next_boundary_x - s->delta_dist_x);
	else
		return (s->world_dist_to_next_boundary_y - s->delta_dist_y);
}
