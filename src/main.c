/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 14:17:12 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/24 10:04:23 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

int	main(int argc, char **argv)
{
	t_game	g;

	g = (t_game){0};
	if (argc != 2)
		return (parse_error("usage: ./cub3d file.cub"), 2);
	if (!check_cub_extension(argv[1]))
		return (parse_error("map file must end with .cub"), 2);
	if (!map_load_and_validate(&g, argv[1]))
		return (cleanup_and_exit(&g, 3));
	init_runtime_defaults(&g);
	g_game = g;
	if (!start_window(&g_game, "cub3d", DEFAULT_WIDTH, DEFAULT_HEIGHT))
		return (cleanup_and_exit(&g_game, 8));
	if (!load_textures_or_fail(&g_game))
		return (cleanup_and_exit(&g_game, 9));
	g_game.running = 1;
	g_game.time_prev = mlx_get_time();
	mlx_close_hook(g_game.graphics.mlx, on_close, NULL);
	mlx_loop_hook(g_game.graphics.mlx, game_loop_tick, &g_game);
	mlx_loop(g_game.graphics.mlx);
	return (cleanup_and_exit(&g_game, 0));
}
