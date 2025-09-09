/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 17:04:34 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/09 11:15:49 by go-donne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

/* 
rendering module
core operations:

	1. Ray Direction:    Screen column → World ray vector
	2. Ray Intersection: Ray vector → Wall hit point + distance  
	3. Projection:       Distance → Screen wall height
	4. Boundaries:       Wall height → Pixel start/end positions
	5. Texture Mapping:  Hit point + screen pixel → Texture coordinates
	6. Texture Sampling: Texture coordinates → Colour value
	7. Pixel Drawing:    Colour → Screen buffer

key decisions:

	column-based rendering
		Why: Ray casting gives distance per column, not per pixel
		Mathematical necessity: One ray → one distance → entire vertical strip

	Decision 2: Data flow preservation
		Why: Texture sampling needs ray intersection data
		Method: Pass t_ray_result through call chain instead of decomposing early

	Decision 3: Texture context construction at render_wall_section level
		Why: This level has both ray data AND precise wall boundaries
		Upstream levels: Missing wall boundaries
		Downstream levels: Don't exist (next is texture functions)

	Decision 4: 42 Norm parameter bundling
		Constraint: Max 4 parameters per function
		Reality: Texture calculation needs 5+ pieces of data
		Solution: Bundle into t_texture_context struct


execution pipeline:

	render_complete_frame()           // Frame iteration
		render_single_column()        // Ray → geometry coordination  
			calculate_ray_direction() // Screen → ray transform
			cast_ray_to_wall()        // Ray → intersection
			calculate_screen_wall_height() // Distance → pixels
			render_wall_column()      // Visual composition
				render_wall_section() // Texture context + pixel iteration
					get_wall_texture_colour() // UV calculation + sampling
						put_pixel()   // Buffer write

result: 2D map data → 3D visual representation 	*/


/* 1. initialise buffer
. clear screen buff to known state (black pixels)
. prepare canvas for additive composition
(compose final image by adding visual elements (ceiling + wall + floor))

2. raycasting core loop
. for each screen col: cast ray, find wall, draw vert strip
. screen width determines ray count (1 ray / col)
. rays spread across player's FOV */
void	render_complete_frame(void)
{
	int	screen_x;
	int	screen_width;

	clear_screen_buffer();
	screen_width = g_game.graphics.screen_width;
	screen_x = 0;
	while (screen_x < screen_width)
	{
		render_single_column(screen_x);
		screen_x++;
	}
}

/* render_single_column:
   processes and renders the wall slice for a single screen column.
   flow: ray direction → wall intersection → projected wall height → draw pixels

   parameters:
     screen_x    -> column index on the screen to render

   steps:
     1. calculate the ray direction based on the column position
     2. cast the ray to find the wall hit result and distance
     3. compute the projected wall height for the screen
     4. render the vertical wall slice with the correct texture/face
*/
void	render_single_column(int screen_x)
{
	double			ray_dir_x;
	double			ray_dir_y;
	t_ray_result	ray_result;
	int				wall_height;

	calculate_ray_direction(screen_x, &ray_dir_x, &ray_dir_y);
	ray_result = cast_ray_to_wall(ray_dir_x, ray_dir_y);
	wall_height = calculate_screen_wall_height(ray_result.distance);
	render_wall_column(screen_x, &ray_result, wall_height);
}

/*

COLUMN-BASED RENDERING: MATHEMATICAL NECESSITY
Core Insight: One ray cast = One distance measurement = One entire vertical wall strip

Why Columns, Not Rows:

	Ray-to-distance mapping: Each ray gives distance to nearest wall
	Wolfenstein constraint: All walls same height → distance determines entire column height
	Perspective projection: Wall height = screen_height / distance (applies to whole column)
	Texture efficiency: Sample texture once per column, not per pixel

Visual Reality:
	Column 0: Ray → Distance 5.2 → Wall height 150px → Draw 150px vertical strip
	Column 1: Ray → Distance 4.8 → Wall height 162px → Draw 162px vertical strip  
	Column 2: Ray → Distance 6.1 → Wall height 127px → Draw 127px vertical strip
	
Mathematical Truth: Each ray provides sufficient information to render an entire vertical screen column.
Performance: 320 ray calculations instead of 320×240 = 76,800 pixel calculations.

The column approach exploits the constraint that walls are uniform height - 
one distance measurement determines the entire vertical appearance.

*/

// STILL NEEDS TO CALL TEXTURING SUB-MODULE

/* render wall: 1 complete vertical strip/slice of the 3D perspective view
convert dist to screen coordinates */
void	render_wall_column(int screen_x, t_ray_result *ray_result, int wall_height)
{
	int	wall_start_y;
	int	wall_end_y;

	wall_start_y = (g_game.graphics.screen_height - wall_height) / 2;
	wall_end_y = wall_start_y + wall_height;
	render_ceiling_section(screen_x, wall_start_y);
	render_wall_section(screen_x, wall_start_y, wall_end_y, ray_result);
	render_floor_section(screen_x, wall_end_y);
}
/* vertically center the wall with ceiling above & floor below
to simulate eye-level perspective:

screen coordinate sys:
y=0    ┌─────────────────┐  ← Top of screen
       │    CEILING      │
       │                 │
y=284  ├─────────────────┤  ← wall_start_y
       │                 │
       │      WALL       │  ← Wall centered on screen
       │                 │
y=484  ├─────────────────┤  ← wall_end_y  
       │                 │
       │     FLOOR       │
       │                 │
y=767  └─────────────────┘  ← Bottom of screen

screen_height = 768
wall_height = 200
wall_start_y = (768 - 200) / 2 = 284   // Center the wall
wall_end_y = 284 + 200 = 484           // Wall bottom edge

Rendering Order (top-down):
	y = 0 → 283: Paint ceiling pixels
	y = 284 → 483: Paint wall texture pixels
	y = 484 → 767: Paint floor pixels	*/
