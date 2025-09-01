/* program entry & orchestration */

#include "cub3d.h"

/* 1. validate cmd line args
2. initialise sys
3. coordinate main game loop
4. manage graceful shutdown */

int main(int ac, char **av)
{
    if (ac < 2)
        error_exit("Usage: ./cub3D map.cub");
    if (!validate_file_extension(av[1], ".cub"))
        error_exit("Invalid map file extension");
    initialize_system(av[1]);
    run_game_loop();
    cleanup_all_resources();
    return (EXIT_SUCCESS);
}

/* initialise subsystems in dependency order */
void initialize_system(char *map_file)
{
    init_game_state();
    if (!load_map_config(map_file))
        error_exit("Failed to load map configuration");
    if (!init_graphics_system())
        error_exit("Graphics initialization failed");
    setup_input_handlers();
}
