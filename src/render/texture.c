/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 14:01:40 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/08 10:43:33 by go-donne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
add surface reality through coordinate transformation from world space to texture space

without texture mapping, you have geometric simulation but not visual conviction. 
Texturing transforms abstract space into experiential place

Your world position determines which texture region you see. 
This creates spatial continuity - the fundamental illusion 
that texture exists on wall surfaces rather than just being painted on screen


texture pipeline - data flow architecture:

render_single_column()
    ↓ [Creates texture context]
render_wall_section() 
    ↓ [Passes context + screen_y]
get_wall_texture_color()
    ↓ [Calculates UV coordinates]
sample_texture_pixel()
    ↓ [Extracts color from texture]
put_pixel()



Function Responsibility Breakdown

get_wall_texture_color(): Coordinate transformation

	Input: Texture context + screen Y position
	Process: World coordinates → UV texture coordinates
	Output: Color value

sample_texture_pixel(): Memory access

	Input: Texture pointer + UV coordinates
	Process: Bounds checking + array indexing
	Output: RGBA pixel value

get_texture_for_direction(): Texture selection

	Input: Wall direction [0-3]
	Process: Array lookup
	Output: Texture pointer
*/

#include "../inc/cub3d.h"
