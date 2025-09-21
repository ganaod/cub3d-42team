/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_texture_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 14:07:54 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/21 14:18:14 by go-donne         ###   ########.fr       */
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
