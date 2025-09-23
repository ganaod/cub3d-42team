#include "../inc/cub3d.h"

void	parse_error(const char *msg)
{
	write(2, "Error: ", 7);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
}

void	handle_exit_input(void)
{
	mlx_t	*mlx;

	mlx = g_game.graphics.mlx;
	if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE))
		mlx_close_window(mlx);
}

void	on_close(void *param)
{
	(void)param;
	g_game.running = 0;
	mlx_close_window(g_game.graphics.mlx);
}
