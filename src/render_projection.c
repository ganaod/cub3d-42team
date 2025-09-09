/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_math_projection.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 11:18:50 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/09 14:44:52 by go-donne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* convert 3D world distances to 2D screen pixel heights

Core Transaction:
	Input: Wall distance (world units)
	Output: Wall height (screen pixels)

Visual Reality: 
The mathematical mechanism that makes distant walls 
appear smaller and near walls appear larger - 
creating the illusion of depth on a flat screen 

Why this exists? fundamental problem: 
human vision perceives depth through perspective - 
objects shrink with distance. 
To simulate 3D on 2D screen, we must mathematically replicate this visual scaling 

	
MATHEMATICAL FOUNDATION: Similar Triangles Perspective Projection
	Eye-to-screen triangle ~ Eye-to-wall triangle
	Ratio preservation: screen_height/focal_length = wall_height_world/wall_distance

Wolfenstein constraint: All walls same world height (1.0 unit)
Normalized case: focal_length = screen_height
Result: wall_height_pixels = screen_height / wall_distance

Mathematical behavior:
	. Distance 1.0 → Wall fills screen (height = screen_height)
	. Distance 2.0 → Wall half screen (height = screen_height/2)
	. Distance → ∞ → Wall approaches 0 pixels	*/

#include "../inc/cub3d.h"

/* Mathematical constraint analysis:
DDA returns distance to wall face, minimum theoretically > 0
However: floating point precision near grid boundaries could 
produce values approaching 0, causing overflow in division
Protection threshold: 0.001 units (1/1000 of grid cell)
Smaller distances treated as "player touching wall" */

/* Core perspective projection: Similar triangles ratio
wall_height_pixels = screen_height / wall_distance */	   

/* Rendering constraint: Prevent buffer overflow from extreme proximity
Maximum 2x screen height allows for reasonable "wall fills view" effect
without creating unmanageable pixel counts for column renderer */
int	calculate_screen_wall_height(double wall_distance)
{
	int	wall_height_pixels;
	
	if (wall_distance < 0.001)
		wall_distance = 0.001;
	wall_height_pixels = (int)(g_game.graphics.screen_height
							/ wall_distance);
	if (wall_height_pixels > g_game.graphics.screen_height * 2)
		wall_height_pixels = g_game.graphics.screen_height * 2;
	return (wall_height_pixels);
}

/* WALL VERTICAL POSITIONING: Eye-level perspective simulation
   
Human vision: Horizon line at eye level (screen center)
Wall positioning: Center wall around screen midpoint
Ceiling above, floor below - simulates looking straight ahead

Mathematical centering: 
wall_start = screen_center - wall_height/2
wall_end = wall_start + wall_height */

void	calculate_wall_boundaries(int wall_height, int *wall_start, int *wall_end)
{
	int	screen_center;

	screen_center = g_game.graphics.screen_height / 2;

	/* Center wall around screen midpoint for eye-level perspective */
	*wall_start = screen_center - (wall_height / 2);
	*wall_end = *wall_start + wall_height;

	/* Boundary enforcement: Prevent column renderer from drawing outside screen buffer
		Mathematical constraint: pixel coordinates must satisfy 0 ≤ y < screen_height */
	if (*wall_start < 0)
		*wall_start = 0;
	if (*wall_end > g_game.graphics.screen_height)
		*wall_end = g_game.graphics.screen_height;
}
