/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_texture_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 14:07:54 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/25 11:24:39 by go-donne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/render.h"

double	safe_fractional_part(double coordinate)
{
	double	fractional;

	fractional = coordinate - floor(coordinate);
	if (fractional < 0.0)
		fractional += 1.0;
	if (fractional >= 1.0)
		fractional = 0.999999;
	return (fractional);
}

/* Sample texture pixel with coordinate clamping
Input: Texture image, raw texture coordinates
Output: Clamped ARGB color value from texture memory
Process: Bounds protection + 2D array indexing */
uint32_t	sample_texture_pixel(t_texture_image *texture, int tex_x, int tex_y)
{
	int	cx;
	int	cy;

	if (tex_x < 0)
		cx = 0;
	else
		cx = tex_x;
	if (cx >= texture->image_width)
		cx = texture->image_width - 1;
	if (tex_y < 0)
		cy = 0;
	else
		cy = tex_y;
	if (cy >= texture->image_height)
		cy = texture->image_height - 1;
	return (texture->pixels[cy * texture->image_width + cx]);
}
