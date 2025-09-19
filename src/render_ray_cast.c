/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_ray_cast.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 13:48:28 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/19 15:02:19 by go-donne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Ray geometric operations: mathematical transformations for 3D vision simulation
Core responsibility: screen coordinate → world ray vector transformations */

#include "../inc/cub3d.h"

/* SCREEN-TO-RAY TRANSFORMATION
Creates peripheral vision by transforming raycaster from 1D scanner → 3D vision simulator

process:
	1. Screen column index → Field-of-view position [-1, +1]
	2. FOV position → Ray direction vector in world space
	3. Result: Natural peripheral vision instead of "tunnel vision"

Visual effect:
	Without: All columns show same straight-ahead view (telescope effect)
	With: Left/right columns show angled views (human-like peripheral vision)

breakdown:
	
	step 1: screen pixel → FOV position
	- Leftmost pixel (0) becomes -1 (left edge of vision)
	- Center pixel becomes 0 (straight ahead)  
	- Rightmost pixel becomes +1 (right edge of vision)
	- Normalization formula: scale to [0,2], then /offset to [-1,+1]

	step 2: calculate actual ray direction for this viewing angle
	- Base direction (where player faces) + angular offset (left/right tilt)
	- Visual: Creates "sweep" of vision from left peripheral → right peripheral
	- Implementation: Vector addition with scaling */
void calculate_ray_direction(int screen_column_x, 
	double *world_ray_direction_x, double *world_ray_direction_y)
{
	double fov_space_camera_plane_offset;
	
	fov_space_camera_plane_offset = SCREEN_TO_FOV_SCALE_FACTOR * screen_column_x 
		/ (double)g_game.graphics.screen_width - FOV_CENTER_OFFSET;
		
	*world_ray_direction_x = g_game.player.world_dir_x 
		+ fov_space_camera_plane_offset * g_game.player.world_camera_plane_x;
	*world_ray_direction_y = g_game.player.world_dir_y 
		+ fov_space_camera_plane_offset * g_game.player.world_camera_plane_y;
}

/* determine wall direction from ray-wall intersection

coordinate space: world space continuous coordinates
semantic convention: direction-based wall naming (wolfenstein standard)

wall direction nomenclature:
- WALL_NORTH: wall located north of player (negative y direction)
- WALL_SOUTH: wall located south of player (positive y direction)  
- WALL_EAST: wall located east of player (positive x direction)
- WALL_WEST: wall located west of player (negative x direction)

logic: wall direction = direction from player to intersection point
- intersection_x > player_x → wall is east of player → WALL_EAST
- intersection_y > player_y → wall is south of player → WALL_SOUTH

input: ray intersection result (world coordinates + wall orientation)
output: wall direction constant for texture selection */
int	determine_intersected_wall_face(t_ray_result *wall_intersection_data)
{
	int face;
	
	if (wall_intersection_data->world_wall_side == VERTICAL_WALL)
	{
		if (wall_intersection_data->world_intersection_x > g_game.player.world_pos_x)
			face = WALL_EAST;
		else
			face = WALL_WEST;
	}
	else
	{
		if (wall_intersection_data->world_intersection_y > g_game.player.world_pos_y)
			face = WALL_SOUTH;
		else
			face = WALL_NORTH;
	}
	
	// Only debug center column to avoid spam
	static int debug_count = 0;

	if (debug_count < 1) 
	{  // Only first hit
		printf("CENTER COLUMN EXPECTED: %d\n", g_game.graphics.screen_width/2);
		debug_count++;
	}

	if (debug_count < 5) 
	{
		printf("Wall face=%d, intersection=(%.2f,%.2f), player=(%.2f,%.2f), side=%d\n", 
			face, 
			wall_intersection_data->world_intersection_x,
			wall_intersection_data->world_intersection_y,
			g_game.player.world_pos_x, 
			g_game.player.world_pos_y,
			wall_intersection_data->world_wall_side);
		debug_count++;
	}
	
	return (face);
}
