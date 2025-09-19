/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_ray_cast.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 13:48:28 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/19 14:08:11 by go-donne         ###   ########.fr       */
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
	if (wall_intersection_data->world_wall_side == VERTICAL_WALL)
	{
		if (wall_intersection_data->world_intersection_x > g_game.player.world_pos_x)
			return (WALL_EAST);	// Ray traveled East, hit WEST FACE of wall cell, but wall is to the east!
		else
			return (WALL_WEST);
	}
	else
	{
		if (wall_intersection_data->world_intersection_y > g_game.player.world_pos_y)
			return (WALL_SOUTH);
		else
			return (WALL_NORTH);
	}
}

// // DEMOS - ALTERNATIVES:

// /* TUNNEL VISION EFFECT
// Purpose: Show what happens WITHOUT peripheral vision transformation
// Result: All screen columns display identical straight-ahead view (telescope effect)

// COMPARISON WITH NORMAL VISION:
// Normal: camera_plane_offset varies from -1 to +1 across screen width
//         → Left columns see left peripheral, right columns see right peripheral
// Tunnel: camera_plane_offset = 0 for ALL columns
//         → All columns see identical straight-ahead direction

// MATHEMATICAL DIFFERENCE:
// Normal: ray_direction = player_direction + (varying_offset * camera_plane)
// Tunnel: ray_direction = player_direction + (0 * camera_plane) = player_direction

// VISUAL RESULT: 
// - Eliminates depth perception from parallax
// - Creates "looking through telescope" effect
// - All screen columns show same wall distance/texture
// - Player can only see directly ahead, no peripheral awareness */
// void	calculate_ray_direction_tunnel_vision(int screen_column_x, 
// 			double *world_ray_direction_x, double *world_ray_direction_y)
// {
// 	(void)screen_column_x;  // Intentionally ignore column position
	
// 	// Force all rays to point straight ahead - no peripheral vision
// 	*world_ray_direction_x = g_game.player.world_dir_x;
// 	*world_ray_direction_y = g_game.player.world_dir_y;
// }

// /* REDUCED PERIPHERAL VISION
// Demonstrates partial peripheral vision reduction */
// void	calculate_ray_direction_narrow_fov(int screen_column_x, 
// 			double *world_ray_direction_x, double *world_ray_direction_y)
// {
// 	double	camera_plane_offset;
// 	double	fov_reduction_factor;

// 	// Reduce field of view by 75% (normal FOV * 0.25)
// 	fov_reduction_factor = 0.25;
	
// 	camera_plane_offset = 2.0 * screen_column_x 
// 		/ (double)g_game.graphics.screen_width - 1.0;
	
// 	// Dramatically reduce peripheral vision spread
// 	*world_ray_direction_x = g_game.player.world_dir_x 
// 		+ (camera_plane_offset * fov_reduction_factor) * g_game.player.world_camera_plane_x;
// 	*world_ray_direction_y = g_game.player.world_dir_y 
// 		+ (camera_plane_offset * fov_reduction_factor) * g_game.player.world_camera_plane_y;
// }
