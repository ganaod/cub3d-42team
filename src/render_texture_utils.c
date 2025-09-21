/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_texture_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 14:07:54 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/21 14:37:32 by go-donne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/render.h"

double	safe_fractional_part(double coordinate)
{
	double	fractional;

	fractional = coordinate - floor(coordinate);
	if (fractional < 0.0)
		fractional += 1.0;
	if (fractional >= 1.0)  // Floating point precision protection
		fractional = 0.999999;
	return (fractional);
}

/* Clamp texture pixel coordinates to valid image bounds 
(instead of wrapping with %, good for tiling) */
int	clamp_texture_pixel(int pixel_coord, int max_dimension)
{
	if (pixel_coord < 0)
		return (0);
	if (pixel_coord >= max_dimension)
		return (max_dimension - 1);
	return (pixel_coord);
}

/* theory: colour representation in graphics

channel = 1 component of color information stored separately

RBG model:
	Red Channel: Intensity of red light (0-255)
	Green Channel: Intensity of green light (0-255)
	Blue Channel: Intensity of blue light (0-255)
	Alpha Channel: Transparency/opacity (0-255)

all get a value from 0-255 (decimal) / 0-? (hexadecimal)	
	
Physical Reality: Computer screens emit red, green, blue light. 
Combining intensities creates perceived colors.

Mathematical Model: 
Color = (Red_Intensity, Green_Intensity, Blue_Intensity, Opacity)


WHAT IS ALPHA CHANNEL?
	Alpha = Opacity Control
	Alpha 0   = Completely Transparent (invisible)
	Alpha 127 = 50% Transparent (semi-transparent)  
	Alpha 255 = Completely Opaque (solid color)

Why Alpha Exists: Enables compositing - layering graphics with transparency effects.

Example:
Window with Alpha 200 = Mostly solid but slightly see-through
Smoke effect with Alpha 50 = Very transparent overlay




MLX42 FORMAT EXPECTATION
RGBA8 Format:
MLX42 expects 32-bit RGBA8: 0xRRGGBBAA



Memory Layout Differences:
Different graphics systems arrange the same color data in different byte orders:
Big-Endian RGBA:
Memory: [0x87][0xCE][0xFA][0xFF]
Reads:   Red   Green Blue  Alpha
Little-Endian RGBA:
Memory: [0xFF][0xFA][0xCE][0x87]
Reads:   Alpha Blue  Green Red
BGRA Format:
Memory: [0xFA][0xCE][0x87][0xFF]  
Reads:   Blue  Green Red   Alpha
These represent THE SAME visual color but stored differently in memory.



therefore parsing utils updated to include the alpha channel, 
opacity set to 255 (fully opaque)


cross-platform programming is complex - 
what works on your machine may fail elsewhere due to byte ordering assumptions.

MLX42 bridges platforms?
*/