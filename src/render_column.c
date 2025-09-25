/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_column.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 10:54:44 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/25 10:45:01 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* screen-frame with its 3 zones:

┌─────────────────┐
│     CEILING     │ (rays above horizontal centreline)
│                 │
├╴╴╴╴╴╴╴╴╴╴╴╴╴╴╴╴╴┤ ← screen_y = wall_start_y_pixel
│      WALL       │ (ray intersection determines height)
│                 │
├╴╴╴╴╴╴╴╴╴╴╴╴╴╴╴╴╴┤ ← screen_y = wall_end_y_pixel
│      FLOOR      │
│                 │	(rays below horizontal centreline)
└─────────────────┘

re. texture, from subject:
	"display different wall textures"
	"set the floor and ceiling to two different colours"

hence
1 helper per section, texturing
only in	render_wall_section(void) */

#include "../inc/render.h"

void	render_ceiling_section(t_game *g, int screen_column_x,
		int wall_start_y_pixel)
{
	int			y;
	int			h;
	uint32_t	color;

	if (!g || !g->graphics.frame)
		return ;
	h = g->graphics.screen_height;
	if (wall_start_y_pixel < 0)
		wall_start_y_pixel = 0;
	if (wall_start_y_pixel > h)
		wall_start_y_pixel = h;
	color = g->map.ceiling_color;
	y = 0;
	while (y < wall_start_y_pixel)
	{
		put_pixel(g, screen_column_x, y, color);
		y++;
	}
}

void	render_wall_section(t_game *g, t_screen_column_bounds *screen_bounds,
	t_ray_result *wall_hit_data)
{
int					y;
uint32_t			color;
t_texture_context	ctx;

// --- TODO 1: build ctx once
ctx.world_wall_face = wall_hit_data->world_wall_face;
ctx.world_wall_intersection_x = wall_hit_data->world_intersection_x;
ctx.world_wall_intersection_y = wall_hit_data->world_intersection_y;
ctx.screen_wall_height = screen_bounds->wall_end_y - screen_bounds->wall_start_y;

// --- TODO 2: precompute texture ptr + stepping
t_texture_image *tex = &g->map.wall_textures[ctx.world_wall_face];
if (!tex || !tex->pixels || tex->image_width <= 0 || tex->image_height <= 0)
	return;

const double u = world_wall_texture_u(&ctx);           // once per column
const int tx_x = (int)(u * tex->image_width);          // once per column
const int wall_h = ctx.screen_wall_height;
if (wall_h <= 0) return;

// project once (instead of inside screen_wall_texture_v)
const int start_y = screen_bounds->wall_start_y;
const int end_y   = screen_bounds->wall_end_y;
const double inv_span = 1.0 / (double)(end_y - start_y);  // safe: wall_h>0
double v = 0.0;                               // at y == start_y
const double dv = inv_span;                   // per-pixel increment

// --- TODO 3: draw pixels with incremental v
y = start_y;
while (y < end_y)
{
	const int tx_y = (int)(v * tex->image_height);
	// clamp & sample (avoid the function call overhead in hot path)
	const int cx = (tx_x < 0) ? 0 : (tx_x >= tex->image_width ? tex->image_width - 1 : tx_x);
	const int cy = (tx_y < 0) ? 0 : (tx_y >= tex->image_height ? tex->image_height - 1 : tx_y);
	color = tex->pixels[cy * tex->image_width + cx];

	put_pixel(g, screen_bounds->column_x, y, color);
	v += dv;
	y++;
}
}

void	render_floor_section(t_game *g, int screen_column_x,
		int wall_end_y_pixel)
{
	int	y;

	y = wall_end_y_pixel;
	while (y < g->graphics.screen_height)
	{
		put_pixel(g, screen_column_x, y, g->map.floor_color);
		y++;
	}
}
