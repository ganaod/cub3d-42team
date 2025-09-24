/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_dda.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 15:48:27 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/24 10:32:40 by go-donne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/render.h"

void			execute_dda_traversal(t_dda_state *state, int *wall_side);
double			calculate_wall_distance(t_dda_state *dda_state, int world_wall_side);
static void		setup_distance_context(t_dda_state *dda_state, int world_wall_side,
						t_distance_calc_context *ctx);
static double	compute_perpendicular_distance(t_distance_calc_context *ctx);

/* Digital Differential Analyzer (DDA) traversal algorithm: 
optimal discrete grid intersection algo

problem: find wall intersection efficiently in discrete grid
solution: step only to grid boundaries, not arbitrary pts

algo:
1. setup: compare distances to next X & Y grid boundaries
2. traverse: step to whichever boundary is closer
3. check cell for wall, repeat until wall found

4. calculate exact perpendicular dist to prevent fisheye

without perpendicular distance:        with perpendicular distance:
player view with direct distances     player view with perpendicular to screen plane

     \     |     /                        |     |     |
      \    |    /                         |     |     |
       \   |   /                          |     |     |
        \  |  /                           |     |     |
         \ | /                            |     |     |
          \|/                             |     |     |
         player                          player

curved walls (fisheye effect)          straight walls (realistic)


without: no depth perception - every wall would appear
same size regardless of distance

core transaction:
	. in: ray direction (where to look)
	. out: distance to nearest wall in that direction

performance: must execute 1000+ time/frame (real-time requirement)
bridges discrete map space & continuous world space


dda traversal state held in t_dda_state	{
int		map_x & y			curr grid pos
int		step_x &y			step dir (-1 / +1)
double	side_dist_x & y		dist to next boundary
double	delta_dist_x & y	dist per grid step	}	*/
t_ray_result	cast_ray_to_wall(double world_ray_dir_x, double world_ray_dir_y)
{
	t_dda_state		dda_state;
	t_ray_result	wall_intersection_result;

	setup_dda_vars(world_ray_dir_x, world_ray_dir_y, &dda_state);
	execute_dda_traversal(&dda_state, &wall_intersection_result.world_wall_side);
	wall_intersection_result.world_perp_dist = 
		calculate_wall_distance(&dda_state, wall_intersection_result.world_wall_side);
	wall_intersection_result.world_intersection_x = g_game.player.world_pos_x 
		+ (wall_intersection_result.world_perp_dist * world_ray_dir_x);
	wall_intersection_result.world_intersection_y = g_game.player.world_pos_y 
		+ (wall_intersection_result.world_perp_dist * world_ray_dir_y);
	wall_intersection_result.world_wall_face = 
		determine_intersected_wall_face(&wall_intersection_result);
	return (wall_intersection_result);
}

/* booelan flag to terminate loop
. separate traversal logic vs dist calc
. clear state machine: searching > found */
void	execute_dda_traversal(t_dda_state *dda_state, int *world_wall_side)
{
	while (!dda_state->wall_intersection_found)
	{
		if (dda_state->world_dist_to_next_boundary_x < dda_state->world_dist_to_next_boundary_y)
		{
			dda_state->world_dist_to_next_boundary_x += dda_state->delta_dist_x;
			dda_state->map_x += dda_state->step_x;
			*world_wall_side = VERTICAL_WALL;
		}
		else
		{
			dda_state->world_dist_to_next_boundary_y += dda_state->delta_dist_y;
			dda_state->map_y += dda_state->step_y;
			*world_wall_side = HORIZONTAL_WALL;
		}
		if (g_game.map.grid[dda_state->map_y * g_game.map.width + dda_state->map_x] == CELL_WALL)
			dda_state->wall_intersection_found = 1;
	}
}

/* calculate perpendicular distance from player to wall intersection */
double	calculate_wall_distance(t_dda_state *dda_state, int world_wall_side)
{
	t_distance_calc_context	calc_context;

	setup_distance_context(dda_state, world_wall_side, &calc_context);
	return (compute_perpendicular_distance(&calc_context));
}

/* setup calculation context with all required values
   extracts coordinate components, step direction, and wall face offset 
   
Direction of movement determines which face of the cell you hit first:
Moving positive (+1): Hit the near edge (0 offset from cell origin)
Moving negative (-1): Hit the far edge (1 offset from cell origin)   */
static void	setup_distance_context(t_dda_state *dda_state, int world_wall_side,
				t_distance_calc_context *ctx)
{
	if (world_wall_side == VERTICAL_WALL)
	{
		ctx->wall_grid_position = (double)dda_state->map_x;
		ctx->player_position = g_game.player.world_pos_x;
		ctx->ray_direction_component = dda_state->world_ray_dir_x;
		ctx->step_direction = dda_state->step_x;
	}
	else
	{
		ctx->wall_grid_position = (double)dda_state->map_y;
		ctx->player_position = g_game.player.world_pos_y;
		ctx->ray_direction_component = dda_state->world_ray_dir_y;
		ctx->step_direction = dda_state->step_y;
	}
	if (ctx->step_direction == 1)
		ctx->wall_face_offset = 0.0;  // near face (left/top edge)
	else
		ctx->wall_face_offset = 1.0;  // far face (right/bottom edge)
}

/* compute final perpendicular distance using prepared context */
static double	compute_perpendicular_distance(t_distance_calc_context *ctx)
{
	double	distance_in_grid_units;

	distance_in_grid_units = ctx->wall_grid_position - ctx->player_position 
		+ ctx->wall_face_offset;
	return (distance_in_grid_units / ctx->ray_direction_component);
}
