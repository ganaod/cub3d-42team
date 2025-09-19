/* program entry & orchestration */

#include "../inc/cub3d.h"

/* 1. validate cmd line args
2. initialise sys
3. coordinate main game loop
4. manage graceful shutdown */

// int main(void)
// {
//     printf("Hello cub3D\n");
//     return(0);
// }

// int main(int ac, char **av)
// {
//     if (ac < 2)
//         error_exit("Usage: ./cub3D map.cub");
//     if (!validate_file_extension(av[1], ".cub"))
//         error_exit("Invalid map file extension");
//     initialize_system(av[1]);
//     run_game_loop();
//     cleanup_all_resources();
//     return (EXIT_SUCCESS);
// }

/* initialise subsystems in dependency order */
// void initialize_system(char *map_file)
// {
//     init_game_state();
//     if (!load_map_config(map_file))
//         error_exit("Failed to load map configuration");
//     if (!init_graphics_system())
//         error_exit("Graphics initialization failed");
//     setup_input_handlers();
// }


/* ===== Draw test image: sky above, ground below ===== */
// static void draw_test_frame(t_game *g)
// {
//     int w = g->graphics.screen_width;
//     int h = g->graphics.screen_height;

//     uint32_t ceil_rgba = (g->map.ceiling_color << 8) | 0xFF; // 0xRRGGBBA
//     uint32_t floor_rgba = (g->map.floor_color   << 8) | 0xFF;

//     for (int y = 0; y < h; y++)
//     {
//         uint32_t color = (y < h / 2) ? ceil_rgba : floor_rgba;
//         for (int x = 0; x < w; x++)
//             mlx_put_pixel(g->graphics.frame, x, y, color);
//     }
// }

// /* ===== Player Default Init (einfacher Startwert) ===== */
// static void player_init_defaults(t_player *p)
// {
//     p->pos_x = 3.5;
//     p->pos_y = 3.5;
//     p->dir_x = -1.0;  // Blick nach "Westen"
//     p->dir_y = 0.0;
//     p->camera_plane_x = 0.0;
//     p->camera_plane_y = FOV; // bei klassischem Setup ~0.66
// }

// /* ===== Map/State Default Init (nur für den Test) ===== */
// static void map_init_defaults(t_map *m)
// {
//     m->grid = NULL;         // noch keine Map geladen
//     m->width = 0;
//     m->height = 0;
//     m->texture_paths[0] = NULL;
//     m->texture_paths[1] = NULL;
//     m->texture_paths[2] = NULL;
//     m->texture_paths[3] = NULL;

//     // Test colors: sky light blue, ground green
//     m->ceiling_color = 0x87CEEB; // 0xRRGGBB
//     m->floor_color   = 0x228B22; // 0xRRGGBB
// }

// int main(void)
// {
//     t_game g;

//     // Window size from your header
//     g.graphics.screen_width  = DEFAULT_WIDTH;
//     g.graphics.screen_height = DEFAULT_HEIGHT;

//     // MLX42 start
//     g.graphics.mlx = mlx_init(DEFAULT_WIDTH, DEFAULT_HEIGHT, "Cub3D", false);
//     if (!g.graphics.mlx)
//         return (EXIT_FAILURE);

//     // Create framebuffer
//     g.graphics.frame = mlx_new_image(g.graphics.mlx,
//                                      g.graphics.screen_width,
//                                      g.graphics.screen_height);
//     if (!g.graphics.frame)
//     {
//         mlx_terminate(g.graphics.mlx);
//         return (EXIT_FAILURE);
//     }

//     // Image in window
//     if (mlx_image_to_window(g.graphics.mlx, g.graphics.frame, 0, 0) < 0)
//     {
//         mlx_delete_image(g.graphics.mlx, g.graphics.frame);
//         mlx_terminate(g.graphics.mlx);
//         return (EXIT_FAILURE);
//     }

//     // Defaults for map/player so that draw_test_frame has colors
//     map_init_defaults(&g.map);
//     player_init_defaults(&g.player);

//     // Draw test image
//     draw_test_frame(&g);

//     // Eventloop
//     mlx_loop(g.graphics.mlx);

//     // Clean up
//     mlx_delete_image(g.graphics.mlx, g.graphics.frame);
//     mlx_terminate(g.graphics.mlx);
//     return (EXIT_SUCCESS);
// }

// int main(int argc, char **argv)
// {
//     t_map   m;
//     int     fd;
//     int     ok;
//     int     i;

//     if (argc != 2)
//     {
//         printf("Usage: %s <file.cub>\n", argv[0]);
//         return (1);
//     }

//     // Map-Struct initialisieren
//     for (i = 0; i < 4; i++)
//         m.texture_paths[i] = NULL;
//     m.floor_color = 0;
//     m.ceiling_color = 0;
//     m.first_map_line = NULL;

//     fd = open(argv[1], O_RDONLY);
//     if (fd < 0)
//     {
//         perror("open");
//         return (1);
//     }

//     ok = parse_header_lines(&m, fd);
//     close(fd);

//     if (!ok)
//     {
//         printf("❌ parse_header_lines failed\n");
//         return (1);
//     }

//     // Ergebnisse ausgeben
//     printf("✅ parse_header_lines succeeded\n");
//     printf("NO: %s\n", m.texture_paths[NORTH]);
//     printf("SO: %s\n", m.texture_paths[SOUTH]);
//     printf("WE: %s\n", m.texture_paths[WEST]);
//     printf("EA: %s\n", m.texture_paths[EAST]);
//     printf("F:  %#08x\n", m.floor_color);
//     printf("C:  %#08x\n", m.ceiling_color);
//     if (m.first_map_line)
//         printf("First map line: %s", m.first_map_line); // Zeile endet mit \n aus GNL

//     // Aufräumen
//     for (i = 0; i < 4; i++)
//         free(m.texture_paths[i]);
//     free(m.first_map_line);

//     return (0);
// }

// static void free_lines_array(char **lines, int h) {
// 	int i = 0;
// 	while (i < h) { free(lines[i]); i++; }
// 	free(lines);
// }

// int main(int argc, char **argv) {
// 	t_map m;
// 	int fd, h, i;
// 	char **lines;
// 	int w;

// 	if (argc != 2) { fprintf(stderr, "usage: %s file.cub\n", argv[0]); return 2; }
// 	fd = open(argv[1], O_RDONLY);
// 	if (fd < 0) { perror("open"); return 1; }

// 	m = (t_map){0};

// 	if (!parse_header_lines(&m, fd)) {
// 		fprintf(stderr, "❌ header parse failed\n");
// 		close(fd); return 3;
// 	}
// 	if (!collect_map_lines(&m, fd, &lines, &h)) {
// 		fprintf(stderr, "❌ collect_map_lines failed\n");
// 		close(fd); return 4;
// 	}
// 	close(fd);


// 	if (!normalize_map(&lines, h, &w)) {
// 		ft_printf("❌ normalize_map failed\n");
// 		return (EXIT_FAILURE);
// 	}
// 	ft_printf("✅ normalize_map: w=%d, h=%d\n", w, h);
// 	i = 0;
// 	while (i < h) {
// 		ft_printf("  [%d] |%s| (len=%d)\n", i, lines[i], (int)ft_strlen(lines[i]));
// 		i++;
// 	}

// 	printf("✅ collected %d map lines:\n", h);
// 	i = 0;
// 	while (i < h) {
// 		printf("  [%d] \"%s\"\n", i, lines[i]);
// 		i++;
// 	}

// 	free_lines_array(lines, h);
// 	return 0;
// }






// Returns 1 on success, 0 on error
static int load_one_texture_png(const char *path, t_texture_image *out)
{
    mlx_texture_t *t = mlx_load_png(path);
    if (!t) {
        ft_printf("ERROR: mlx_load_png failed for '%s'\n", path);
        return 0;
    }

    // t->pixels is RGBA8, width * height * 4 bytes
    out->image_width  = (int)t->width;
    out->image_height = (int)t->height;
    out->mlx_image    = NULL;

    size_t count = (size_t)out->image_width * (size_t)out->image_height;
    out->pixels = malloc(count * sizeof(uint32_t));
    if (!out->pixels) {
        mlx_delete_texture(t);
        ft_printf("ERROR: malloc texture pixels failed for '%s'\n", path);
        return 0;
    }

    // Pack RGBA bytes into a 32-bit pixel your renderer uses.
    // If your renderer expects ARGB, pack as 0xAARRGGBB (adjust if you use another order).
    for (size_t i = 0; i < count; ++i) {
        uint8_t r = t->pixels[i * 4 + 0];
        uint8_t g = t->pixels[i * 4 + 1];
        uint8_t b = t->pixels[i * 4 + 2];
        uint8_t a = t->pixels[i * 4 + 3];
        out->pixels[i] = (uint32_t)(a << 24 | r << 16 | g << 8 | b); // ARGB
    }

    mlx_delete_texture(t);
    return 1;
}






static void free_lines_array(char **lines, int h) {
    int i = 0;
    while (i < h) { free(lines[i]); i++; }
    free(lines);
}








static int init_window_and_frame(t_game *g, int w, int h, const char *title)
{
    g->graphics.screen_width = w;
    g->graphics.screen_height = h;
    
    g->graphics.mlx = mlx_init(w, h, title, false);
    if (!g->graphics.mlx)
        return (0);
    
    g->graphics.frame = mlx_new_image(g->graphics.mlx, w, h);
    if (!g->graphics.frame)
        return (0);
    
    return (1);
}





static void sync_player_world_fields_from_parser(t_game *g)
{
    // Player position and direction already set by parser - don't overwrite!
    // Just ensure camera plane is perpendicular to direction with correct FOV
    
    // Camera plane = perpendicular to direction, scaled by FOV
    g->player.world_camera_plane_x = -g->player.world_dir_y * FOV_CAMERA_PLANE_MAGNITUDE;
    g->player.world_camera_plane_y = g->player.world_dir_x * FOV_CAMERA_PLANE_MAGNITUDE;
}






static void free_map(t_map *m)
{
    if (!m) return;

    // 1D-Grid freigeben
    free(m->grid);
    m->grid = NULL;

    // Texturpfade (falls via strdup o.ä.)
    for (int i = 0; i < 4; ++i) {
        free(m->texture_paths[i]);
        m->texture_paths[i] = NULL;
    }

    // Falls du echte MLX-Images in wall_textures hast:
    for (int i = 0; i < 4; ++i) {
        if (m->wall_textures[i].mlx_image) {
            // Hinweis: g_game.graphics.mlx muss noch gültig sein
            mlx_delete_image(g_game.graphics.mlx, m->wall_textures[i].mlx_image);
            m->wall_textures[i].mlx_image = NULL;
        }
        free(m->wall_textures[i].pixels);
        m->wall_textures[i].pixels = NULL;      // wird von MLX verwaltet, nicht separat freigeben
        m->wall_textures[i].image_width = 0;
        m->wall_textures[i].image_height = 0;
    }
}





static int load_all_wall_textures(t_map *m)
{
    // Validate paths parsed from header
    for (int i = 0; i < 4; ++i) {
        if (!m->texture_paths[i] || !*m->texture_paths[i]) {
            ft_printf("ERROR: missing texture path for face %d\n", i);
            return 0;
        }
    }

    // Load each face
    if (!load_one_texture_png(m->texture_paths[WALL_NORTH], &m->wall_textures[WALL_NORTH])) return 0;
    if (!load_one_texture_png(m->texture_paths[WALL_SOUTH], &m->wall_textures[WALL_SOUTH])) return 0;
    if (!load_one_texture_png(m->texture_paths[WALL_WEST],  &m->wall_textures[WALL_WEST]))  return 0;
    if (!load_one_texture_png(m->texture_paths[WALL_EAST],  &m->wall_textures[WALL_EAST]))  return 0;

    return 1;
}






int main(int argc, char **argv)
{
    t_game  g = (t_game){0};
    char  **lines = NULL;
    int     h = 0;
    int     fd;
    int     player_found = 0;

    if (argc != 2) 
	{
        parse_error("usage: ./cub3d file.cub");
        return (2);
    }

    fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        parse_error("open failed");
        return (1);
    }

    /* 1) Header parsen */
    if (!parse_header_lines(&g.map, fd)) {
        parse_error("header parse failed");
        close(fd);
        free_map(&g.map);
        return (3);
    }

    /* 2) Map-Zeilen sammeln + normalisieren */
    if (!collect_map_lines(&g.map, fd, &lines, &h)) {
        parse_error("collect_map_lines failed");
        close(fd);
        free_map(&g.map);
        return (4);
    }
    close(fd);

    if (!normalize_map(&lines, h, &g.map.width)) {
        parse_error("normalize_map failed");
        free_lines_array(lines, h);
        free_map(&g.map);
        return (5);
    }

    /* 3) Grid bauen + Closed-Check */
    if (!build_grid_from_lines(&g.map, &g.player, lines, &player_found)
        || player_found != 1)
    {
        parse_error("grid build failed (player count)");
        free_lines_array(lines, h);
        free_map(&g.map);
        return (6);
    }
    if (!map_is_closed(&g.map)) {
        parse_error("map is open (leaks into VOID)");
        free_lines_array(lines, h);
        free_map(&g.map);
        return (7);
    }
    free_lines_array(lines, h);

    /* 4) Grafik initialisieren (Fenster + Framebuffer) */
    if (!init_window_and_frame(&g, DEFAULT_WIDTH, DEFAULT_HEIGHT, "cub3d")) {
        parse_error("mlx init failed");
        free_map(&g.map);
        return (8);
    }

    /* 5) Texturen laden */
    if (!load_all_wall_textures(&g.map)) {
        parse_error("failed to load wall textures");
        mlx_terminate(g.graphics.mlx);
        free_map(&g.map);
        return (9);
    }

    /* 6) Player-Werte -> world_* Felder spiegeln (für Raycaster) */
    g.movement_speed = 3.0;   // Einheiten pro Sekunde
    g.rotation_speed = 2.0;   // Radiant pro Sekunde
    sync_player_world_fields_from_parser(&g);

    /* 7) Frame-Loop */
    g.running = 1;
    g_game = g; // global setzen, alle Render-Files nutzen g_game

    // Delta-Time initialisieren
    g_game.time_prev = mlx_get_time();

    // Bild ins Fenster setzen
    mlx_image_to_window(g_game.graphics.mlx, g_game.graphics.frame, 0, 0);

    // Hooks registrieren
    mlx_close_hook(g_game.graphics.mlx, on_close, NULL);
    mlx_loop_hook(g_game.graphics.mlx, game_loop_tick, NULL);

    // Hauptloop starten
    mlx_loop(g_game.graphics.mlx);

    /* 8) Cleanup */
    mlx_terminate(g_game.graphics.mlx);
    free_map(&g_game.map);
    return (0);
}
