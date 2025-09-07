/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 14:01:40 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/07 15:26:15 by go-donne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

texture_image *get_texture_for_direction(int wall_direction);
int calculate_texture_x(double wall_hit_pos, int wall_side);
int calculate_texture_y(int screen_y, int wall_start, int wall_height);
int sample_texture_pixel(texture_image *tex, int tex_x, int tex_y);

// /*
// TEXTURE MAPPING: Map wall texture to screen coordinates
// */
// int get_wall_texture_color(int wall_direction, int screen_y, int wall_height)
// {
//     // Determine which texture to use based on wall face
//     texture_image *wall_texture = get_texture_for_direction(wall_direction);
    
//     // Calculate texture coordinates
//     double texture_y_ratio = (double)(screen_y - wall_start_y) / wall_height;
//     int texture_y = (int)(texture_y_ratio * wall_texture->height);
    
//     // Extract color from texture at calculated coordinates
//     return (extract_texture_pixel(wall_texture, texture_x, texture_y));
// }



// TEMPORARY
/*
SOLID COLOR VERIFICATION
Purpose: Verify wall face detection accuracy before texture complexity
Strategy: Each wall face gets distinct solid color
*/

int get_wall_texture_color(int wall_direction, int screen_y, int wall_height)
{
    // Solid colors for each wall face - highly distinct for verification
    if (wall_direction == NORTH)
        return (0xFF0000);  // Bright Red
    else if (wall_direction == SOUTH)  
        return (0x00FF00);  // Bright Green
    else if (wall_direction == EAST)
        return (0x0000FF);  // Bright Blue  
    else if (wall_direction == WEST)
        return (0xFFFF00);  // Yellow
    
    return (0xFF00FF);      // Magenta fallback (debug indicator)
}