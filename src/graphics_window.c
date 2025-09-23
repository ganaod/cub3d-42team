#include "../inc/cub3d.h"

/* WINDOW LIFECYCLE - SYSTEM ESTABLISHMENT
Initialize MLX context, create resizable window, establish framebuffer
Sets up complete graphics pipeline with event handling hooks
Returns window ready for rendering operations */
int	gfx_open_window_resizable(t_game *game, const char *title, int w, int h)
{
	t_graphics	*g;

	if (!game || !title || w <= 0 || h <= 0)
		return (0);
	g = &game->graphics;
	g->mlx = mlx_init((int32_t)w, (int32_t)h, title, true);
	if (!g->mlx)
		return (0);
	g->screen_width = w;
	g->screen_height = h;
	if (!gfx_rebuild_framebuffer(g, w, h))
	{
		mlx_terminate(g->mlx);
		return (0);
	}
	if (mlx_image_to_window(g->mlx, g->frame, 0, 0) < 0)
	{
		mlx_delete_image(g->mlx, g->frame);
		mlx_terminate(g->mlx);
		return (0);
	}
	mlx_resize_hook(g->mlx, handle_resize, game);
	return (1);
}

/* WINDOW STATE CONTROL
Programmatically maximize window to full monitor dimensions
Triggers resize callback which handles framebuffer reconstruction */
int	gfx_maximize_now(t_graphics *g)
{
	int32_t	mw;
	int32_t	mh;

	if (!g || !g->mlx)
		return (0);
	mlx_get_monitor_size(0, &mw, &mh);
	mlx_set_window_size(g->mlx, mw, mh);
	return (1);
}

/* EVENT HANDLING - RUNTIME RESPONSE
Window resize callback triggered by MLX system events
Reconstructs framebuffer at appropriate resolution for new window size */
void	handle_resize(int32_t w, int32_t h, void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (!game || !game->graphics.mlx || w <= 0 || h <= 0)
		return ;
	if (!gfx_rebuild_framebuffer(&game->graphics, (int)w, (int)h))
		return ;
	if (game->graphics.frame)
	{
		int32_t center_x = (w - game->graphics.screen_width) / 2;
		int32_t center_y = (h - game->graphics.screen_height) / 2;
		if (mlx_image_to_window(game->graphics.mlx, game->graphics.frame, 
			center_x, center_y) < 0)
				return;
	}
}

/* RESOLUTION POLICY - PERFORMANCE DECISION
Choose optimal render resolution based on window dimensions
Large windows ( > w & h threshholds)
use internal scaling to maintain performance 
e.g. behaviour: 
	Window 2560x1440 → Render 1280x800  (internal scaling) 
constant workload */
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

/* FRAMEBUFFER IMPLEMENTATION - DECISION EXECUTION
Destroy existing framebuffer and create new one at chosen resolution
Updates screen dimensions that drive rendering pipeline column count */
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
