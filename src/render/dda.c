/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 15:48:27 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/05 17:02:52 by go-donne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void		execute_dda_traversal(t_dda_state *state, int *wall_side);
double		calculate_wall_distance(t_dda_state *state, int wall_side);

/* visual reality: How far can I see before hitting a wall?
. player stands at position, looks toward screen column X
. vision travels in straight line until blocked by wall
. fn returns: how far that vision traveled

without: no depth perception - every wall would appear
same size regardless of distance

core transaction:
	. in: ray direction (where to look)
	. out: distance to nearest wall in that direction

problem: find intersection of continuous line with discrete grid
constraint: must be exact (no approximatn errs)
performance: must execute 1000+ time/frame (real-time requirement)


Digital Differential Analyzer (DDA) algorithm: 
instead of checking arbitrary pts, only check grid
boundary crossings (efficient)

think of DDA as:
	Standing at player position
	Looking along ray direction
	"Which grid line will I cross first - vertical or horizontal?"
	Step to that grid line, check for wall
	Repeat until wall found

ret: perpendicular dist (prevent fisheye distortion) */

// coordinate
double	cast_ray_to_wall(double ray_dir_x, double ray_dir_y, int *wall_side)
{
	t_dda_state	state;
	double		wall_distance;

	setup_dda_vars(ray_dir_x, ray_dir_y, &state);
	execute_dda_traversal(&state, wall_side);
	wall_distance = calculate_wall_distance(&state, wall_side);
	return (wall_distance);
}

/* core dda algo

dda stepping loop:
. compare distances to next x & y grid boundaries
. step to whichever boundary is closer
. update dist for axis stepped along
. check if new grid cell contains wall
. repeat until wall found */
void	execute_dda_traversal(t_dda_state *state, int *wall_side)
{
	while (!state->wall_hit)
	{
		if (state->side_dist_x < state->side_dist_y)
		{
			state->side_dist_x += state->delta_dist_x;
			state->map_x += state->step_x;
			*wall_side = VERTICAL_WALL;
		}
		else
		{
			state->side_dist_y += state->delta_dist_y;
			state->map_y += state->step_y;
			*wall_side = HORIZONTAL_WALL;
		}
		if (g_game.map.grid[state->map_y * g_game.map.width + state->map_x] == WALL)
			state->wall_hit = 1;
	}
}

/* dist calc:
. calc perpendicular dist to prevent fisheye distortion
. use exact intersection pt on wall face
. ret dist from player > wall intersection */
double	calculate_wall_distance(t_dda_state *state, int wall_side)
{
	if (wall_side == VERTICAL_WALL)
		return ((state->map_x - g_game.player.pos_x + (1 - state->step_x) / 2) / state->ray_dir_x);
	else
		return ((state->map_y - g_game.player.pos_y + (1 - state->step_y) / 2) / state->ray_dir_y);
}
