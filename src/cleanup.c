/* error handling & resource cleanup

Handles program death gracefully
Prevents memory leaks
Provides clear error messages
Coordinates shutdown sequence */

#include "../inc/cub3d.h"

/*
UPSTREAM RESPONSIBILITY:
- Program termination and error reporting
- Resource deallocation coordination  
- Memory cleanup and leak prevention
- Graceful shutdown procedures
*/

/*
ERROR HANDLING STRATEGY:
- Immediate exit on critical failures
- Resource cleanup before exit
- Clear error messages to user
*/
void error_exit(char *message)
{
    write(STDERR_FILENO, "Error: ", 7);
    write(STDERR_FILENO, message, ft_strlen(message));
    write(STDERR_FILENO, "\n", 1);
    cleanup_all_resources();
    exit(EXIT_FAILURE);
}

/*
PROGRAM LIFECYCLE MANAGEMENT:
- Track initialization status
- Coordinate subsystem dependencies
- Ensure proper cleanup order
*/
void cleanup_all_resources(void)
{
    // Cleanup in reverse order of initialization
    cleanup_graphics_system();
    cleanup_map_data();
    cleanup_game_state();
}

/*
GRAPHICS SYSTEM CLEANUP:
Release all MLX resources and handles
*/
void cleanup_graphics_system(void)
{
    if (g_game.graphics.image_ptr)
    {
        mlx_destroy_image(g_game.graphics.mlx_ptr, g_game.graphics.image_ptr);
        g_game.graphics.image_ptr = NULL;
    }
    
    if (g_game.graphics.window_ptr)
    {
        mlx_destroy_window(g_game.graphics.mlx_ptr, g_game.graphics.window_ptr);
        g_game.graphics.window_ptr = NULL;
    }
    
    if (g_game.graphics.mlx_ptr)
    {
        mlx_destroy_display(g_game.graphics.mlx_ptr);
        free(g_game.graphics.mlx_ptr);
        g_game.graphics.mlx_ptr = NULL;
    }
}

/*
MAP DATA CLEANUP:
Free dynamically allocated map resources
*/
void cleanup_map_data(void)
{
    int i;
    
    // Free texture paths
    i = 0;
    while (i < 4)
    {
        if (g_game.map.texture_paths[i])
        {
            free(g_game.map.texture_paths[i]);
            g_game.map.texture_paths[i] = NULL;
        }
        i++;
    }
    
    // Free map grid
    if (g_game.map.grid)
    {
        free(g_game.map.grid);
        g_game.map.grid = NULL;
    }
}

/*
GAME STATE CLEANUP:
Reset game state to clean condition
*/
void cleanup_game_state(void)
{
    // Reset game state structure
    ft_memset(&g_game, 0, sizeof(t_game));
}