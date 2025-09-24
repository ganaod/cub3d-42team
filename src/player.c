/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 13:08:21 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/24 14:32:07 by blohrer          ###   ########.fr       */
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

static void	handle_move_ws(t_game *g, double step)
{
	double	nx;
	double	ny;

	if (mlx_is_key_down(g->graphics.mlx, MLX_KEY_W))
	{
		nx = g->player.world_pos_x + g->player.world_dir_x * step;
		ny = g->player.world_pos_y + g->player.world_dir_y * step;
		try_move_player(g, nx, ny);
	}
	if (mlx_is_key_down(g->graphics.mlx, MLX_KEY_S))
	{
		nx = g->player.world_pos_x - g->player.world_dir_x * step;
		ny = g->player.world_pos_y - g->player.world_dir_y * step;
		try_move_player(g, nx, ny);
	}
}

static void	handle_strafe_ad(t_game *g, double step)
{
	double	nx;
	double	ny;

	if (mlx_is_key_down(g->graphics.mlx, MLX_KEY_A))
	{
		nx = g->player.world_pos_x - g->player.world_camera_plane_x * step;
		ny = g->player.world_pos_y - g->player.world_camera_plane_y * step;
		try_move_player(g, nx, ny);
	}
	if (mlx_is_key_down(g->graphics.mlx, MLX_KEY_D))
	{
		nx = g->player.world_pos_x + g->player.world_camera_plane_x * step;
		ny = g->player.world_pos_y + g->player.world_camera_plane_y * step;
		try_move_player(g, nx, ny);
	}
}

void	handle_input(t_game *g, double dt)
{
	double	step;
	double	ang;

	step = g->movement_speed * dt;
	ang = g->rotation_speed * dt;
	handle_exit_input(g);
	handle_move_ws(g, step);
	handle_strafe_ad(g, step);
	handle_rotation_input(g, ang);
}

void	game_loop_tick(void *param)
{
	t_game	*g;
	double	dt;

	g = (t_game *)param;
	if (!g || !g->running)
		return ;
	dt = get_delta_time(g);
	handle_input(g, dt);
	render_complete_frame(g);
}
