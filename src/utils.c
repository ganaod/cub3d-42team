/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 09:55:16 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/24 13:26:09 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void	parse_error(const char *msg)
{
	write(2, "Error: ", 7);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
}

void	handle_exit_input(t_game *g)
{
	mlx_t	*mlx;

	if (!g || !g->graphics.mlx)
		return ;
	mlx = g->graphics.mlx;
	if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE))
		mlx_close_window(mlx);
}

void	on_close(void *param)
{
	t_game	*g;

	g = (t_game *)param;
	if (!g || !g->graphics.mlx)
		return ;
	g->running = 0;
	mlx_close_window(g->graphics.mlx);
}

int	check_cub_extension(const char *filename)
{
	int	len;

	if (!filename)
		return (0);
	len = 0;
	while (filename[len] != '\0')
		len++;
	if (len < 4)
		return (0);
	if (filename[len - 1] != 'b' || filename[len - 2] != 'u' || filename[len
			- 3] != 'c' || filename[len - 4] != '.')
		return (0);
	return (1);
}
