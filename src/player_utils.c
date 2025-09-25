/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 09:32:59 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/25 15:49:19 by blohrer          ###   ########.fr       */
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

/* attempt to move P to new pos
while preventing walking through Ws
checks dist to nearest wall at proposed x & y coords
if both coords are clear of Ws, update P pos */
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

static int	mpr_prepare_buffers(const t_map *m, char **vis_void, int **q)
{
	int	total;
	int	i;

	total = m->width * m->height;
	*vis_void = (char *)malloc(total);
	*q = (int *)malloc(sizeof(int) * total);
	if (!*vis_void || !*q)
	{
		free(*vis_void);
		free(*q);
		return (0);
	}
	i = 0;
	while (i < total)
	{
		(*vis_void)[i] = 0;
		i++;
	}
	return (1);
}

int	map_is_closed_player_region(const t_map *m, const t_player *pl)
{
	char	*vis_void;
	int		*q;
	int		ok;

	if (!m || !pl || !m->grid || m->width <= 0 || m->height <= 0)
		return (0);
	vis_void = NULL;
	q = NULL;
	if (!mpr_prepare_buffers(m, &vis_void, &q))
		return (0);
	mark_outside_void_pure(m, vis_void, q);
	ok = player_region_is_closed(m, pl, vis_void);
	free(vis_void);
	free(q);
	return (ok);
}
