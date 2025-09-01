/* ************************************************************************** */
/*                                                                            */
/*   map.c - Configuration Parsing & Map Data Loading                       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

/*
UPSTREAM RESPONSIBILITY:
- Parse .cub configuration file format
- Extract and validate all game configuration
- Build playable map data structure
- Locate player starting position and orientation
*/

/*
CONFIGURATION FILE FORMAT:
R 1920 1080          // Resolution
NO ./path/north.xpm  // North wall texture  
SO ./path/south.xpm  // South wall texture
WE ./path/west.xpm   // West wall texture
EA ./path/east.xpm   // East wall texture
F 220,100,0          // Floor color (RGB)
C 225,30,0           // Ceiling color (RGB)

1111111111111        // Map data (1=wall, 0=space, N/S/E/W=player)
1000000000001
1000N00000001
1111111111111
*/

int load_map_config(char *filename)
{
    // PHASE 1: FILE ACCESS
    int fd = open_file_safely(filename);
    if (fd < 0)
        return (0);
    
    // PHASE 2: PARSE CONFIGURATION PARAMETERS
    /*
    Parse each line and extract:
    - Screen resolution settings
    - Texture file paths for each wall direction
    - Floor and ceiling colors
    */
    if (!parse_resolution_settings(fd))
        return (cleanup_and_fail(fd));
        
    if (!parse_texture_paths(fd))
        return (cleanup_and_fail(fd));
        
    if (!parse_color_values(fd))  
        return (cleanup_and_fail(fd));
    
    // PHASE 3: PARSE MAP GRID DATA
    /*
    Convert map ASCII representation to:
    - 2D integer grid for collision detection
    - Player starting position (x, y)
    - Player starting orientation (direction vector)
    */
    if (!parse_map_grid(fd))
        return (cleanup_and_fail(fd));
    
    // PHASE 4: VALIDATION
    if (!validate_complete_configuration())
        return (cleanup_and_fail(fd));
    
    close(fd);
    return (1);
}

/*
MAP GRID PARSING STRATEGY:
- Read all map lines into buffer
- Calculate grid dimensions (width x height)
- Convert ASCII characters to integers
- Validate map integrity (closed walls, single player)
*/
int parse_map_grid(int fd)
{
    // Read all remaining lines as map data
    char **map_lines = read_map_lines(fd);
    
    // Calculate grid dimensions
    calculate_map_dimensions(map_lines);
    
    // Allocate map grid memory
    allocate_map_grid();
    
    // Convert characters to collision integers
    for (int y = 0; y < map_height; y++) {
        for (int x = 0; x < map_width; x++) {
            char cell = map_lines[y][x];
            
            // Convert map symbols
            if (cell == '1') map_grid[y][x] = WALL;
            if (cell == '0') map_grid[y][x] = EMPTY;
            if (cell == ' ') map_grid[y][x] = VOID;
            
            // Extract player information  
            if (cell == 'N' || cell == 'S' || cell == 'E' || cell == 'W') {
                set_player_start_position(x, y);
                set_player_start_direction(cell);
                map_grid[y][x] = EMPTY; // Player stands on empty space
            }
        }
    }
    
    return (validate_map_integrity());
}

/*
MAP VALIDATION REQUIREMENTS:
- Map must be completely surrounded by walls
- Exactly one player starting position
- No invalid characters in map data
- Map must be rectangular (no jagged edges)
*/
int validate_map_integrity(void)
{
    // Check map boundaries are all walls
    if (!check_boundary_walls())
        return (0);
    
    // Verify single player start found
    if (player_start_count != 1)
        return (0);
    
    // Ensure no unreachable areas
    if (!verify_map_connectivity())
        return (0);
        
    return (1);
}

/*
PLAYER INITIALIZATION:
Set player starting position and orientation from map data
*/
void set_player_start_position(int x, int y)
{
    // Convert map grid coordinates to world coordinates
    // Place player in center of grid cell
    g_game.player.pos_x = (double)x + 0.5;
    g_game.player.pos_y = (double)y + 0.5;
}

void set_player_start_direction(char direction)
{
    // Set player direction vector based on map symbol
    if (direction == 'N')  // North - facing up
    {
        g_game.player.dir_x = 0.0;
        g_game.player.dir_y = -1.0;
    }
    else if (direction == 'S')  // South - facing down
    {
        g_game.player.dir_x = 0.0;
        g_game.player.dir_y = 1.0;
    }
    else if (direction == 'E')  // East - facing right
    {
        g_game.player.dir_x = 1.0;
        g_game.player.dir_y = 0.0;
    }
    else if (direction == 'W')  // West - facing left
    {
        g_game.player.dir_x = -1.0;
        g_game.player.dir_y = 0.0;
    }
    
    // Set camera plane perpendicular to direction for FOV
    // Camera plane determines field of view width
    if (direction == 'N' || direction == 'S')
    {
        g_game.player.camera_plane_x = FOV;
        g_game.player.camera_plane_y = 0.0;
    }
    else  // E or W
    {
        g_game.player.camera_plane_x = 0.0;
        g_game.player.camera_plane_y = FOV;
    }
}