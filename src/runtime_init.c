/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runtime_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 14:49:59 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/23 14:50:29 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void	sync_player_world_fields_from_parser(t_game *g)
{
	g->player.world_camera_plane_x = -g->player.world_dir_y
		* FOV_CAMERA_PLANE_MAGNITUDE;
	g->player.world_camera_plane_y = g->player.world_dir_x
		* FOV_CAMERA_PLANE_MAGNITUDE;
}

int	start_window(t_game *g, const char *title, int w, int h)
{
	if (!g || !title)
		return (0);
	if (!gfx_open_window_resizable(g, title, w, h))
		return (0);
	mlx_image_to_window(g->graphics.mlx, g->graphics.frame, 0, 0);
	mlx_resize_hook(g->graphics.mlx, handle_resize, g);
	return (1);
}

void	init_runtime_defaults(t_game *g)
{
	if (!g)
		return ;
	g->movement_speed = 3.0;
	g->rotation_speed = 2.0;
	sync_player_world_fields_from_parser(g);
}
