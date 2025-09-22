/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 13:08:21 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/22 09:42:44 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"
#include "../inc/render.h"

int	map_cell(const t_map *m, int ix, int iy)
{
	if (!m || !m->grid)
		return (CELL_WALL);
	if (ix < 0 || iy < 0 || ix >= m->width || iy >= m->height)
		return (CELL_WALL);
	return (m->grid[iy * m->width + ix]);
}

static void	handle_move_ws(double step)
{
	double	nx;
	double	ny;

	if (mlx_is_key_down(g_game.graphics.mlx, MLX_KEY_W))
	{
		nx = g_game.player.world_pos_x + g_game.player.world_dir_x * step;
		ny = g_game.player.world_pos_y + g_game.player.world_dir_y * step;
		try_move_player(nx, ny);
	}
	if (mlx_is_key_down(g_game.graphics.mlx, MLX_KEY_S))
	{
		nx = g_game.player.world_pos_x - g_game.player.world_dir_x * step;
		ny = g_game.player.world_pos_y - g_game.player.world_dir_y * step;
		try_move_player(nx, ny);
	}
}

static void	handle_strafe_ad(double step)
{
	double	nx;
	double	ny;

	if (mlx_is_key_down(g_game.graphics.mlx, MLX_KEY_A))
	{
		nx = g_game.player.world_pos_x - g_game.player.world_camera_plane_x
			* step;
		ny = g_game.player.world_pos_y - g_game.player.world_camera_plane_y
			* step;
		try_move_player(nx, ny);
	}
	if (mlx_is_key_down(g_game.graphics.mlx, MLX_KEY_D))
	{
		nx = g_game.player.world_pos_x + g_game.player.world_camera_plane_x
			* step;
		ny = g_game.player.world_pos_y + g_game.player.world_camera_plane_y
			* step;
		try_move_player(nx, ny);
	}
}

void	handle_input(double dt)
{
	double	step;
	double	ang;

	step = g_game.movement_speed * dt;
	ang = g_game.rotation_speed * dt;
	handle_exit_input();
	handle_move_ws(step);
	handle_strafe_ad(step);
	handle_rotation_input(ang);
}

void	game_loop_tick(void *param)
{
	double	dt;

	(void)param;
	if (!g_game.running)
		return ;
	dt = get_delta_time();
	handle_input(dt);
	render_complete_frame();
}
