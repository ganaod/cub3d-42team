#ifndef RENDER_H
# define RENDER_H

# include "cub3d.h"
/* hierarchy:
cub3d.h (core types)
    ↑
render.h (depends on core, adds rendering)
    ↑  
render_*.c files (include render.h) 

render.h NOT included in cub3d.h: would create
circular dependency */

// ⭐ RENDERING CONSTRAINTS
/* boundary protection:
. / by 0: clamp min distance
. screen overflow: limit max wall height */
#define MINIMUM_WALL_DISTANCE_THRESHOLD	1.0
#define MAXIMUM_WALL_HEIGHT_PIXELS		(g_game.graphics.screen_height * 2)

// ⭐ ALGORITHM CONSTANTS
// wall side types (for DDA)
# define VERTICAL_WALL					0
# define HORIZONTAL_WALL				1

// ⭐⭐ RENDERING STRUCTS
// ⭐ ALGORITHM STATE
typedef struct s_dda_state
{
	int				map_x;
	int				map_y;
	int				step_x;
	int				step_y;
	double			delta_dist_x;							// distance per grid step
	double			delta_dist_y;
	double			world_dist_to_next_boundary_x;			// dist from origin to next boundary
	double			world_dist_to_next_boundary_y;
	double			world_ray_dir_x;						// normalised ray dir vector
	double			world_ray_dir_y;
	int				wall_intersection_found;				// boolean flag
}					t_dda_state;

// wall distance calculation context
typedef struct s_distance_calc_context
{
	double			wall_grid_position;			// wall cell coordinate
	double			player_position;			// player coordinate (same axis)
	double			ray_direction_component;	// ray direction (same axis)
	double			wall_face_offset;			// 0.0 or 0.5 for correct face
	int				step_direction;				// +1 or -1 movement direction
}					t_distance_calc_context;

// dda traversal output (ray space > world space)
typedef struct	s_ray_result
{
	double			world_intersection_x;
	double			world_intersection_y;
	int				world_wall_side;
	double			world_perpendicular_distance;
	int				world_wall_face;
}					t_ray_result;

typedef struct	s_texture_context
{
	int				world_wall_face;
	double			world_wall_intersection_x;
	double			world_wall_intersection_y;
	int				screen_wall_height;
}					t_texture_context;

// ⭐⭐ RENDERING FUNCTION PROTOTYPES
// main.c
void			render_complete_frame(void);
void			render_single_column(int screen_column_x);

// column.c
void			render_wall_column(int screen_column_x, t_ray_result *wall_intersection_data, int projected_wall_height);
void			render_ceiling_section(int screen_column_x, int wall_start_y_pixel);
void			render_wall_section(int screen_column_x, int wall_start_y_pixel, int wall_end_y_pixel, t_ray_result *wall_hit_data);
void			render_floor_section(int screen_column_x, int wall_end_y_pixel);

// ray_cast.c
void			calculate_ray_direction(int screen_column_x, double *world_ray_direction_x, double *world_ray_direction_y);
// void			calculate_ray_direction_tunnel_vision(int screen_column_x, double *world_ray_direction_x, double *world_ray_direction_y);
t_ray_result	cast_ray_to_wall(double world_ray_dir_x, double world_ray_dir_y);
int				determine_intersected_wall_face(t_ray_result *wall_intersection_data);

// dda.c
void			execute_dda_traversal(t_dda_state *dda_state, int *world_wall_side);
double			calculate_wall_distance(t_dda_state *dda_state, int world_wall_side);

// dda_setup.c
void			setup_dda_vars(double world_ray_dir_x, double world_ray_dir_y, t_dda_state *dda_state);

// projection.c
int				calculate_screen_wall_height(double world_wall_distance);
void			simulate_eye_level_perspective(int wall_height, int *wall_start, int *wall_end);

// texture.c
int				screen_pixel_texture_colour(t_texture_context *ctx, int current_pixel_y);

// texture_utils.c
double			safe_fractional_part(double coordinate);
int				clamp_texture_pixel(int pixel_coord, int max_dimension);

// pixel_buffer.c
void			clear_screen_buffer(void);
void			put_pixel(int x, int y, int color);

#endif