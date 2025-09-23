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


int gfx_open_window(t_graphics *g, const char *title, int w, int h)
{
    if (!g || !title || w <= 0 || h <= 0)
        return (0);
    g->mlx = mlx_init((int32_t)w, (int32_t)h, title, false);
    if (!g->mlx)
        return (0);
    g->screen_width = w;
    g->screen_height = h;
    if (!gfx_rebuild_framebuffer(g, g->screen_width, g->screen_height))
        return (0);
    if (mlx_image_to_window(g->mlx, g->frame, 0, 0) < 0)
        return (0);
    mlx_resize_hook(g->mlx, handle_resize, g);
    return (1);
}

int gfx_open_window_default(t_graphics *g, const char *title)
{
    return gfx_open_window(g, title, DEFAULT_WIDTH, DEFAULT_HEIGHT);
}

int gfx_open_window_resizable(t_game *game, const char *title, int w, int h)
{
    t_graphics *g;

    if (!game || !title || w <= 0 || h <= 0)
        return (0);

    g = &game->graphics;

    // MLX initialisieren (resizable = true)
    g->mlx = mlx_init((int32_t)w, (int32_t)h, title, true);
    if (!g->mlx)
        return (0);

    g->screen_width = w;
    g->screen_height = h;

    // Framebuffer erstellen
    if (!gfx_rebuild_framebuffer(g, w, h))
    {
        mlx_terminate(g->mlx);
        return (0);
    }

    // Image zur MLX Instanz hinzufügen
    if (mlx_image_to_window(g->mlx, g->frame, 0, 0) < 0)
    {
        mlx_delete_image(g->mlx, g->frame);
        mlx_terminate(g->mlx);
        return (0);
    }

    // Resize hook registrieren
    mlx_resize_hook(g->mlx, handle_resize, game);

    return (1);
}

int gfx_maximize_now(t_graphics *g)
{
    int32_t mw, mh;

    if (!g || !g->mlx)
        return (0);

    mlx_get_monitor_size(0, &mw, &mh);
    mlx_set_window_size(g->mlx, mw, mh);

    // Screen-Dimensionen aktualisieren wird durch resize callback gemacht
    // g->screen_width = (int)mw;
    // g->screen_height = (int)mh;

    return (1);
}

#define MAX_RENDER_WIDTH    1600
#define MAX_RENDER_HEIGHT   900

void handle_resize(int32_t w, int32_t h, void *param)
{
    t_game *game;

    game = (t_game *)param;
    if (!game || !game->graphics.mlx || w <= 0 || h <= 0)
        return;

    printf("Window resized to: %dx%d\n", w, h);

    if (!gfx_rebuild_framebuffer(&game->graphics, (int)w, (int)h))
    {
        return;
    }

    if (game->graphics.frame)
    {
        if (mlx_image_to_window(game->graphics.mlx, game->graphics.frame, 0, 0) < 0)
        {
            return;
        }
    }
}

