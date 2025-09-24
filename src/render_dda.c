/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_dda.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 12:09:39 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/24 13:16:33 by go-donne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/render.h"

void			execute_dda_traversal(t_dda_state *state, int *wall_side);
double			calculate_wall_distance(t_dda_state *dda_state, int wall_side);

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
player view with direct distances     player view with perpendicular 
                                     to screen plane

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
	execute_dda_traversal(&dda_state,
		&wall_intersection_result.world_wall_side);
	wall_intersection_result.world_perpendicular_distance
		= calculate_wall_distance(&dda_state,
			wall_intersection_result.world_wall_side);
	wall_intersection_result.world_intersection_x = g_game.player.world_pos_x
		+ (wall_intersection_result.world_perpendicular_distance
			* world_ray_dir_x);
	wall_intersection_result.world_intersection_y = g_game.player.world_pos_y
		+ (wall_intersection_result.world_perpendicular_distance
			* world_ray_dir_y);
	wall_intersection_result.world_wall_face
		= determine_intersected_wall_face(&wall_intersection_result);
	return (wall_intersection_result);
}

/* boolean flag to terminate loop
. separate traversal logic vs dist calc
. clear state machine: searching > found */
void	execute_dda_traversal(t_dda_state *dda_state, int *world_wall_side)
{
	while (!dda_state->wall_intersection_found)
	{
		if (dda_state->world_dist_to_next_boundary_x
			< dda_state->world_dist_to_next_boundary_y)
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
		if (g_game.map.grid[dda_state->map_y * g_game.map.width
				+ dda_state->map_x] == CELL_WALL)
			dda_state->wall_intersection_found = 1;
	}
}

/* calculate perpendicular distance from player to wall intersection */
double calculate_wall_distance(t_dda_state *dda_state, int world_wall_side)
{
    if (world_wall_side == VERTICAL_WALL)
        return (dda_state->world_dist_to_next_boundary_x - dda_state->delta_dist_x);
    else
        return (dda_state->world_dist_to_next_boundary_y - dda_state->delta_dist_y);
}
