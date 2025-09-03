/* raycasting & frame rendering */

#include "cub3d.h"

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
        draw_wall_column(screen_x, wall_height, wall_direction);
    }
    
    // PHASE 3: RENDER UI ELEMENTS
    render_user_interface();
}

/*
MATHEMATICAL TRANSFORM: Screen Column → Ray Direction Vector
INPUT: Screen column index
OUTPUT: Ray direction components (by reference)
DEPENDENCIES: Global player state, screen dimensions
*/
void calculate_ray_direction(int screen_x, double *ray_dir_x, double *ray_dir_y)
{
    double camera_x;
    
    // STEP 1: Screen space [0, width-1] → Camera space [-1, +1]
    camera_x = 2.0 * screen_x / (double)g_game.graphics.screen_width - 1.0;
    
    // STEP 2: Ray = Player Direction + Camera Plane Offset
    *ray_dir_x = g_game.player.dir_x + camera_x * g_game.player.camera_plane_x;
    *ray_dir_y = g_game.player.dir_y + camera_x * g_game.player.camera_plane_y;
}


/*
RAY CASTING ALGORITHM (DDA - Digital Differential Analyzer):
Step through map grid until wall collision detected.
Mathematically efficient grid traversal.
*/
double cast_ray_to_wall(void)
{
    // INITIALIZATION: Starting position and direction
    double ray_x = player.position_x;
    double ray_y = player.position_y; 
    double ray_dir_x = current_ray_direction_x;
    double ray_dir_y = current_ray_direction_y;
    
    // GRID TRAVERSAL: Step through map cells
    int map_x = (int)ray_x;
    int map_y = (int)ray_y;
    
    // DISTANCE CALCULATION: How far to next grid line?
    double delta_dist_x = calculate_delta_distance_x();
    double delta_dist_y = calculate_delta_distance_y();
    
    // STEPPING: Which direction and how far?
    calculate_step_direction_and_distances();
    
    // DDA LOOP: Step until wall hit
    int wall_hit = 0;
    while (!wall_hit)
    {
        // Move to next grid intersection
        if (side_dist_x < side_dist_y) {
            side_dist_x += delta_dist_x;
            map_x += step_x;
            wall_side = VERTICAL_WALL; // Hit vertical wall
        } else {
            side_dist_y += delta_dist_y;
            map_y += step_y;  
            wall_side = HORIZONTAL_WALL; // Hit horizontal wall
        }
        
        // Check if current grid cell contains wall
        if (map_grid[map_y][map_x] == WALL)
            wall_hit = 1;
    }
    
    // DISTANCE CALCULATION: Prevent fisheye distortion
    if (wall_side == VERTICAL_WALL)
        return ((map_x - ray_x + (1 - step_x) / 2) / ray_dir_x);
    else
        return ((map_y - ray_y + (1 - step_y) / 2) / ray_dir_y);
}

/*
WALL RENDERING: Convert distance to screen coordinates
*/
void draw_wall_column(int screen_x, int wall_height, int wall_direction)
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

/*
TEXTURE MAPPING: Map wall texture to screen coordinates
*/
int get_wall_texture_color(int wall_direction, int screen_y, int wall_height)
{
    // Determine which texture to use based on wall face
    texture_image *wall_texture = get_texture_for_direction(wall_direction);
    
    // Calculate texture coordinates
    double texture_y_ratio = (double)(screen_y - wall_start_y) / wall_height;
    int texture_y = (int)(texture_y_ratio * wall_texture->height);
    
    // Extract color from texture at calculated coordinates
    return (extract_texture_pixel(wall_texture, texture_x, texture_y));
}

/* screen buffer management:
clear & present frame buffer ops */
void clear_screen_buffer(void)
{
    int total_pixels;
    int *pixel_data;
    int i;
    
    // Calculate total pixels in screen buffer
    total_pixels = g_game.graphics.screen_width * g_game.graphics.screen_height;
    
    // Get pointer to image data as integer array for faster clearing
    pixel_data = (int *)g_game.graphics.image_data;
    
    // Clear entire screen to black (0x000000)
    i = 0;
    while (i < total_pixels)
    {
        pixel_data[i] = 0x000000;
        i++;
    }
}

void present_frame_to_screen(void)
{
    // Display the completed frame buffer to the window
    mlx_put_image_to_window(
        g_game.graphics.mlx_ptr,
        g_game.graphics.window_ptr,
        g_game.graphics.image_ptr,
        0, 0
    );
}

/* direct pixel access to image buffer */
void put_pixel(int x, int y, int color)
{
    int pixel_index;
    int *pixel_data;
    
    // Bounds checking
    if (x < 0 || x >= g_game.graphics.screen_width || 
        y < 0 || y >= g_game.graphics.screen_height)
        return;
    
    // Calculate pixel position in buffer
    pixel_index = y * g_game.graphics.screen_width + x;
    
    // Get pixel data as integer array
    pixel_data = (int *)g_game.graphics.image_data;
    
    // Set pixel color
    pixel_data[pixel_index] = color;
}