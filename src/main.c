/* program entry & orchestration */

#include "../inc/cub3d.h"

static void sync_player_world_fields_from_parser(t_game *g)
{
    // Player position and direction already set by parser - don't overwrite!
    // Just ensure camera plane is perpendicular to direction with correct FOV

    // Camera plane = perpendicular to direction, scaled by FOV
    g->player.world_camera_plane_x = -g->player.world_dir_y * FOV_CAMERA_PLANE_MAGNITUDE;
    g->player.world_camera_plane_y = g->player.world_dir_x * FOV_CAMERA_PLANE_MAGNITUDE;
}

static int open_rdonly(const char *path)
{
    int fd = open(path, O_RDONLY);
    if (fd < 0)
        parse_error("open failed");
    return fd;
}
static int map_load_and_validate(t_game *g, const char *path)
{
    char **lines = NULL;
    int    h = 0;
    int  player_found = 0;
    int    fd;

    if (!g || !path)
        return 0;

    fd = open_rdonly(path);
    if (fd < 0)
        return 0;

    if (!parse_header_lines(&g->map, fd)) { parse_error("header parse failed"); close(fd); return 0; }
    if (!collect_map_lines(&g->map, fd, &lines, &h)) { parse_error("collect_map_lines failed"); close(fd); free_map(&g->map); return 0; }
    close(fd);

    if (!normalize_map(&lines, h, &g->map.width)) { parse_error("normalize_map failed"); free_lines_array(lines, h); free_map(&g->map); return 0; }
    if (!build_grid_from_lines(&g->map, &g->player, lines, &player_found) || player_found != 1)
    { parse_error("grid build failed (player count)"); free_lines_array(lines, h); free_map(&g->map); return 0; }
    if (!map_is_closed(&g->map))
    { parse_error("map is open (leaks into VOID)"); free_lines_array(lines, h); free_map(&g->map); return 0; }

    free_lines_array(lines, h);
    return 1;
}

static int start_window(t_game *g, const char *title, int w, int h)
{
    if (!g || !title) return (0);
    if (!gfx_open_window_resizable(g, title, w, h)) return (0);
    // genau EINMAL ins Fenster hängen (hier oder innerhalb deiner gfx_open_*)
    mlx_image_to_window(g->graphics.mlx, g->graphics.frame, 0, 0);
    // Resize-Hook mit t_game* (falls nicht in gfx_open_* erledigt)
    mlx_resize_hook(g->graphics.mlx, handle_resize, g);
    return (1);
}

static int load_textures_or_fail(t_game *g)
{
    if (!g) return (0);
    if (!load_all_wall_textures(&g->map)) { parse_error("failed to load wall textures"); return (0); }
    return (1);
}

static void init_runtime_defaults(t_game *g)
{
    if (!g) return;
    g->movement_speed = 3.0;
    g->rotation_speed = 2.0;
    sync_player_world_fields_from_parser(g);
}

int main(int argc, char **argv)
{
    t_game g = (t_game){0};

    if (argc != 2) { parse_error("usage: ./cub3d file.cub"); return 2; }
    if (!map_load_and_validate(&g, argv[1])) return cleanup_and_exit(&g, 3);

    init_runtime_defaults(&g);
    g_game = g;

    if (!start_window(&g_game, "cub3d", DEFAULT_WIDTH, DEFAULT_HEIGHT))
        return cleanup_and_exit(&g_game, 8);
    if (!load_textures_or_fail(&g_game))
        return cleanup_and_exit(&g_game, 9);

    g_game.running = 1;
    g_game.time_prev = mlx_get_time();
    mlx_close_hook(g_game.graphics.mlx, on_close, NULL);
    mlx_loop_hook(g_game.graphics.mlx, game_loop_tick, &g_game);
    mlx_loop(g_game.graphics.mlx);

    return cleanup_and_exit(&g_game, 0);
}
