/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_ray_cast.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 13:48:28 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/15 16:37:12 by go-donne         ###   ########.fr       */
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

/* determine which wall face the ray intersected

GEOMETRIC ANALYSIS:
- Vertical walls (extend along X-axis): Compare world_intersection_x with world_pos_x
- Horizontal walls (extend along Y-axis): Compare world_intersection_y with world_pos_y

CARDINAL DIRECTION MAPPING:
	NORTH: Top face of wall cell (ray came from below)
	SOUTH: Bottom face of wall cell (ray came from above)  
	EAST: Right face of wall cell (ray came from left)				// right face of wall cell? or right-side of map space, which is an east wall?
	WEST: Left face of wall cell (ray came from right)

INPUT: Ray intersection result containing world coordinates and wall orientation
OUTPUT: Cardinal direction constant (NORTH/SOUTH/EAST/WEST) */
int	determine_intersected_wall_face(t_ray_result *wall_intersection_data)
{
	if (wall_intersection_data->world_wall_side == VERTICAL_WALL)
	{
		if (wall_intersection_data->world_intersection_x > g_game.player.world_pos_x)
			return (WEST);
		else
			return (EAST);
	}
	else
	{
		if (wall_intersection_data->world_intersection_y > g_game.player.world_pos_y)
			return (NORTH);
		else
			return (SOUTH);
	}
}

/*

TO DO

update all call sites to use the new function signatures:

render_single_column() calls calculate_ray_direction()
cast_ray_to_wall() should call determine_intersected_wall_face()

*/





/* DEMO: TUNNEL VISION EFFECT
Purpose: Show what happens WITHOUT peripheral vision transformation
Result: All screen columns display identical straight-ahead view (telescope effect)

COMPARISON WITH NORMAL VISION:
Normal: camera_plane_offset varies from -1 to +1 across screen width
        → Left columns see left peripheral, right columns see right peripheral
Tunnel: camera_plane_offset = 0 for ALL columns
        → All columns see identical straight-ahead direction

MATHEMATICAL DIFFERENCE:
Normal: ray_direction = player_direction + (varying_offset * camera_plane)
Tunnel: ray_direction = player_direction + (0 * camera_plane) = player_direction

VISUAL RESULT: 
- Eliminates depth perception from parallax
- Creates "looking through telescope" effect
- All screen columns show same wall distance/texture
- Player can only see directly ahead, no peripheral awareness */
void	calculate_ray_direction_tunnel_vision(int screen_column_x, 
			double *world_ray_direction_x, double *world_ray_direction_y)
{
	(void)screen_column_x;  // Intentionally ignore column position
	
	// Force all rays to point straight ahead - no peripheral vision
	*world_ray_direction_x = g_game.player.world_dir_x;
	*world_ray_direction_y = g_game.player.world_dir_y;
}




/* ALTERNATIVE: REDUCED PERIPHERAL VISION
Demonstrates partial peripheral vision reduction */
void	calculate_ray_direction_narrow_fov(int screen_column_x, 
			double *world_ray_direction_x, double *world_ray_direction_y)
{
	double	camera_plane_offset;
	double	fov_reduction_factor;

	// Reduce field of view by 75% (normal FOV * 0.25)
	fov_reduction_factor = 0.25;
	
	camera_plane_offset = 2.0 * screen_column_x 
		/ (double)g_game.graphics.screen_width - 1.0;
	
	// Dramatically reduce peripheral vision spread
	*world_ray_direction_x = g_game.player.world_dir_x 
		+ (camera_plane_offset * fov_reduction_factor) * g_game.player.world_camera_plane_x;
	*world_ray_direction_y = g_game.player.world_dir_y 
		+ (camera_plane_offset * fov_reduction_factor) * g_game.player.world_camera_plane_y;
}
