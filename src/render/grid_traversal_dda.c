/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 15:48:27 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/05 15:49:18 by go-donne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

/* visual reality: How far can I see before hitting a wall?
. Player stands at position, looks toward screen column X
. vision travels in straight line until blocked by wall
. function returns: how far that vision traveled

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
boundary crossings

think of DDA as:
	Standing at player position
	Looking along ray direction
	"Which grid line will I cross first - vertical or horizontal?"
	Step to that grid line, check for wall
	Repeat until wall found

Mathematically efficient grid traversal

ret: perpendicular dist (prevent fisheye distortion) */

// coordinator
double	cast_ray_to_wall(double ray_dir_x, double ray_dir_y, int *wall_side)
{
	t_dda_state	state;
	double		wall_distance;

	setup_dda_vars(ray_dir_x, ray_dir_y, &state);
	wall_distance = execute_dda_traversal(&state, wall_side);
	return (wall_distance);
}

/* initialiser
calculate all mathematical setup vars
transform ray dir > DDA stepping params */
void	setup_dda_vars(double ray_dir_x, double ray_dir_y, t_dda_state *state)
{
	state->map_x = (int)g_game.player.pos_x;
	state->map_y = (int)g_game.player.pos_y;
	state->delta_dist_x = fabs(1.0 / ray_dir_x);
	state->delta_dist_y = fabs(1.0 / ray_dir_y);

	setup_x_axis_stepping(ray_dir_x, state);
	setup_y_axis_stepping(ray_dir_y, state);
	state->wall_hit = 0;
}

/* stepping set up
prepares:
. which dir to step thru grid (step_x, step_y)
. how far to 1st grid boundary (side_dist_x, side_dist_y) 

enables core dda loop:
if x boundary closer: step in x dir ...*/
static void	setup_x_axis_stepping(double ray_dir_x, t_dda_state *state)
{
	if (ray_dir_x < 0)
	{
		state->step_x = -1;
		state->side_dist_x = (g_game.player.pos_x - state->map_x)
							* state->delta_dist_x;
	}
	else
	{
		state->step_x = 1;
		state->side_dist_x = (state->map_x + 1.0 - g_game.player.pos_x)
							* state->delta_dist_x;
	}
}

static void	setup_y_axis_stepping(double ray_dir_y, t_dda_state *state)
{
	if (ray_dir_y < 0)
	{
		state->step_y = -1;
		state->side_dist_y = (g_game.player.pos_y - state->map_y)
							* state->delta_dist_y;
	}
	else
	{
		state->step_y = 1;
		state->side_dist_y = (state->map_y + 1.0 - g_game.player.pos_y)
							* state->delta_dist_y;
	}
}

// UPCOMING
double execute_dda_traversal(t_dda_state *state, int *wall_side);