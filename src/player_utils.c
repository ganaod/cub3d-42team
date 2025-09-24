/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 10:17:40 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/24 13:27:56 by go-donne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static int	is_walkable(const t_map *m, double x, double y)
{
	int	ix = (int)x;
	int	iy = (int)y;
	
	// Basic collision - must be in empty cell
	if (map_cell(m, ix, iy) != CELL_EMPTY)
		return (0);
		
	// Check distance to nearest walls (prevent extreme proximity)
	const double MIN_WALL_DIST = 0.1;
	
	// Check 4 neighboring cells for walls
	if (map_cell(m, ix + 1, iy) == CELL_WALL && (x - ix) > (1.0 - MIN_WALL_DIST))
		return (0);  // Too close to east wall
	if (map_cell(m, ix - 1, iy) == CELL_WALL && (x - ix) < MIN_WALL_DIST)
		return (0);  // Too close to west wall  
	if (map_cell(m, ix, iy + 1) == CELL_WALL && (y - iy) > (1.0 - MIN_WALL_DIST))
		return (0);  // Too close to south wall
	if (map_cell(m, ix, iy - 1) == CELL_WALL && (y - iy) < MIN_WALL_DIST)
		return (0);  // Too close to north wall
		
	return (1);
}

void	try_move_player(double new_x, double new_y)
{
	double	cur_y = g_game.player.world_pos_y;
	
	if (is_walkable(&g_game.map, new_x, cur_y))
		g_game.player.world_pos_x = new_x;
	if (is_walkable(&g_game.map, g_game.player.world_pos_x, new_y))
		g_game.player.world_pos_y = new_y;
}

void	rotate_player(double angle)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = g_game.player.world_dir_x;
	old_plane_x = g_game.player.world_camera_plane_x;
	g_game.player.world_dir_x = old_dir_x * cos(angle)
		- g_game.player.world_dir_y * sin(angle);
	g_game.player.world_dir_y = old_dir_x * sin(angle)
		+ g_game.player.world_dir_y * cos(angle);
	g_game.player.world_camera_plane_x = old_plane_x * cos(angle)
		- g_game.player.world_camera_plane_y * sin(angle);
	g_game.player.world_camera_plane_y = old_plane_x * sin(angle)
		+ g_game.player.world_camera_plane_y * cos(angle);
}

void	handle_rotation_input(double ang)
{
	mlx_t	*mlx;

	mlx = g_game.graphics.mlx;
	if (mlx_is_key_down(mlx, MLX_KEY_Q) || mlx_is_key_down(mlx, MLX_KEY_LEFT))
		rotate_player(-ang);
	if (mlx_is_key_down(mlx, MLX_KEY_E) || mlx_is_key_down(mlx, MLX_KEY_RIGHT))
		rotate_player(+ang);
}

double	get_delta_time(void)
{
	double	dt;

	g_game.time_now = mlx_get_time();
	dt = g_game.time_now - g_game.time_prev;
	if (dt < 0.0)
		dt = 0.0;
	if (dt > 0.05)
		dt = 0.05;
	g_game.time_prev = g_game.time_now;
	return (dt);
}
