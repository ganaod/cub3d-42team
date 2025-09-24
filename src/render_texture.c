/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_texture.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 14:01:40 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/24 13:32:51 by go-donne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* TEXTURE
. projection established spatial positioning, scale relationships
. texturing provides surface appearance, visual detail
. coordinate both for complete visual representation

texture mapping: world pos > texture pos
texture sampling: extract colour vals	*/

#include "../inc/render.h"

static double	world_wall_texture_u(t_texture_context *ctx);
static double	screen_wall_texture_v(t_texture_context *ctx,
					int current_pixel_y);
static int		texture_pixel_colour(t_texture_image *texture_image,
					int texture_pixel_x, int texture_pixel_y);

/* Transform single screen pixel → colour value
Input Space: SCREEN SPACE (pixel Y coordinate)
	+ TEXTURE CONTEXT (world intersection data)
Output Space: COLOUR VALUE (final RGB pixel data)
Mathematical Process: Complete coordinate space pipeline orchestration
. Called for each pixel as we paint the wall 

invalid states:
. invalid wall face (corrupted ray data) 
. missing texture (failed file load)
. broken texture data (corrupted image dimensions)
these specific wall pixels render as bright magenta (0xFF00FF)
clearer debugging than silent black pixels / crash */
int	screen_pixel_texture_colour(t_texture_context *ctx, int current_pixel_y)
{
	t_texture_image	*tex;
	double			texture_coord_u;
	double			texture_coord_v;
	int				texture_pixel_x;
	int				texture_pixel_y;

	if (ctx->world_wall_face < WALL_NORTH || ctx->world_wall_face > WALL_EAST)
		return (0xFF00FF);
	tex = &g_game.map.wall_textures[ctx->world_wall_face];
	if (!tex || !tex->pixels || tex->image_width <= 0 || tex->image_height <= 0)
		return (0xFF00FF);
	texture_coord_u = world_wall_texture_u(ctx);
	texture_coord_v = screen_wall_texture_v(ctx, current_pixel_y);
	texture_pixel_x = (int)(texture_coord_u * tex->image_width);
	texture_pixel_y = (int)(texture_coord_v * tex->image_height);
	return (texture_pixel_colour(tex, texture_pixel_x, texture_pixel_y));
}

/* World intersection → texture horizontal position
in: WORLD SPACE (wall intersection coordinates)
out: TEXTURE SPACE (horizontal position 0.0-1.0)
Mathematical Process: world_wall_position - floor(world_wall_position)
	floor calculation: fractional result represents relative position within
	grid cell, independent of cell's world location
. Ray impact becomes surface coordinate */
static double world_wall_texture_u(t_texture_context *ctx)
{
    double wall_coordinate;
    
    if (ctx->world_wall_face == WALL_NORTH || ctx->world_wall_face == WALL_SOUTH)
        wall_coordinate = ctx->world_wall_intersection_x;  // Use parametric X
    else
        wall_coordinate = ctx->world_wall_intersection_y;  // Use parametric Y
        
    return safe_fractional_part(wall_coordinate);
}

/* Screen pixel → texture vertical position
Input Space: SCREEN SPACE (pixel Y position)
Output Space: TEXTURE SPACE (vertical position 0.0-1.0)
Mathematical Process: (current_pixel_y - wall_start_y)
	/ (wall_end_y - wall_start_y)
. Display location becomes surface coordinate */
static double	screen_wall_texture_v(t_texture_context *ctx, int curr_px_y)
{
	int		start_y;
	int		end_y;
	double	span;

	simulate_eye_level_perspective(ctx->screen_wall_height,
		&start_y, &end_y);
	span = end_y - start_y;
	if (span <= 0)
		return (0.0);
	return ((double)(curr_px_y - start_y) / span);
}

/* Texture coordinates → memory colour
Input Space: TEXTURE SPACE (normalized UV coordinates converted to pixel indices)
Output Space: COLOUR VALUE (RGB data from texture memory)
Mathematical Process: Bounds protection + 2D→1D array indexing
. Surface position becomes pixel reality */
static int	texture_pixel_colour(t_texture_image *tx_img,
	int tx_px_x, int tx_px_y)
{
	int	clamped_w;
	int	clamped_h;

	if (!tx_img || !tx_img->pixels)
		return (0xFF00FF);
	if (tx_img->image_width <= 0 || tx_img->image_height <= 0)
		return (0xFF00FF);
	clamped_w = clamp_texture_pixel(tx_px_x, tx_img->image_width);
	clamped_h = clamp_texture_pixel(tx_px_y, tx_img->image_height);
	return (tx_img->pixels[clamped_h * tx_img->image_width + clamped_w]);
}
