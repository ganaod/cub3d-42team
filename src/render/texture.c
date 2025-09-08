/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 14:01:40 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/08 14:55:26 by go-donne         ###   ########.fr       */
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









	
TEXTURE MAPPING - World Position → Texture Position

What it means: Calculating which pixel in the texture corresponds to a specific wall location.

Mathematical Transform: UV calculation
pixel conversion
texture pixel



TEXTURE SAMPLING - Extracting Color Values

What it means: Reading pixel color from texture at calculated coordinates.

Memory Operation:

c
// Input: texture coordinates (19, 45)
// Process: Array indexing  
int pixel_index = 45 * 64 + 19;  // 2D → 1D conversion
uint32_t color = texture.pixels[pixel_index];  // Memory read
// Output: 0xFF8B4513 (brown brick color)
Computer Reality: Convert 2D texture position → 1D memory address → color value


*/