/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_dda.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 09:35:42 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/25 09:36:15 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/render.h"

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
