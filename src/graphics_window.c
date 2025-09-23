# include "../inc/cub3d.h"

// window lifecycle: establish sys
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

// event handling (runtime): respond to external changes
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

// resolution policy
// performance vs quality decision logic
static void	choose_render_resolution(int window_w, int window_h, 
	int *render_w, int *render_h)
{
if (window_w > RENDER_WIDTH_THRESHOLD || window_h > RENDER_HEIGHT_THRESHOLD)
{
*render_w = INTERNAL_RENDER_WIDTH;
*render_h = INTERNAL_RENDER_HEIGHT;
}
else
{
*render_w = window_w;
*render_h = window_h;
}
}

// framebuffer implementation: execute decisions
int	gfx_rebuild_framebuffer(t_graphics *g, int window_w, int window_h)
{
mlx_image_t	*new_frame;
int			render_w;
int			render_h;

if (!g || !g->mlx || window_w <= 0 || window_h <= 0)
return (0);
if (g->frame)
{
mlx_delete_image(g->mlx, g->frame);
g->frame = NULL;
}
choose_render_resolution(window_w, window_h, &render_w, &render_h);
new_frame = mlx_new_image(g->mlx, (uint32_t)render_w, (uint32_t)render_h);
if (!new_frame)
return (0);
g->frame = new_frame;
g->screen_width = render_w;
g->screen_height = render_h;
return (1);
}
