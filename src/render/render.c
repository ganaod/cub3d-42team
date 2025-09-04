/* raycasting & frame rendering */

#include "../inc/cub3d.h"

/* RESPONSIBILITY:
- Implement core raycasting mathematics
- Transform 3D world into 2D screen representation
- Manage rendering pipeline from player view to pixels
- Handle texture mapping and visual effects */

/*
RAYCASTING FUNDAMENTAL CONCEPT:
For each screen column:
1. Cast ray from player through that column
2. Find distance to nearest wall
3. Calculate wall height based on distance
4. Draw vertical strip representing that wall segment

MATHEMATICAL FOUNDATION:
- Ray = Player Position + Direction * Distance
- Wall Height = Screen Height / Wall Distance  
- Perspective correction prevents fisheye distortion */

void render_complete_frame(void)
{
    // 1. clear screen buffer
    clear_screen_buffer();
    
    // 2. cast rays for each screen column
    /*
    Screen width determines ray count.
    Each column gets exactly one ray.
    Rays spread across player's field of view.
    */
    for (int screen_x = 0; screen_x < g_game.graphics.screen_width; screen_x++)
    {
		double ray_dir_x, ray_dir_y;

        // Calculate ray direction for this column
        calculate_ray_direction(screen_x, &ray_dir_x, &ray_dir_y);
        
        // Cast ray and find wall collision
        double wall_distance = cast_ray_to_wall();
        
        // Determine which wall face was hit (N/S/E/W)
        int wall_direction = get_wall_face_hit();
        
        // Calculate wall strip height on screen
        int wall_height = calculate_screen_wall_height(wall_distance);
        
        // Draw the vertical wall strip
        render_wall_column(screen_x, wall_height, wall_direction);
    }
    
    // PHASE 3: RENDER UI ELEMENTS
    render_user_interface();
}

/*
WALL RENDERING: Convert distance to screen coordinates
*/
void render_wall_column(int screen_x, int wall_height, int wall_direction)
{
    // Calculate wall strip boundaries on screen
    int wall_start_y = (screen_height - wall_height) / 2;
    int wall_end_y = wall_start_y + wall_height;
    
    // CEILING RENDERING: Above wall
    for (int y = 0; y < wall_start_y; y++)
        put_pixel(screen_x, y, ceiling_color);
    
    // WALL RENDERING: Apply texture if available
    for (int y = wall_start_y; y < wall_end_y; y++) {
        int texture_color = get_wall_texture_color(wall_direction, y, wall_height);
        put_pixel(screen_x, y, texture_color);
    }
    
    // FLOOR RENDERING: Below wall
    for (int y = wall_end_y; y < screen_height; y++)
        put_pixel(screen_x, y, floor_color);
}
