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



/*
1. initialise buffer
. clear screen buff to known state (black pixels)
. prepare canvas for additive composition

2. raycasting core loop
. for each screen col: cast ray, find wall, draw vert strip
. screen width determines ray count (1 ray / col)
. rays spread across player's FOV */
void	render_complete_frame(void)
{
	int	screen_x;
	int	screen_width;

	clear_screen_buffer();
	screen_width = g_game.graphics.screen_width;
	screen_x = 0;
	while (screen_x < screen_width)
	{
		render_single_column(screen_x);
		screen_x++;
	}
}


/* ray processing for 1 screen col
flow:
Ray Direction → Wall Distance → Screen Height → Pixels */
void	render_single_column(int screen_x)
{
	double	ray_dir_x;
	double	ray_dir_y;
	double	wall_distance;
	int		wall_direction;

	calculate_ray_direction(screen_x, &ray_dir_x, &ray_dir_y);
	wall_distance = cast_ray_to_wall(ray_dir_x, ray_dir_y, &wall_direction);
	draw_wall_column(screen_x, wall_distance, wall_direction);
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
