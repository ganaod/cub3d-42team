#include "../inc/cub3d.h"

texture_image *get_texture_for_direction(int wall_direction);
int calculate_texture_x(double wall_hit_pos, int wall_side);
int calculate_texture_y(int screen_y, int wall_start, int wall_height);
int sample_texture_pixel(texture_image *tex, int tex_x, int tex_y);

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