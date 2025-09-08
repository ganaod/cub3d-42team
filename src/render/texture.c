/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 14:01:40 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/08 15:09:20 by go-donne         ###   ########.fr       */
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



RENDERING METHOD APPROACHES
Different rendering paradigms solve this transformation differently:

RASTERIZATION:
	Process: 3D surfaces → 2D screen projection → 
	interpolate texture coordinates across projected area
	Application: Complex 3D models with arbitrary surfaces

RAYCASTING:
	Process: Screen column → Ray → Wall intersection → Direct texture coordinate calculation
	Application: 2D map with vertical walls

RAY TRACING:
	Process: Screen pixel → Ray → Surface intersection → Texture sample + lighting calculation
	Application: Photorealistic rendering with complex lighting







	


	
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