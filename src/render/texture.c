/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 14:01:40 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/08 14:45:43 by go-donne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

/* projection mathematics create spatial relationships 

brain recognizes 3D space through surface detail patterns
hence texturing





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