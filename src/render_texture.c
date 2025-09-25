/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_texture.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 11:22:37 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/25 11:25:48 by go-donne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/render.h"

/* Validate texture data and prepare rendering context
Input: Game state, column bounds, wall intersection data
Output: Populated texture step data structure
Returns: 1 on success, 0 on invalid texture/wall face */
int	setup_wall_texture_data(t_game *g, t_screen_column_bounds *bounds,
		t_ray_result *wall_hit, t_texture_step_data *data)
{
	t_texture_context	ctx;
	double				u;

	ctx.world_wall_face = wall_hit->world_wall_face;
	ctx.world_wall_intersection_x = wall_hit->world_intersection_x;
	ctx.world_wall_intersection_y = wall_hit->world_intersection_y;
	ctx.screen_wall_height = bounds->wall_end_y - bounds->wall_start_y;
	if (ctx.world_wall_face < WALL_NORTH || ctx.world_wall_face > WALL_EAST)
		return (0);
	data->texture = &g->map.wall_textures[ctx.world_wall_face];
	if (!data->texture || !data->texture->pixels)
		return (0);
	if (data->texture->image_width <= 0 || data->texture->image_height <= 0)
		return (0);
	u = world_wall_texture_u(&ctx);
	data->texture_x = (int)(u * data->texture->image_width);
	data->start_y = bounds->wall_start_y;
	data->end_y = bounds->wall_end_y;
	return (1);
}

/* Calculate texture coordinate stepping for incremental rendering
Input: Texture step data with screen boundaries
Output: Sets v_current to 0.0 and v_increment for each pixel step
Optimization: One division per column instead of per pixel */
void	calculate_texture_stepping(t_texture_step_data *data)
{
	int	span;

	span = data->end_y - data->start_y;
	if (span <= 0)
	{
		data->v_increment = 0.0;
		return ;
	}
	data->v_current = 0.0;
	data->v_increment = 1.0 / (double)span;
}

/* Render vertical wall column with incremental texture sampling
Input: Game state, screen column X, prepared texture step data
Process: Incremental V stepping eliminates per-pixel divisions
Output: Textured pixels written to frame buffer */
void	render_textured_pixels(t_game *g, int column_x,
			t_texture_step_data *data)
{
	int			y;
	int			tx_y;
	uint32_t	color;

	y = data->start_y;
	while (y < data->end_y)
	{
		tx_y = (int)(data->v_current * data->texture->image_height);
		color = sample_texture_pixel(data->texture, data->texture_x, tx_y);
		put_pixel(g, column_x, y, color);
		data->v_current += data->v_increment;
		y++;
	}
}

/* Convert world wall intersection to texture horizontal coordinate
Input: Texture context with world intersection point and wall face
Output: Normalized U coordinate (0.0 to 1.0) across texture width
Process: Extract fractional part of world coordinate for texture mapping */
double	world_wall_texture_u(t_texture_context *ctx)
{
	double	wall_coordinate;

	if (ctx->world_wall_face == WALL_NORTH
		|| ctx->world_wall_face == WALL_SOUTH)
		wall_coordinate = ctx->world_wall_intersection_x;
	else
		wall_coordinate = ctx->world_wall_intersection_y;
	return (safe_fractional_part(wall_coordinate));
}

/* Extract fractional part of coordinate for texture mapping
Input: World coordinate (can be negative or > 1.0)
Output: Normalized fractional part in range [0.0, 1.0)
Handles: Negative coordinates and ensures valid texture coordinate range */
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
