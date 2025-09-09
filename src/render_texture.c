/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_texture.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 14:01:40 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/09 09:16:36 by go-donne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

/* 

Projection establishes spatial positioning and scale relationships. 
Texturing provides surface appearance and visual detail.

Structural relationship:
	Projection layer: Determines where surfaces appear and their relative sizes
	Texturing layer: Determines what those surfaces look like
	Combined system: Creates complete visual representation

Functional separation:
	Projection: Solves geometric problems (distance, perspective, boundaries)
	Texturing: Solves appearance problems (color, pattern, material identity)

Integration requirement: Both layers must coordinate to produce coherent visual output.


How do we map surface appearance data onto the geometric surfaces that projection has positioned?



Coordinate transformation problem:

Given state:
	Projection output: Wall appears at specific screen columns with calculated heights
	Texture data: Appearance information stored as 2D image array
	Required output: Each screen pixel needs a color value

The mapping challenge:
Screen pixel position → Wall surface position → Texture coordinate → Color value



Rendering method approaches:

Different rendering paradigms solve this transformation differently:

RASTERIZATION:
	Process: 3D surfaces → 2D screen projection → 
	interpolate texture coordinates across projected area
	Application: Complex 3D models with arbitrary surfaces

RAY TRACING:
	Process: Screen pixel → Ray → Surface intersection → Texture sample + lighting calculation
	Application: Photorealistic rendering with complex lighting

RAYCASTING:
	Process: Screen column → Ray → Wall intersection → Direct texture coordinate calculation
	Application: 2D map with vertical walls

Screen column (SCREEN SPACE): Pixel column index [0, screen_width-1]
Ray (WORLD SPACE): Mathematical line with origin (player_pos) + direction vector
Wall intersection (WORLD SPACE): Exact coordinates (x, y) where ray hits wall boundary
Texture coordinate calculation (TEXTURE SPACE): Transform world coordinates → [0,1] texture coordinates	


	

My specific transformation chain:

Input: Screen column X, wall distance D, wall face direction F
Output: Color value for each pixel in that column
Required mappings:
	Screen Y coordinate → Wall surface Y position
	Wall intersection point → Texture X coordinate
	Texture coordinates → Color value

	
clarification in terms of spaces:

1. "Screen Y coordinate → Wall surface Y position"
	Screen Y: SCREEN SPACE pixel row [0, screen_height-1]
	Wall surface Y: WALL-LOCAL SPACE - vertical position on the wall face [0,1]
	NOT world space Y - this is position up/down the wall surface itself

2. "Wall intersection point → Texture X coordinate"
	Wall intersection: WORLD SPACE coordinates (2.34, 4.67)
	Texture X: TEXTURE SPACE coordinate [0,1] representing horizontal position across texture

3. "Texture coordinates → Color value"
	Texture coordinates: TEXTURE SPACE [0,1] × [0,1]
	Color value: RGB data that gets written to SCREEN SPACE pixels

	

Recap:

We've established:
	Projection creates geometric skeleton, texturing adds surface appearance
	We need to map surface appearance data onto geometric surfaces
	We have different coordinate spaces: screen space, world space, wall-local space, texture space
	The transformation chain is: 
		screen space → world space → wall-local space → texture space → screen space


	


The Correspondence Problem:

	Current state: We know we need to map wall surface positions to texture positions.

	The fundamental question: How do we establish correspondence between:
		Any wall surface (different sizes, orientations, world positions)
		Any texture image (different pixel dimensions)


The Standardisation requirement:

	Problem: Wall surfaces have arbitrary world coordinates - different sizes, orientations
	Question: How do we make texture mapping independent of world position? 
		how to describe any position on any surface? 
	Recognition: We need a universal coordinate system for wall surfaces.
		that will give us, e.g. a universal way to say "middle"
	

How do we create this universal position description system?

Normalisation approach:
	core principle: convert absolute measurements to relative measurements:
	relative_position = absolute_position ÷ total_dimension

Range standardisation:
	What should the relative position range be?

	Options:
		0% to 100% (percentage system)
		0 to 1 (decimal fraction system)
		-1 to +1 (centered system)

	Choice rationale for [0,1]:
		Mathematical convenience: Direct multiplication with pixel dimensions
		Universal meaning: 0 = start, 1 = end, 0.5 = middle
		Graphics convention: Established standard across computer graphics


	
How do we apply this normalization to actual wall intersection calculations?
		





THE WALL SURFACE COORDINATE PROBLEM

Current data: Ray hits wall at world coordinates (5.23, 3.67)
Question: What does this world coordinate tell us about position on the wall surface?
Key insight: We need to separate the wall identification from the position within wall


DECOMPOSING THE WALL INTERSECTION

Given intersection: (5.23, 3.67)

	Step 1: Identify which wall cell
	Wall cell: (floor(5.23), floor(3.67)) = (5, 3)

	Step 2: Extract position within cell
	X position within cell: 5.23 - 5 = 0.23
	Y position within cell: 3.67 - 3 = 0.67

Recognition: These fractional parts (0.23, 0.67) are already normalized [0,1]



THE WALL FACE ORIENTATION PROBLEM
Critical question: Which fractional part represents horizontal position across wall face?

Wall face analysis:
	North/South faces: Wall runs East-West → horizontal = X fractional part
	East/West faces: Wall runs North-South → horizontal = Y fractional part

Example calculations:
North face hit: U = X_fractional = 0.23
East face hit:  U = Y_fractional = 0.67

The UV assignment:
	U coordinate: Horizontal position across wall face
	V coordinate: Vertical position up wall face (from screen pixel position)








The UV Coordinate System:

	Principle: Establish normalized coordinates for any surface
		U coordinate: Horizontal position across surface [0,1]
		V coordinate: Vertical position across surface [0,1]

	Key insight: Every wall surface, regardless of size or world position, maps to the same [0,1] × [0,1] coordinate space.
	
	
	


How do we calculate these normalized coordinates from world intersection points?



	
THE VERTICAL POSITION CHALLENGE

Current situation: We have U coordinate from wall intersection. But V coordinate represents vertical position on wall surface.
Key recognition: V coordinate doesn't come from world coordinates - it comes from screen pixel position.



THE SCREEN-TO-WALL MAPPING

Given data:
	Wall appears on screen: from wall_start_y to wall_end_y
	Current screen pixel: at position screen_y
Question: What wall surface height does this screen pixel represent?

Wall rendering spans:
	wall_start_y = 200 pixels (top of wall on screen)
	wall_end_y = 400 pixels   (bottom of wall on screen)
	wall_height = 200 pixels  (total wall height on screen)


	
THE NORMALIZATION CALCULATION

For screen pixel at position 300:

Step 1: Calculate offset from wall top
pixel_offset = screen_y - wall_start_y
pixel_offset = 300 - 200 = 100 pixels

Step 2: Calculate relative position
relative_position = pixel_offset ÷ total_wall_height
V = 100 ÷ 200 = 0.5

Meaning: This pixel represents the middle of the wall surface (50% down from top).

General formula:
V = (screen_y - wall_start_y) / (wall_end_y - wall_start_y)

Result: V coordinate [0,1] where 0 = wall top, 1 = wall bottom.






Next: How do we convert these UV coordinates to actual texture pixel indices?






THE TEXTURE SPACE CONVERSION
Precisely. The UV coordinates [0,1] represent relative positions that we apply to the texture image.
Given:

U = 0.3, V = 0.7 (wall surface coordinates)
Texture image: 128 × 128 pixels
Question: Which texture pixel corresponds to this wall position?


THE SCALING TRANSFORMATION

Convert relative position to absolute pixel index:

	Step 1: Scale U coordinate to texture width
	texture_x = U × texture_width
	texture_x = 0.3 × 128 = 38.4

	Step 2: Scale V coordinate to texture height
	texture_y = V × texture_height  
	texture_y = 0.7 × 128 = 89.6

	Step 3: Convert to integer indices
	pixel_x = (int)texture_x = 38
	pixel_y = (int)texture_y = 89



THE PIXEL ACCESS

Extract color from texture array:
	pixel_index = pixel_y × texture_width + pixel_x
	pixel_index = 89 × 128 + 38 = 11,422
	color = texture_data[pixel_index]
	
Complete transformation:
Wall surface position (30% across, 70% down)
→ Texture position (30% across, 70% down)  
→ Pixel indices (38, 89)
→ Color value

Recognition: The relative position relationship is preserved - 
30% across wall surface = 30% across texture image.


Final step: This color value gets written to the screen pixel that initiated the ray.













		
	
TEXTURE MAPPING - World Position → Texture Position

What it means: Calculating which pixel in the texture corresponds to a specific wall location.

Mathematical Transform: UV calculation
pixel conversion
texture pixel



TEXTURE SAMPLING - Extracting Color Values

What it means: Reading pixel color from texture at calculated coordinates.

Memory Operation:

	// Input: texture coordinates (19, 45)
	// Process: Array indexing  
	int pixel_index = 45 * 64 + 19;  // 2D → 1D conversion
	uint32_t color = texture.pixels[pixel_index];  // Memory read

	// Output: 0xFF8B4513 (brown brick color)

Computer Reality: Convert 2D texture position → 1D memory address → color value


*/


int get_wall_texture_color(t_texture_context *ctx, int screen_y)
{
    t_texture_image *texture;
    double          texture_u, texture_v;
    int             texture_x, texture_y;

    // STEP 1: Get texture for this wall direction
    texture = get_texture_for_direction(ctx->wall_direction);
    if (!texture || !texture->pixels)
        return (0xFF00FF); // Magenta fallback

    // STEP 2: Calculate texture U coordinate (horizontal)
    texture_u = calculate_texture_u(ctx);

    // STEP 3: Calculate texture V coordinate (vertical)  
    texture_v = calculate_texture_v(ctx, screen_y);

    // STEP 4: Convert UV [0,1] to pixel coordinates
    texture_x = (int)(texture_u * texture->width);
    texture_y = (int)(texture_v * texture->height);

    // STEP 5: Sample pixel from texture
    return sample_texture_pixel(texture, texture_x, texture_y);
}

double calculate_texture_u(t_texture_context *ctx)
{
    double wall_pos;

    // Different calculation based on wall orientation
    if (ctx->wall_direction == NORTH || ctx->wall_direction == SOUTH)
        wall_pos = ctx->wall_hit_x; // Vertical walls use X coordinate
    else
        wall_pos = ctx->wall_hit_y; // Horizontal walls use Y coordinate

    // Return fractional part [0,1]
    return (wall_pos - floor(wall_pos));
}

double calculate_texture_v(t_texture_context *ctx, int screen_y)
{
    int wall_start_y, wall_end_y;

    calculate_wall_boundaries(ctx->wall_height, &wall_start_y, &wall_end_y);

    // Map screen position to texture coordinate [0,1]
    if (wall_end_y <= wall_start_y)
        return (0.0);

    return ((double)(screen_y - wall_start_y) / (wall_end_y - wall_start_y));
}

int sample_texture_pixel(t_texture_image *tex, int tex_x, int tex_y)
{
    // Bounds protection
    if (tex_x >= tex->width) tex_x = tex->width - 1;
    if (tex_y >= tex->height) tex_y = tex->height - 1;
    if (tex_x < 0) tex_x = 0;
    if (tex_y < 0) tex_y = 0;

    // Linear array access: 2D → 1D conversion
    return (tex->pixels[tex_y * tex->width + tex_x]);
}

t_texture_image *get_texture_for_direction(int wall_direction)
{
    if (wall_direction >= 0 && wall_direction < 4)
        return (&g_game.map.wall_textures[wall_direction]);
    return (NULL);
}