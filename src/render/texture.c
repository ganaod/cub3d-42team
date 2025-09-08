/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 14:01:40 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/08 15:51:07 by go-donne         ###   ########.fr       */
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
		


.... MORE INTERMEDIARY STEPS HERE ....



The UV Coordinate System:

	Principle: Establish normalized coordinates for any surface
		U coordinate: Horizontal position across surface [0,1]
		V coordinate: Vertical position across surface [0,1]

	Key insight: Every wall surface, regardless of size or world position, maps to the same [0,1] × [0,1] coordinate space.
	
	
	


How do we calculate these normalized coordinates from world intersection points?



	




		
	
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