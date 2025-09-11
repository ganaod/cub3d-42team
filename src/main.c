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

static void free_lines_array(char **lines, int h) {
    int i = 0;
    while (i < h) { free(lines[i]); i++; }
    free(lines);
}

static char cell_ch(int v) {
    if (v == CELL_WALL)  return '#';
    if (v == CELL_EMPTY) return '.';
    if (v == CELL_VOID)  return ' ';
    return '?';
}

int main(int argc, char **argv) {
    t_game g = (t_game){0};
    char **lines;
    int   h;
    int   fd;
    int   player_found;
    int   y;
    int   x;
    int   ok;

    if (argc != 2) { fprintf(stderr, "usage: %s file.cub\n", argv[0]); return 2; }
    fd = open(argv[1], O_RDONLY);
    if (fd < 0) { perror("open"); return 1; }

    if (!parse_header_lines(&g.map, fd)) { fprintf(stderr, "❌ header parse failed\n"); close(fd); return 3; }
    if (!collect_map_lines(&g.map, fd, &lines, &h)) { fprintf(stderr, "❌ collect_map_lines failed\n"); close(fd); return 4; }
    close(fd);

    /* normalize_map passt lines rechteckig an (Spaces rechts) */
    if (!normalize_map(&lines, h, &g.map.width)) {
        fprintf(stderr, "❌ normalize_map failed\n");
        free_lines_array(lines, h);
        return 5;
    }

    /* Grid bauen (ermittelt width/height intern neu & setzt Player) */
    player_found = 0;
    if (!build_grid_from_lines(&g.map, &g.player, lines, &player_found)) {
        fprintf(stderr, "❌ build_grid_from_lines failed (player_found=%d)\n", player_found);
        free_lines_array(lines, h);
        return 6;
    }

    /* 👉 NEU: Map-Closed-Check (Flood-Fill über CELL_VOID vom Rand) */
    ok = map_is_closed(&g.map);
    if (!ok) {
        fprintf(stderr, "❌ map_is_closed: map is OPEN (leckt ins VOID)\n");
        free_lines_array(lines, h);
        free(g.map.grid);
        return 7;
    }
    printf("✅ map_is_closed: map is CLOSED\n");

    /* Ausgabe */
    printf("✅ grid built: w=%d h=%d player_found=%d\n", g.map.width, g.map.height, player_found);
    printf("   player: pos=(%.2f, %.2f) dir=(%.2f, %.2f) plane=(%.2f, %.2f)\n",
           g.player.world_pos_x, g.player.world_pos_y, g.player.world_dir_x, g.player.world_dir_y,
           g.player.world_camera_plane_x, g.player.world_camera_plane_y);

    y = 0;
    while (y < g.map.height) {
        x = 0;
        while (x < g.map.width) {
            putchar(cell_ch(g.map.grid[y * g.map.width + x]));
            x++;
        }
        putchar('\n');
        y++;
    }

    /* Aufräumen */
    free_lines_array(lines, h);
    free(g.map.grid);
    return 0;
}
