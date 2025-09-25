/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_texture.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 20:26:50 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/25 20:26:55 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/render.h"

/* Validate texture data and prepare rendering context
Input: Game state, column bounds, wall intersection data
Output: Populated texture step data structure */
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

/* INCREMENTAL TEXTURE STEPPING OPTIMIZATION
PROBLEM: Original per-pixel approach required expensive division per pixel:
   v = (current_y - wall_start) / wall_span  [~100-500 divisions per column]

SOLUTION: Pre-calculate step size once, then use simple addition per pixel:
   v_increment = 1.0 / wall_span  (once per column)
   v += v_increment               (per pixel - just addition)

ANALOGY: Painting a wall stripe using a photo reference
- OLD: For each brushstroke, measure distance from top, calculate %,
	find photo position
- NEW: Pre-calculate "each brushstroke moves 1% down the photo",
	just add 1% each time

MATHEMATICAL PROOF: Both produce identical linear interpolation sequences:
   Per-pixel: 0.0, 1/span, 2/span, 3/span, ...
   Incremental: 0.0, 0+dv, 0+dv+dv, 0+dv+dv+dv, ... (where dv = 1/span)

PERFORMANCE: ~50% reduction in arithmetic operations, identical visual output */
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
