#ifndef CUB3D_H
# define CUB3D_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>	
# include <math.h>
# include "../lib/MLX42/include/MLX42/MLX42.h"
# include "../lib/libft/libft/libft.h"
# include "../lib/libft/gnl/get_next_line.h"
# include "../lib/libft/ft_printf/ft_printf.h"

# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1
// # define BUFFER_SIZE 1024

# define HDR_NO (1<<0)
# define HDR_SO (1<<1)
# define HDR_WE (1<<2)
# define HDR_EA (1<<3)
# define HDR_F  (1<<4)
# define HDR_C  (1<<5)

// map cell types
# define EMPTY 0		// traversable space
# define WALL 1			// collision / rendering surface
# define VOID 2			// invalid map space (outside boundaries)

// wall face directions (for texture selection)
# define NORTH 0
# define SOUTH 1
# define WEST 2
# define EAST 3
/* Determines which specific wall face was hit
Used for texture selection (different textures per face)
Calculated from VERTICAL_WALL/HORIZONTAL_WALL + ray direction */

// wall side types (for DDA)
# define VERTICAL_WALL	0
# define HORIZONTAL_WALL 1
/* Determines which type of grid line was crossed
Used for distance calculation (different formulae for X vs Y boundaries)
Set during DDA traversal based on which boundary was closer */

// rendering constants
# define DEFAULT_WIDTH	1024		// screen sampling resolution
# define DEFAULT_HEIGHT	768			// vertical pixel count
# define FOV 			0.66		// field of view / radians



// player state
typedef struct s_player
{
	double		pos_x;				// World position X
	double		pos_y;				// World position Y
	double		dir_x;				// View direction X
	double		dir_y;				// View direction Y
	double		camera_plane_x;		// Camera plane X (FOV)
	double		camera_plane_y;		// Camera plane Y (FOV)
}	t_player;

/* MAP CONFIGURATION */
typedef struct s_map
{
	int			*grid;				// 2D map as 1D array
	int			width;				// Map width in cells
	int			height;				// Map height in cells
	char		*texture_paths[4];	// Wall texture file paths
	uint32_t	floor_color;		// 0xRRGGBB
	uint32_t	ceiling_color;		// 0xRRGGBB
	char		*first_map_line;
}	t_map;

/* GRAPHICS SYSTEM */
typedef struct s_graphics
{
	mlx_t		*mlx;				// MLX42 Kontext
	mlx_image_t	*frame;				// Framebuffer (WIN_W x WIN_H, RGBA8888)
	int			screen_width;
	int			screen_height;
}	t_graphics;

// dda temp algorithm state
typedef struct s_dda_state
{
	int			map_x;				// current grid x pos
	int			map_y;				// current grid y pos
	int			step_x;				// x step dir (-1 / +1)
	int			step_y;				// y step dir (-1 / +1)
	double		delta_dist_x;		// dist to traverse 1 X grid cell
	double		delta_dist_y;		// dist to traverse 1 Y grid cell
	double		side_dist_x;		// dist to next X grid boundary
	double		side_dist_y;		// dist to next Y grid boundary
	double		ray_dir_x;
	double		ray_dir_y;
	int			wall_hit;			// wall collision flag
}	t_dda_state;

// column rendering state
// like dda state: temporary algorithmic struct, not a persistent game entity
// groups related data to conform to 42 Norm
typedef struct	s_column_render
{
	double		wall_distance;
	int			wall_height;
	int			wall_direction;
}	t_column_render;

typedef struct s_texinfo
{
	char		**slot;
	const char	*line;
	int			j;
	int			bit;
	int			*flags;
}	t_texinfo;


// game state
typedef struct s_game
{
	t_player	player;
	t_map		map;
	t_graphics	graphics;
	int			running;		// game loop flag
	double		movement_speed;
	double		rotation_speed;	// (camera)
}	t_game;

// global game state
extern t_game	g_game;

//  ================== PARSE_UTILS ==================

int	skip_ws(const char *s, int i);
int	parse_u8_component(const char *s, int *i, int *out);
int expect_comma(const char *s, int *i);
int	parse_rgb_triplet(const char *s, uint32_t *out_rgb);

//  ================== PARSE_HEADER ==================

int	set_texture_field(char **dst_path, const char *after_key);
int	set_color_field(uint32_t *dst_rgb, const char *after_key, int *was_set);
int	parse_header_line(t_map *m, const char *line, int *flags);
int	match2(const char *s, int i, char a, char b);
int	set_tex_entry(char **slot, const char *after_key, int bit, int *flags);
int	parse_header_texture(t_map *m, const char *line, int i, int *flags);
int	parse_header_color(t_map *m, const char *line, int i, int *flags);

//  ================== PARSE_HEADER ==================

int	parse_header_lines(t_map *m, int fd);

//  ================== COLLECT_MAP_LINES ==================

int	collect_map_lines(t_map *m, int fd, char ***out_lines, int *out_h);
int	append_line(char ***lines_ptr, int *cap_ptr, int *h_ptr, char *take);
int	validate_map_line(const char *s);
void	rstrip_eol(char *s);

//  ================== NORMALIZE_MAP ==================

int	normalize_map(char ***lines_io, int h, int *out_w);

// RENDER
// main render pipeline
void	render_complete_frame(void);
void	render_single_column(int screen_x);

// ray math
void	calculate_ray_direction(int screen_x, double *ray_dir_x, double *ray_dir_y);

// dda algo
double	cast_ray_to_wall(double ray_dir_x, double ray_dir_y, int *wall_side);
void	execute_dda_traversal(t_dda_state *state, int *wall_side);
double	calculate_wall_distance(t_dda_state *state, int wall_side);

// dda setup utils
void	setup_dda_vars(double ray_dir_x, double ray_dir_y, t_dda_state *state);

// projection calc
int		calculate_screen_wall_height(double wall_distance);
void	calculate_wall_boundaries(int wall_height, int *wall_start, int *wall_end);

// texture sampling
int		get_wall_texture_color(int wall_direction, int screen_y, int wall_height);

// wall rendering
void	render_wall_column(int screen_x, double wall_distance, int wall_direction);

// screen buffer ops
void	clear_screen_buffer(void);
void	put_pixel(int x, int y, int color);
void	present_frame_to_screen(void);

#endif
