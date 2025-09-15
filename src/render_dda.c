/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_dda.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 15:48:27 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/15 17:50:00 by go-donne         ###   ########.fr       */
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

ret: perpendicular dist (prevent fisheye distortion) 



bridges discrete map space & continuous world space

steps thru integer grid cells while maintaining continuous dist precision for accurate intersection calculations




dda traversal state held in:

typedef struct s_dda_state {
    int    map_x, map_y;        // Current grid position
    int    step_x, step_y;      // Step direction (-1 or +1)
    double side_dist_x, side_dist_y;  // Distance to next boundary
    double delta_dist_x, delta_dist_y; // Distance per grid step
} t_dda_state;


wall side types
# define VERTICAL_WALL	0
# define HORIZONTAL_WALL 1

*/




t_ray_result	cast_ray_to_wall(double world_ray_dir_x, double world_ray_dir_y)
{
	t_dda_state		dda_state;
	t_ray_result	wall_intersection_result;

	// Step 1: Initialize DDA traversal state using existing setup function
	setup_dda_vars(world_ray_dir_x, world_ray_dir_y, &dda_state);

	// Step 2: Execute DDA algorithm to find wall intersection
	execute_dda_traversal(&dda_state, &wall_intersection_result.world_wall_side);

	// Step 3: Calculate perpendicular distance (prevents fisheye distortion)
	wall_intersection_result.world_distance = 
		calculate_wall_distance(&dda_state, wall_intersection_result.world_wall_side);

	// Step 4: Calculate precise intersection coordinates
	wall_intersection_result.world_intersection_x = g_game.player.world_pos_x 
		+ (wall_intersection_result.world_distance * world_ray_dir_x);
	wall_intersection_result.world_intersection_y = g_game.player.world_pos_y 
		+ (wall_intersection_result.world_distance * world_ray_dir_y);

	// Step 5: Determine wall face using coordinate-based method (SINGLE SOURCE OF TRUTH)
	wall_intersection_result.world_wall_face = 
		determine_intersected_wall_face(&wall_intersection_result);

	return (wall_intersection_result);
}

/* core dda algo

dda stepping loop:
. compare distances to next x & y grid boundaries
. step to whichever boundary is closer
. update dist for axis stepped along
. check if new grid cell contains wall
. repeat until wall found */
void	execute_dda_traversal(t_dda_state *dda_state, int *world_wall_side)
{
	while (!dda_state->wall_intersection_found)
	{
		// Determine which boundary is closer
		if (dda_state->world_dist_to_next_boundary_x < dda_state->world_dist_to_next_boundary_y)
		{
			// Step to next vertical boundary
			dda_state->world_dist_to_next_boundary_x += dda_state->delta_dist_x;
			dda_state->map_x += dda_state->step_x;
			*world_wall_side = VERTICAL_WALL;
		}
		else
		{
			// Step to next horizontal boundary
			dda_state->world_dist_to_next_boundary_y += dda_state->delta_dist_y;
			dda_state->map_y += dda_state->step_y;
			*world_wall_side = HORIZONTAL_WALL;
		}

		// Check if current grid cell contains wall
		if (g_game.map.grid[dda_state->map_y * g_game.map.width + dda_state->map_x] == CELL_WALL)
			dda_state->wall_intersection_found = 1;
	}
}

/* dist calc:
. calc perpendicular dist to prevent fisheye distortion
. use exact intersection pt on wall face
. ret dist from player > wall intersection */
double	calculate_wall_distance(t_dda_state *dda_state, int world_wall_side)
{
	if (world_wall_side == VERTICAL_WALL)
	{
		return ((dda_state->map_x - g_game.player.world_pos_x 
			+ (1 - dda_state->step_x) / 2) / dda_state->world_ray_dir_x);
	}
	else
	{
		return ((dda_state->map_y - g_game.player.world_pos_y 
			+ (1 - dda_state->step_y) / 2) / dda_state->world_ray_dir_y);
	}
}
