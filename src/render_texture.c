/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_texture.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 14:01:40 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/19 11:41:40 by go-donne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* TEXTURE
. projection established spatial positioning, scale relationships
. texturing provides surface appearance, visual detail
. coordinate both for complete visual representation

how to map surface appearance onto geometric surfaces positioned by projection?
coordinate transformation problem:
	projection output: walls at screen columns with heights
	texture data as 2D image array
	required output: each screen pixel needs colour val
mapping challenge:
	screen pixel pos > wall surface pos > texture coord > colour val

texture mapping: world pos > texture pos
texture sampling: extract colour vals
	read pixel colour from texture at calculated coords

correspondence between:
	any map wall surface (diff sizes, orientations, world positions) &
	any texture image (diff pixel dimensions)
requires universal coord sys.
normalisation: converting abs > relative measurements
	(relative_pos = abs_pos / total_dimension)
range standardisation: decimal fraction sys, [0,1]
	convenience: direct multiplication with pixel dimensions
	graphics standard

	applied here:
1. identify wall cell
which coord represents horiz pos across wall depends on wall orientation

map space wall orientations:
	NORTH/SOUTH walls: horizontal boundaries (extend along X-axis)
	├── As you walk along them: X coordinate changes, Y stays constant
	└── For texture U: Use wall_hit_x (position along X-axis)

	EAST/WEST walls: Vertical boundaries (extend along Y-axis)
	├── As you walk along them: Y coordinate changes, X stays constant
	└── For texture U: Use wall_hit_y (position along Y-axis)

2. extract fractional part  */

#include "../inc/cub3d.h"

static double	world_wall_texture_u(t_texture_context *ctx);
static double	screen_wall_texture_v(t_texture_context *ctx, int current_pixel_y);
static int		texture_pixel_colour(t_texture_image *texture_image, int texture_pixel_x, int texture_pixel_y);

/* Transform single screen pixel → colour value
Input Space: SCREEN SPACE (pixel Y coordinate) + TEXTURE CONTEXT (world intersection data)
Output Space: COLOUR VALUE (final RGB pixel data)
Mathematical Process: Complete coordinate space pipeline orchestration
. Called for each pixel as we paint the wall */
int	screen_pixel_texture_colour(t_texture_context *ctx, int current_pixel_y)
{
	t_texture_image	*wall_texture_image;
	double			texture_coord_u;
	double			texture_coord_v;
	int				texture_pixel_x;
	int				texture_pixel_y;

	t_texture_image *tex = &g_game.map.wall_textures[ctx->world_wall_face];

	if (ctx->world_wall_face < WALL_NORTH || ctx->world_wall_face > WALL_EAST) {
		ft_printf("DEBUG: invalid wall_face=%d\n", ctx->world_wall_face);
		return 0xFF00FF; // magenta debug
	}
	if (!tex || !tex->pixels || tex->image_width <= 0 || tex->image_height <= 0) {
		ft_printf("DEBUG: texture not ready (face=%d)\n", ctx->world_wall_face);
		return 0xFF00FF;
	}

	wall_texture_image = &g_game.map.wall_textures[ctx->world_wall_face];
	texture_coord_u = world_wall_texture_u(ctx);
	texture_coord_v = screen_wall_texture_v(ctx, current_pixel_y);
	texture_pixel_x = (int)(texture_coord_u * wall_texture_image->image_width);
	texture_pixel_y = (int)(texture_coord_v * wall_texture_image->image_height);
	return (texture_pixel_colour(wall_texture_image, texture_pixel_x, texture_pixel_y));
}

/* World intersection → texture horizontal position
Input Space: WORLD SPACE (wall intersection coordinates)
Output Space: TEXTURE SPACE (horizontal position 0.0-1.0)
Mathematical Process: world_wall_position - floor(world_wall_position)
	floor calculation: fractional result represents relative position within
	grid cell, independent of cell's world location
. Ray impact becomes surface coordinate */
static double	world_wall_texture_u(t_texture_context *ctx)
{
	double	world_wall_intersection_coordinate;

	if (ctx->world_wall_face == WALL_NORTH || ctx->world_wall_face == WALL_SOUTH)
		world_wall_intersection_coordinate = ctx->world_wall_intersection_x;
	else
		world_wall_intersection_coordinate = ctx->world_wall_intersection_y;
	return (world_wall_intersection_coordinate - floor(world_wall_intersection_coordinate));
}

/* Screen pixel → texture vertical position
Input Space: SCREEN SPACE (pixel Y position)
Output Space: TEXTURE SPACE (vertical position 0.0-1.0)
Mathematical Process: (current_pixel_y - wall_start_y) / (wall_end_y - wall_start_y)
. Display location becomes surface coordinate */
static double	screen_wall_texture_v(t_texture_context *ctx, int current_pixel_y)
{
	int	screen_wall_start_y;
	int	screen_wall_end_y;

	calculate_wall_boundaries(ctx->screen_wall_height, &screen_wall_start_y, &screen_wall_end_y);
	if (screen_wall_end_y <= screen_wall_start_y)
		return (0.0);
	return ((double)(current_pixel_y - screen_wall_start_y) / (screen_wall_end_y - screen_wall_start_y));
}

/* Texture coordinates → memory colour
Input Space: TEXTURE SPACE (normalized UV coordinates converted to pixel indices)
Output Space: COLOUR VALUE (RGB data from texture memory)
Mathematical Process: Bounds protection + 2D→1D array indexing
. Surface position becomes pixel reality */
static int	texture_pixel_colour(t_texture_image *texture_image, int texture_pixel_x, int texture_pixel_y)
{
	if (texture_pixel_x >= texture_image->image_width)
		texture_pixel_x = texture_image->image_width - 1;
	if (texture_pixel_y >= texture_image->image_height)
		texture_pixel_y = texture_image->image_height - 1;
	if (texture_pixel_x < 0)
		texture_pixel_x = 0;
	if (texture_pixel_y < 0)
		texture_pixel_y = 0;
	return (texture_image->pixels[texture_pixel_y * texture_image->image_width + texture_pixel_x]);
}
