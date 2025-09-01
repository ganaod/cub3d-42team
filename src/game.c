/* ************************************************************************** */
/*                                                                            */
/*   game.c - Game Loop Execution & State Management                        */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

/*
UPSTREAM RESPONSIBILITY:
- Execute core game loop operations
- Coordinate game state updates
- Manage game timing and flow control
- Handle game running/exit conditions
*/

/*
CORE GAME LOOP PATTERN:
1. Process Input → Update Game State
2. Render Frame → Display Results  
3. Continue or Exit Decision

This is the heartbeat of the entire program.
*/
void run_game_loop(void)
{
    while (game_running())
    {
        // INPUT PHASE
        process_input_events();
        
        // UPDATE PHASE  
        update_player_state();
        
        // RENDER PHASE
        render_complete_frame();
        
        // DISPLAY PHASE
        present_frame_to_screen();
    }
}

/*
GAME STATE MANAGEMENT:
Initialize and manage global game state
*/
void init_game_state(void)
{
    // Initialize global game structure
    ft_memset(&g_game, 0, sizeof(t_game));
    
    // Set default values
    g_game.running = 1;
    g_game.movement_speed = 0.1;
    g_game.rotation_speed = 0.05;
    
    // Initialize player starting state
    init_player_defaults();
}

void init_player_defaults(void)
{
    // Default position (will be overridden by map parsing)
    g_game.player.pos_x = 5.0;
    g_game.player.pos_y = 5.0;
    
    // Default facing direction (North)
    g_game.player.dir_x = 0.0;
    g_game.player.dir_y = -1.0;
    
    // Initialize camera plane for FOV
    g_game.player.camera_plane_x = FOV;
    g_game.player.camera_plane_y = 0.0;
}

/*
GAME STATE QUERIES:
Check current game status
*/
int game_running(void)
{
    return (g_game.running);
}

void set_game_exit(void)
{
    g_game.running = 0;
}

/*
PLAYER STATE UPDATES:
Handle any per-frame player state updates
*/
void update_player_state(void)
{
    // Currently no per-frame updates needed
    // Future: animation states, physics integration, etc.
    
    // Validate player position stays within bounds
    validate_player_bounds();
}

void validate_player_bounds(void)
{
    // Ensure player hasn't gone out of map bounds
    if (g_game.player.pos_x < 0)
        g_game.player.pos_x = 0;
    if (g_game.player.pos_y < 0)
        g_game.player.pos_y = 0;
    if (g_game.player.pos_x >= g_game.map.width)
        g_game.player.pos_x = g_game.map.width - 1;
    if (g_game.player.pos_y >= g_game.map.height)
        g_game.player.pos_y = g_game.map.height - 1;
}