/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 09:32:59 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/25 20:33:03 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

double	clampd(double v, double lo, double hi)
{
	if (v < lo)
		return (lo);
	if (v > hi)
		return (hi);
	return (v);
}

void	try_move_player(t_game *g, double new_x, double new_y)
{
	double	r;

	if (!g)
		return ;
	r = 0.15;
	if (!collides_with_wall(&g->map, new_x, g->player.world_pos_y, r))
		g->player.world_pos_x = new_x;
	if (!collides_with_wall(&g->map, g->player.world_pos_x, new_y, r))
		g->player.world_pos_y = new_y;
}

void	rotate_player(t_game *g, double angle)
{
	double	old_dir_x;
	double	old_plane_x;

	if (!g)
		return ;
	old_dir_x = g->player.world_dir_x;
	old_plane_x = g->player.world_camera_plane_x;
	g->player.world_dir_x = old_dir_x * cos(angle) - g->player.world_dir_y
		* sin(angle);
	g->player.world_dir_y = old_dir_x * sin(angle) + g->player.world_dir_y
		* cos(angle);
	g->player.world_camera_plane_x = old_plane_x * cos(angle)
		- g->player.world_camera_plane_y * sin(angle);
	g->player.world_camera_plane_y = old_plane_x * sin(angle)
		+ g->player.world_camera_plane_y * cos(angle);
}

void	handle_rotation_input(t_game *g, double ang)
{
	mlx_t	*mlx;

	if (!g || !g->graphics.mlx)
		return ;
	mlx = g->graphics.mlx;
	if (mlx_is_key_down(mlx, MLX_KEY_Q) || mlx_is_key_down(mlx, MLX_KEY_LEFT))
		rotate_player(g, -ang);
	if (mlx_is_key_down(mlx, MLX_KEY_E) || mlx_is_key_down(mlx, MLX_KEY_RIGHT))
		rotate_player(g, +ang);
}

double	get_delta_time(t_game *g)
{
	double	dt;

	if (!g)
		return (0.0);
	g->time_now = mlx_get_time();
	dt = g->time_now - g->time_prev;
	if (dt < 0.0)
		dt = 0.0;
	if (dt > 0.05)
		dt = 0.05;
	g->time_prev = g->time_now;
	return (dt);
}
