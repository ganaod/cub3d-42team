/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_projection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 11:18:50 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/24 11:07:02 by go-donne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/render.h"

/* convert 1D dist measurements > 2D screen pixel heights 
(creating 3D/2.5D depth illusion)

core transformation:
. in: wall dist (/ world units. 1D ray-cast measurement)
. out: wall height (screen pixels for 2D display)

visual exp:
make distant walls appear smaller and near walls appear larger - 
creating the illusion of 3D depth on a flat screen 

why? fundamental problem: 
. human vision perceives depth through perspective - 
objects shrink with distance. 
. to simulate 3D on 2D screen,
we must mathematically replicate this visual scaling 
	
mathematics: similar triangles perspective projection:
. triangle 1: △(Eye, Screen_Centre, Screen_Edge)
. triangle 2: △(Eye, Wall_Centre, Wall_Edge)
	△₁ ~ △₂
	. same reference pts (centre) for similar ratios
ratio preservation: screen_height/focal_length = wall_height_world/wall_distance

	Wolfenstein constraint: All walls same world height (1.0 unit)
	Normalized case: focal_length = screen_height
	Result: wall_height_pixels = screen_height / wall_distance

behaviour:
	. Distance 1.0 → Wall fills screen (height = screen_height)
	. Distance 2.0 → Wall half screen (height = screen_height/2)
	. Distance → ∞ → Wall approaches 0 pixels	*/

static void	centre_wall_at_eye_level(int wall_height,
				int *wall_start, int *wall_end);
static void	enforce_screen_pixel_boundaries(int *wall_start, int *wall_end);

/* CALCULATE SCREEN WALL HEIGHT
Perspective projection: Convert world distance to screen pixels

Mathematical constraint analysis:
DDA returns distance to wall face, minimum theoretically > 0
However: floating point precision near grid boundaries could 
produce values approaching 0, causing overflow in division
Protection threshold - clamping (see .h)
Smaller distances treated as "player touching wall"

Core perspective projection: Similar triangles ratio
wall_height_pixels = screen_height / wall_distance	   

Rendering constraint: Prevent buffer overflow from extreme proximity
Maximum 2x screen height allows for reasonable "wall fills view" effect
without creating unmanageable pixel counts for column renderer  */
int	calculate_screen_wall_height(double world_wall_distance)
{
	int		screen_wall_height_pixels;
	double	world_wall_distance_protected;

	world_wall_distance_protected = world_wall_distance;
	if (world_wall_distance_protected < MINIMUM_WALL_DISTANCE_THRESHOLD)
		world_wall_distance_protected = MINIMUM_WALL_DISTANCE_THRESHOLD;
	screen_wall_height_pixels = (int)(g_game.graphics.screen_height
			/ world_wall_distance_protected);
	if (screen_wall_height_pixels > MAXIMUM_WALL_HEIGHT_PIXELS)
		screen_wall_height_pixels = MAXIMUM_WALL_HEIGHT_PIXELS;
	return (screen_wall_height_pixels);
}

/* HIGH-LEVEL PERSPECTIVE SIMULATION */
void	simulate_eye_level_perspective(int wall_height,
		int *wall_start, int *wall_end)
{
	centre_wall_at_eye_level(wall_height, wall_start, wall_end);
	enforce_screen_pixel_boundaries(wall_start, wall_end);
}

/* eye-level wall positioning 
why? human vision places horizon line at eye level
how? centre wall vertically on screen around midpoint
to simulate natural perspective

visual exp:
. looking straight ahead: C above, F below
. wall appears centred in FoV
. screen centre represents eye-level horizon line

math implementation:
wall_start = screen_centre_horizon_line - wall_height/2
wall_end = wall_start + wall_height
. screen midpt = natural horizon position
. wall spans equally above/below this line
. preserved perspective geometry from similar triangles */
static void	centre_wall_at_eye_level(int wall_height,
		int *wall_start, int *wall_end)
{
	int	screen_centre_horizon_line;

	screen_centre_horizon_line = g_game.graphics.screen_height / 2;
	*wall_start = screen_centre_horizon_line - (wall_height / 2);
	*wall_end = *wall_start + wall_height;
}

/* screen buffer has fixed dimensions [0, screen_height - 1]
implementation requirement: protect array bounds */
static void	enforce_screen_pixel_boundaries(int *wall_start, int *wall_end)
{
	if (*wall_start < 0)
		*wall_start = 0;
	if (*wall_end > g_game.graphics.screen_height)
		*wall_end = g_game.graphics.screen_height;
}
