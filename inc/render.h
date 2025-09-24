/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 11:57:43 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/24 16:05:57 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_H
# define RENDER_H

# include "cub3d.h"

# define MINIMUM_WALL_DISTANCE_THRESHOLD 1.0
# define MAXIMUM_WALL_HEIGHT_MULTIPLIER 2

# define VERTICAL_WALL 0
# define HORIZONTAL_WALL 1

typedef struct s_dda_state
{
	int			map_x;
	int			map_y;
	int			step_x;
	int			step_y;
	double		delta_dist_x;
	double		delta_dist_y;
	double		world_dist_to_next_boundary_x;
	double		world_dist_to_next_boundary_y;
	double		world_ray_dir_x;
	double		world_ray_dir_y;
	int			wall_intersection_found;
}				t_dda_state;

typedef struct s_distance_calc_context
{
	double		wall_grid_position;
	double		player_position;
	double		ray_direction_component;
	double		wall_face_offset;
	int			step_direction;
}				t_distance_calc_context;

typedef struct s_ray_result
{
	double		world_intersection_x;
	double		world_intersection_y;
	int			world_wall_side;
	double		world_perpendicular_distance;
	int			world_wall_face;
}				t_ray_result;

typedef struct s_texture_context
{
	int			world_wall_face;
	double		world_wall_intersection_x;
	double		world_wall_intersection_y;
	int			screen_wall_height;
}				t_texture_context;

void			render_complete_frame(t_game *g);
void			render_single_column(t_game *g, int screen_column_x);
void			render_wall_column(t_game *g, int screen_column_x,
					t_ray_result *wall_intersection_data,
					int projected_wall_height);
void			render_ceiling_section(t_game *g, int screen_column_x,
					int wall_start_y_pixel);
void			render_wall_section(t_game *g, int screen_column_x,
					int wall_start_y_pixel, int wall_end_y_pixel,
					t_ray_result *wall_hit_data);
void			render_floor_section(t_game *g, int screen_column_x,
					int wall_end_y_pixel);

void			calculate_ray_direction(t_game *g, int screen_column_x,
					double *world_ray_direction_x,
					double *world_ray_direction_y);
t_ray_result	cast_ray_to_wall(t_game *g, double world_ray_dir_x,
					double world_ray_dir_y);
int				determine_intersected_wall_face(t_game *g,
					t_ray_result *wall_intersection_data);

void			execute_dda_traversal(t_game *g, t_dda_state *dda_state,
					int *world_wall_side);
double			calculate_wall_distance(t_game *g, t_dda_state *dda_state,
					int world_wall_side);

void			setup_dda_vars(t_game *g, double ray_dir_x, double ray_dir_y,
					t_dda_state *state);

int				calculate_screen_wall_height(t_game *g,
					double world_wall_distance);
void			simulate_eye_level_perspective(t_game *g, int wall_height,
					int *wall_start, int *wall_end);

int				screen_pixel_texture_colour(t_game *g, t_texture_context *ctx,
					int current_pixel_y);

double			safe_fractional_part(double coordinate);
int				clamp_texture_pixel(int pixel_coord, int max_dimension);

void			clear_screen_buffer(t_game *g);
void			put_pixel(t_game *g, int screen_x, int screen_y,
					int pixel_color);
double			compute_perpendicular_distance(t_distance_calc_context *ctx);
void			setup_x_step(t_game *g, double ray_dir_x, t_dda_state *state);
void			setup_y_step(t_game *g, double ray_dir_y, t_dda_state *state);
void			centre_wall_at_eye_level(t_game *g, int wall_height,
					int *wall_start, int *wall_end);
void			enforce_screen_pixel_boundaries(t_game *g, int *wall_start,
					int *wall_end);
double			screen_wall_texture_v(t_game *g, t_texture_context *ctx,
					int curr_px_y);
double			world_wall_texture_u(t_texture_context *ctx);
int				screen_pixel_texture_colour(t_game *g, t_texture_context *ctx,
					int current_pixel_y);
int				texture_pixel_colour(t_texture_image *tx_img, int tx_px_x,
					int tx_px_y);

#endif
