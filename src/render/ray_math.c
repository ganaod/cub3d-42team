/* Parametric line intersection with discrete grid
ray casting & DDA algos */

#include "../inc/cub3d.h"



/* create peripheral vision
transform raycaster from 1D scanner > 3D vision simulator

transform each screen col > ray dir vector (viewing angle)
visual result: natural FOV instead of "tunnel vision"

without: all cols show same straight-ahead view (telescope effect)
with: l/r cols show angled views (human-like peripheral vision)

dependencies: g_game.player (pos/dir),
			g_game_graphics (screen width)


steps:

1. screen pixel > FOV pos
. leftmost pixel (0) becomes -1 (left edge of vision)
. center pixel becomes 0 (straight ahead)
. rightmost pixel becomes +1 (right edge of vision)
normalisation formula for range mapping:
	. scale to [0,2]
	. shift to [-1,+1]

2. calculate actual ray direction for this viewing angle
. base dir (where player faces) + angular offset (l/r tilt)
. visual: creates "sweep" of vision from left peripheral > right peripheral 

vector addition with scaling */
void	calculate_ray_direction(int screen_x, double *ray_dir_x, double *ray_dir_y)
{
	double	camera_x;

	camera_x = 2.0 * screen_x / (double)g_game.graphics.screen_width - 1.0;
	*ray_dir_x = g_game.player.dir_x + camera_x * g_game.player.camera_plane_x;
	*ray_dir_y = g_game.player.dir_y + camera_x * g_game.player.camera_plane_y;
}

/* visual reality: How far can I see before hitting a wall?
. Player stands at position, looks toward screen column X
. vision travels in straight line until blocked by wall
. function returns: how far that vision traveled

without: no depth perception - every wall would appear
same size regardless of distance

core transaction:
	. in: ray direction (where to look)
	. out: distance to nearest wall in that direction

problem: find intersection of continuous line with discrete grid
constraint: must be exact (no approximatn errs)
performance: must execute 1000+ time/frame (real-time requirement)

Digital Differential Analyzer (DDA) algorithm: 
instead of checking arbitrary pts, only check grid
boundary crossings

Step through map grid until wall collision detected
Mathematically efficient grid traversal

ret: perpendicular dist (prevent fisheye distortion) */

// coordinator
double	cast_ray_to_wall(double ray_dir_x, double ray_dir_y, int *wall_side)
{
	t_dda_state	state;
	double		wall_distance;

	setup_dda_vars(ray_dir_x, ray_dir_y, &state);
	wall_distance = execute_dda_traversal(&state, wall_side);
	return (wall_distance);
}

/* initialiser
calculate all mathematical setup vars
transform ray dir > DDA stepping params */
void	setup_dda_vars(double ray_dir_x, double ray_dir_y, t_dda_state *state)
{
	state->map_x = (int)g_game.player.pos_x;
	state->map_y = (int)g_game.player.pos_y;
	state->delta_dist_x = fabs(1.0 / ray_dir_x);
	state->delta_dist_y = fabs(1.0 / ray_dir_y);
	if (ray_dir_x < 0)
	{
		state->step_x = -1;
		state->side_dist_x = (g_game.player.pos_x - state->map_x)
							* state->delta_dist_x;
	}
	else
	{
		state->step_x = 1;
		state->side_dist_x = (state->map_x + 1.0 - g_game.player.pos_x)
							* state->delta_dist_x;
	}
	// y dir
	state->wall_hit = 0;
}

// UPCOMING
int get_wall_face_hit(/* DDA intersection data */);