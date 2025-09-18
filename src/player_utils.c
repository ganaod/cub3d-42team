/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 10:17:40 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/18 10:28:06 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static int	is_walkable(const t_map *m, double x, double y)
{
	int	ix;
	int	iy;
	int	c;

	ix = (int)x;
	iy = (int)y;
	// nur leere Zellen sind begehbar; VOID und WALL nicht
	c = map_cell(m, ix, iy);
	return (c == CELL_EMPTY);
}

void	try_move_player(double new_x, double new_y)
{
	double	cur_y;

	// kleiner Radius, damit man nicht in Ecken hängen bleibt
	// (hier einfache Variante: Achsen getrennt testen)
	// double cur_x = g_game.player.world_pos_x;
	cur_y = g_game.player.world_pos_y;
	// X separat
	if (is_walkable(&g_game.map, new_x, cur_y))
		g_game.player.world_pos_x = new_x;
	// Y separat
	if (is_walkable(&g_game.map, g_game.player.world_pos_x, new_y))
		g_game.player.world_pos_y = new_y;
}

void	rotate_player(double angle)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = g_game.player.world_dir_x;
	old_plane_x = g_game.player.world_camera_plane_x;
	// Richtung drehen
	g_game.player.world_dir_x = old_dir_x * cos(angle)
		- g_game.player.world_dir_y * sin(angle);
	g_game.player.world_dir_y = old_dir_x * sin(angle)
		+ g_game.player.world_dir_y * cos(angle);
	// Kamera-Ebene drehen (orthogonal zur Blickrichtung)
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

	// aktuelle Zeit abrufen
	g_game.time_now = mlx_get_time();
	// Differenz zum letzten Frame
	dt = g_game.time_now - g_game.time_prev;
	// für Stabilität clampen (z. B. beim Breakpoint)
	if (dt < 0.0)
		dt = 0.0;
	if (dt > 0.05)
		dt = 0.05; // max ~50 ms
	// "letzte Zeit" aktualisieren
	g_game.time_prev = g_game.time_now;
	return (dt);
}
