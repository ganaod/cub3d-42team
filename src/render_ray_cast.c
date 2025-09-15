/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_ray_cast.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 13:48:28 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/15 17:15:23 by go-donne         ###   ########.fr       */
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

/* WALL FACE DETERMINATION FROM RAY-WALL INTERSECTION

GEOMETRIC ANALYSIS:
- Ray traveling East (+X) hits West face of wall cell
- Ray traveling West (-X) hits East face of wall cell  
- Ray traveling South (+Y) hits North face of wall cell
- Ray traveling North (-Y) hits South face of wall cell

WALL FACE NOMENCLATURE:
- NORTH face: Top edge of wall cell
- SOUTH face: Bottom edge of wall cell
- EAST face: Right edge of wall cell  
- WEST face: Left edge of wall cell

INPUT: Ray intersection result with world coordinates and wall orientation
OUTPUT: Wall face constant (NORTH/SOUTH/EAST/WEST) */
int	determine_intersected_wall_face(t_ray_result *wall_intersection_data)
{
	if (wall_intersection_data->world_wall_side == VERTICAL_WALL)
	{
		if (wall_intersection_data->world_intersection_x > g_game.player.world_pos_x)
			return (WEST);	// Ray traveled East, hit WEST FACE of wall cell
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

The logic in determine_intersected_wall_face() is a form of geometric deduction. 
It relies on the principle that the orientation of a surface (a wall face) 
is directly related to the direction of the ray that hits it. 
The code is effectively a simple rule-based expert system 
that interprets the geometric state of the ray-wall intersection to make a decision.


The core idea is to break down the problem into two orthogonal cases:

Is it a vertical wall? 
This means the ray hit an east or west face. 
The deciding factor is whether the ray traveled mostly in the x-direction.

Is it a horizontal wall? 
This means the ray hit a north or south face. 
The deciding factor is whether the ray traveled mostly in the y-direction.


Within each case, the logic uses a directional comparison:

For vertical walls, it compares the hit point's x-coordinate to the player's x-coordinate. 
A hit point to the east of the player means the ray traveled east, and thus must have hit a west-facing wall.

For horizontal walls, it compares the hit point's y-coordinate to the player's y-coordinate. 
A hit point to the south of the player means the ray traveled south, and thus must have hit a north-facing wall.

This method is efficient and simple because it avoids complex calculations 
by using the results of the raycasting algorithm itself 
(the hit coordinates and the wall orientation) to infer the wall face.



Program Architecture: Abstraction and Cohesion

The question of whether determine_intersected_wall_face() belongs in your ray-casting file 
or a new file is about program cohesion.


Ray-casting: The primary purpose of ray-casting is to trace a ray from a viewpoint 
into a scene and find the first object it hits.

Texturing: The primary purpose of texturing is to determine how to apply an image to a surface.


The determine_intersected_wall_face() function is a bridge between these two modules. 
It takes the output of the ray-casting algorithm (the t_ray_result) 
and transforms it into the input needed for the texturing logic.


Recommendation: The function is very closely tied to the result of a ray cast, 
so keeping it in the ray-casting file is a cohesive design choice. 
It completes the ray's journey by fully classifying the intersection event.


An alternative is to have a separate "rendering" or "drawing" module 
that takes the raw ray-casting results and performs all subsequent operations, 
including wall-face determination and texturing. 
However, for a project like cub3d, a simpler structure with a ray_casting.c file 
that handles everything from ray creation to the final intersection result 
(including wall face) is a clean and common design. 

Separating it might introduce unnecessary complexity and file-hopping. 
Your current structure is well-justified from an architectural standpoint.



*/



// DEMOS - ALTERNATIVES:

/* TUNNEL VISION EFFECT
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




/* REDUCED PERIPHERAL VISION
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
