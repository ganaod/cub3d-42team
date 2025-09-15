/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_texture.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 14:01:40 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/15 18:25:08 by go-donne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

/* projection established spatial positioning, scale relationships
texturing provides surface appearance, visual detail
coordinate both for complete visual representation 

how to map surface appearance onto geometric surfaces positioned by projection? 
coordinate transformation problem:
	projection output: walls at screen columns with heights
	texture data as 2D image array
	required output: each screen pixel needs colour val
mapping challenge:
	screen pixel pos > wall surface pos > texture coord > colour val

ray casting:
	screen col (pixel col index) > ray (world space)
	> wall intersection (world space), exact coords
	> texture coord calc. colour val gets written to screen space pixels 
	
texture mapping: world pos > texture pos
texture sampling: extract colour vals 
	read pixel colour from texture at calculated coords */

int	get_wall_texture_colour(t_texture_context *ctx, int screen_y)
{
	t_texture_image  *texture;    // STEP 1: Texture selection
    double           texture_u;   // STEP 2: Abstract U coordinate [0,1]  
    double           texture_v;   // STEP 3: Abstract V coordinate [0,1]
    int              texture_x;   // STEP 4: Concrete X pixel index
    int              texture_y;   // STEP 5: Concrete Y pixel index

	texture = get_texture_for_direction(ctx->wall_direction);
	if (!texture || !texture->pixels)
		return (0xFF00FF);
	texture_u = calculate_texture_u(ctx);
	texture_v = calculate_texture_v(ctx, screen_y);
	texture_x = (int)(texture_u * texture->width);
	texture_y = (int)(texture_v * texture->height);
	return (sample_texture_pixel(texture, texture_x, texture_y));
}

static t_texture_image	*get_texture_for_direction(int wall_direction)
{
	if (wall_direction >= 0 && wall_direction < 4)
		return (&g_game.map.wall_textures[wall_direction]);
	return (NULL);
}

/* correspondence between: 
	any map wall surface (diff sizes, orientations, world positions) &
	any texture image (diff pixel dimensions)
requires universal coord sys.
normalisation: converting abs > relative measurements
	relative_pos = abs_pos / total_dimension
range standardisation: decimal fraction sys, [0,1]
	convenience: direct multiplication with pixel dimensions
	graphics standard
applied here: 
*/

/* 1. identify wall cell
which coord represents horiz pos across wall depends on wall orientation

map space wall orientations:
	NORTH/SOUTH walls: horizontal boundaries (extend along X-axis)
	├── As you walk along them: X coordinate changes, Y stays constant
	└── For texture U: Use wall_hit_x (position along X-axis)

	EAST/WEST walls: Vertical boundaries (extend along Y-axis)  
	├── As you walk along them: Y coordinate changes, X stays constant
	└── For texture U: Use wall_hit_y (position along Y-axis)

2. extract fractional part  */
static double	calculate_texture_u(t_texture_context *ctx)
{
	double	wall_pos;

	if (ctx->wall_direction == NORTH || ctx->wall_direction == SOUTH)
		wall_pos = ctx->wall_hit_x;
	else
		wall_pos = ctx->wall_hit_y;
	return (wall_pos - floor(wall_pos));
}

/* input: screen pixel pos (abs)
process: normalisation ( (current - start) / (total range) )*/
static double	calculate_texture_v(t_texture_context *ctx, int screen_y)
{
	int	wall_start_y;
	int	wall_end_y;

	calculate_wall_boundaries(ctx->wall_height, &wall_start_y, &wall_end_y);
	if (wall_end_y <= wall_start_y)
		return (0.0);
	return ((double)(screen_y - wall_start_y) / (wall_end_y - wall_start_y));
}

static int	sample_texture_pixel(t_texture_image *tex, int tex_x, int tex_y)
{
	if (tex_x >= tex->width)
		tex_x = tex->width - 1;
	if (tex_y >= tex->height)
		tex_y = tex->height - 1;
	if (tex_x < 0)
		tex_x = 0;
	if (tex_y < 0)
		tex_y = 0;
	return (tex->pixels[tex_y * tex->width + tex_x]);
}


/*

Texture System Pipeline - Five Core Functions:

get_wall_texture_colour() - Main Orchestrator

	Input: Texture context + screen Y position
	Process: Coordinates entire texture sampling pipeline
	Output: Final colour value
	Role: The conductor - calls all other texture functions in sequence

	
calculate_texture_u() - Horizontal Coordinate

	Input: Wall hit coordinates from ray intersection
	Process: Extracts fractional part for horizontal position [0,1]
	Logic: Vertical walls use X coordinate, horizontal walls use Y coordinate
	Output: U coordinate representing position across wall surface

	
calculate_texture_v() - Vertical Coordinate

	Input: Screen Y position + wall height
	Process: Maps screen pixel position to wall surface position [0,1]
	Logic: (screen_y - wall_start_y) / wall_height
	Output: V coordinate representing position up/down wall surface

	
sample_texture_pixel() - Memory Access

	Input: Texture image + pixel coordinates
	Process: Bounds protection + 2D→1D array indexing
	Logic: pixels[tex_y * tex_width + tex_x]
	Output: Raw colour value from texture memory

	
get_texture_for_direction() - Texture Selection

	Input: Wall direction (NORTH/SOUTH/EAST/WEST)
	Process: Array lookup in global texture storage
	Output: Pointer to appropriate texture image




The Complete Transform Chain:

Wall hit (2.34, 4.67) + Screen pixel Y=300
    ↓ calculate_texture_u()
U=0.34 (horizontal position across wall)
    ↓ calculate_texture_v()  
V=0.6 (vertical position up wall)
    ↓ UV→pixel conversion
Texture coordinates (34, 60) in 100x100 texture
    ↓ sample_texture_pixel()
Colour value 0xFF8B4513 (from texture memory)



*/





// /* temp / demo fn - hardcoded wall colours 
// . demonstrates basic wall colouring principle without texture files
// . usage: replace get_wall_texture_colour() in render_wall_section() */
// int get_wall_hardcoded_color(int wall_direction)
// {
// 	if (wall_direction == NORTH)
// 		return (0xFF4169E1);	// Royal Blue
// 	else if (wall_direction == SOUTH)  
// 		return (0xFF32CD32);	// Lime Green
// 	else if (wall_direction == EAST)
// 		return (0xFFDC143C);	// Crimson Red  
// 	else if (wall_direction == WEST)
// 		return (0xFFFF8C00);	// Dark Orange
// 	else
// 		return (0xFFFF00FF);	// Magenta fallback
// }