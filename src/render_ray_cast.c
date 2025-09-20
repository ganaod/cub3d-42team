/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_ray_cast.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 13:48:28 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/20 13:52:11 by go-donne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Ray geometric operations: mathematical transformations for 3D vision simulation
Core responsibility: screen coordinate → world ray vector transformations */

#include "../inc/cub3d.h"

/* what: visual game experience: create peripheral vision
how: by transforming raycaster from 1D scanner → 3D vision simulator



determine ray direction vector using an offset from the centre

offset = deviation parameter, how far left/right the ray tilts from forward


world_ray_direction (parametric direction vector for this column)
	= dir + offset * plane
	dir = forward vector
	plane = camera plane vector (perpendicular to dir)
		scale the vector along the perpendicular axis
	offset ∈ [-1, 1]

this creates a sweep across the FOV — the angular deviation




process:
	1. screen column index → FoV position [-1, +1]
	2. FoV position → ray direction vector in world space
	3. result: natural peripheral vision instead of "tunnel vision"

visual effect:
	Without: All columns show same straight-ahead view (telescope effect)
	With: Left/right columns show angled views (human-like peripheral vision)

breakdown:
	
step 1: screen pixel → FOV position
. leftmost pixel (0) becomes -1 (left edge of vision)
. center pixel becomes 0 (straight ahead)  
. rightmost pixel becomes +1 (right edge of vision)
. normalization formula: scale to [0,2], then offset to [-1,+1]

step 2: calculate actual ray direction for this viewing angle
. base direction (where player faces) + angular offset (left/right tilt)
. visual: Creates "sweep" of vision from left peripheral → right peripheral
. implementation: vector addition with scaling */
	
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

// /* no peripheral sweep → all columns show the same forward view.
// visual effect: telescope-like, extremely narrow, boring “tunnel” view */
// void	calculate_ray_direction_tunnel_vision(int screen_column_x,
// 		double *world_ray_direction_x, double *world_ray_direction_y)
// {
// 	(void)screen_column_x;
// 	*world_ray_direction_x = g_game.player.world_dir_x;
// 	*world_ray_direction_y = g_game.player.world_dir_y;
// }

/* ray-wall intersection > wall direction for texture selection

coordinate space: world space continuous coordinates
semantic convention: direction-based wall naming (wolfenstein standard)

wall direction nomenclature:
. WALL_NORTH: wall located north of player (negative y direction)
. WALL_SOUTH: wall located south of player (positive y direction)  
. WALL_EAST: wall located east of player (positive x direction)
. WALL_WEST: wall located west of player (negative x direction)

logic: wall direction = direction from player to intersection point
. intersection_x > player_x → wall is east of player → WALL_EAST
. intersection_y > player_y → wall is south of player → WALL_SOUTH */
int	determine_intersected_wall_face(t_ray_result *wall_intersection_data)
{
	if (wall_intersection_data->world_wall_side == VERTICAL_WALL)
	{
		if (wall_intersection_data->world_intersection_x > g_game.player.world_pos_x)
			return (WALL_EAST);
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
