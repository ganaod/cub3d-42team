/* system initialisation & graphics setup

Builds system from ground up
Establishes MLX interface
Sets up user interaction
Prepares graphics pipeline */

#include "../inc/cub3d.h"

/*
UPSTREAM RESPONSIBILITY:
- Graphics system initialization and setup
- MLX library interface management
- Event handler registration and coordination
- System resource allocation
*/

/*
GRAPHICS SYSTEM INITIALIZATION:
Setup MLX graphics subsystem components
*/
int init_graphics_system(void)
{
    // Initialize MLX library
    g_game.graphics.mlx_ptr = mlx_init();
    if (!g_game.graphics.mlx_ptr)
        return (0);
    
    // Set screen dimensions
    g_game.graphics.screen_width = DEFAULT_WIDTH;
    g_game.graphics.screen_height = DEFAULT_HEIGHT;
    
    // Create window
    g_game.graphics.window_ptr = mlx_new_window(
        g_game.graphics.mlx_ptr,
        g_game.graphics.screen_width,
        g_game.graphics.screen_height,
        "cub3D"
    );
    if (!g_game.graphics.window_ptr)
        return (0);
    
    // Create image buffer
    g_game.graphics.image_ptr = mlx_new_image(
        g_game.graphics.mlx_ptr,
        g_game.graphics.screen_width,
        g_game.graphics.screen_height
    );
    if (!g_game.graphics.image_ptr)
        return (0);
    
    // Get image data pointer
    g_game.graphics.image_data = mlx_get_data_addr(
        g_game.graphics.image_ptr,
        &g_game.graphics.bits_per_pixel,
        &g_game.graphics.line_length,
        &g_game.graphics.endian
    );
    
    return (1);
}

/* INPUT SYSTEM SETUP:
Register MLX event handlers for user interaction */
void setup_input_handlers(void)
{
    // Register key press handler
    mlx_key_hook(g_game.graphics.window_ptr, key_press_handler, NULL);
    
    // Register window close handler  
    mlx_hook(g_game.graphics.window_ptr, 17, 0, window_close_handler, NULL);
    
    // Register main loop handler
    mlx_loop_hook(g_game.graphics.mlx_ptr, main_loop_handler, NULL);
}

/*
MLX EVENT HANDLERS:
Interface between MLX events and game logic
*/
int key_press_handler(int keycode, void *param)
{
    (void)param;
    
    if (keycode == KEY_ESC)
        set_game_exit();
    
    return (0);
}

int window_close_handler(void *param)
{
    (void)param;
    set_game_exit();
    return (0);
}

int main_loop_handler(void *param)
{
    (void)param;
    
    // This will be called continuously by MLX
    // For now, just check if we should exit
    if (!game_running())
        mlx_loop_end(g_game.graphics.mlx_ptr);
    
    return (0);
}