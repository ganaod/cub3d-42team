/* Parametric line intersection with discrete grid
ray casting & DDA algos */

#include "../inc/cub3d.h"



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


/* create peripheral vision effect

transform each screen col > ray dir vector (viewing angle)
visual result: natural FOV instead of "tunnel vision"

without: all cols show same straight-ahead view (telescope effect)
with: l/r cols show angled views (human-like peripheral vision)

dependencies: g_game.player (pos/dir),
			g_game_graphics (screen width)

steps:
1. screen space [0, width-1] > camera space [-1, +1]
2. ray = player dir + camera plane offset */
void	calculate_ray_direction(int screen_x, double *ray_dir_x, double *ray_dir_y)
{
	double	camera_x;

	camera_x = 2.0 * screen_x / (double)g_game.graphics.screen_width - 1.0;

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


// UPCOMING
int get_wall_face_hit(/* DDA intersection data */);