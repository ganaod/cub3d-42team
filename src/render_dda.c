/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_dda.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 09:35:42 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/25 14:36:25 by go-donne         ###   ########.fr       */
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

/* "walk along" grid
ray: continuous line in 2D space
grid: discrete (cells, walls)
want to know which cell ray hits 1st

ray can cross:
. vertical grid line (const x, up-down)
(move into next col: E/W)
. horiz grid line (const y, l-r)
(cross 1 means moving into next row:
S if step_y = +1, N if step_y = -1)

at each iteration:
which boundary (vert / horiz) is closer along ray's dir?

advance along the smaller dist:
ensures moving to closest grid boundary ray actually hits next

after moving:
update that dist (1 cell's worth / delta_dist_y / _y)
step the map coords (map_y / _y)
repeat until W cell hit

efficient:
. const time / step: no trig inside loop
distances precomputed
. only necessary cells visited: don't overstep or
sample pts along ray: only check cells ray enters
. guarantees correct order

therefore superior to ray marching:
instead of incrementing tiny floating steps along ray,
jump from cell boundary > boundary, always along correct dir */
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
