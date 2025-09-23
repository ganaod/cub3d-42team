/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 11:33:20 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/23 11:36:52 by go-donne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../inc/cub3d.h"

static void	choose_render_resolution(int window_w, int window_h, 
	int *render_w, int *render_h)
{
if (window_w > RENDER_WIDTH_THRESHOLD || window_h > RENDER_HEIGHT_THRESHOLD)
{
*render_w = INTERNAL_RENDER_WIDTH;
*render_h = INTERNAL_RENDER_HEIGHT;
}
else
{
*render_w = window_w;
*render_h = window_h;
}
}

int	gfx_rebuild_framebuffer(t_graphics *g, int window_w, int window_h)
{
mlx_image_t	*new_frame;
int			render_w;
int			render_h;

if (!g || !g->mlx || window_w <= 0 || window_h <= 0)
return (0);
if (g->frame)
{
mlx_delete_image(g->mlx, g->frame);
g->frame = NULL;
}
choose_render_resolution(window_w, window_h, &render_w, &render_h);
new_frame = mlx_new_image(g->mlx, (uint32_t)render_w, (uint32_t)render_h);
if (!new_frame)
return (0);
g->frame = new_frame;
g->screen_width = render_w;
g->screen_height = render_h;
return (1);
}