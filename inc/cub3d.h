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
/* Zelltypen fürs Grid */
# define CELL_VOID  (-1)  /* ' '  -> außerhalb/ungefüllt */
# define CELL_EMPTY (0)   /* '0'  -> begehbar */
# define CELL_WALL  (1)   /* '1'  -> Wand  */

#ifndef FOV_PLANE
# define FOV_PLANE 0.66
#endif

/* Richtungscodes */
# define DIR_N 0
# define DIR_S 1
# define DIR_W 2
# define DIR_E 3

/* map cell types
the algorithm casts rays until hitting WALL,
treats EMPTY as traversable,
and VOID handles map parsing edge cases
where spaces might exist beyond defined boundaries. */
# define EMPTY 0		// walkable space
# define WALL 1			// collision / rendering surface
# define VOID 2			// invalid map space (outside boundaries)

// wall face directions (for texture selection)
# define NORTH 0
# define SOUTH 1
# define WEST 2
# define EAST 3

// wall side types (for DDA)
# define VERTICAL_WALL	0
# define HORIZONTAL_WALL 1

// rendering constants
# define DEFAULT_WIDTH	1024		// screen sampling resolution
# define DEFAULT_HEIGHT	768			// vertical pixel count
# define FOV 			0.66		// field of view / radians

typedef struct s_texture_image
{
	mlx_image_t		*mlx_image;		// mlx42 image pointer
	uint32_t		*pixels;		// pixel buffer
	int				width;			// image width
	int				height;			// image height
}					t_texture_image;

// player state
typedef struct s_player
{
	double			pos_x;			// world position x
	double			pos_y;			// world position y
	double			dir_x;			// view direction x
	double			dir_y;			// view direction y
	double			camera_plane_x;	// camera plane x (fov)
	double			camera_plane_y;	// camera plane y (fov)
}					t_player;

// map configuration
typedef struct s_map
{
	int				*grid;				// 2d map as 1d array
	int				width;				// map width in cells
	int				height;				// map height in cells
	char			*texture_paths[4];	// wall texture file paths
	uint32_t		floor_color;		// 0xrrggbb
	uint32_t		ceiling_color;		// 0xrrggbb
	char			*first_map_line;	// first line of map
	t_texture_image	*wall_textures[4];	// wall textures
}					t_map;

// graphics system
typedef struct s_graphics
{
	mlx_t			*mlx;			// mlx42 context
	mlx_image_t		*frame;			// framebuffer (win_w x win_h, rgba8888)
	int				screen_width;	// window width
	int				screen_height;	// window height
}					t_graphics;

// dda temp algorithm state
typedef struct s_dda_state
{
	int				map_x;			// current grid x pos
	int				map_y;			// current grid y pos
	int				step_x;			// x step dir (-1 / +1)
	int				step_y;			// y step dir (-1 / +1)
	double			delta_dist_x;	// dist to traverse 1 x grid cell
	double			delta_dist_y;	// dist to traverse 1 y grid cell
	double			side_dist_x;	// dist to next x grid boundary
	double			side_dist_y;	// dist to next y grid boundary
	double			ray_dir_x;		// ray direction x
	double			ray_dir_y;		// ray direction y
	int				wall_hit;		// wall collision flag
}					t_dda_state;

// column rendering state
// like dda state: temporary algorithmic struct, not a persistent game entity
// groups related data to conform to 42 norm
typedef struct s_column_render
{
	double			wall_distance;	// perpendicular distance to wall
	int				wall_height;	// calculated wall height
	int				wall_direction;	// wall side hit (0-3)
}					t_column_render;

/* ray casting: a fn that maps:
	in: ray_origin, ray_direction
	out: distance, wall_side, hit_coords 
hence new struct */
typedef struct	s_ray_result
{
	double			distance;		// perpendicular wall dist
	int				wall_side;		// VERT / HORIZ
	double			hit_x;
	double			hit_y;
	int				wall_face;		// n/s/e/w
}					t_ray_result;

typedef struct	s_texture_context
{
	int				wall_direction;
	double			wall_hit_x;
	double			wall_hit_y;
	int				wall_height;
}					t_texture_context;

typedef struct s_texinfo
{
	char			**slot;			// texture slot
	const char		*line;			// config line
	int				j;				// index
	int				bit;			// bit flag
	int				*flags;			// pointer to flags
}					t_texinfo;

// game state
typedef struct s_game
{
	t_player		player;			// player data
	t_map			map;			// map data
	t_graphics		graphics;		// graphics system
	int				running;		// game loop flag
	double			movement_speed;	// player movement speed
	double			rotation_speed;	// camera rotation speed
}					t_game;

// global game state
extern t_game		g_game;

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

//  ================== (NO SURR-)RENDER(!!!) ==================
// main render pipeline
void			render_complete_frame(void);
void			render_single_column(int screen_x);

// ray math
void			calculate_ray_direction(int screen_x,
					double *ray_dir_x, double *ray_dir_y);

// dda algo
t_ray_result	cast_ray_to_wall(double ray_dir_x, double ray_dir_y);
void			execute_dda_traversal(t_dda_state *state, int *wall_side);
double			calculate_wall_distance(t_dda_state *state, int wall_side);

// dda setup utils
void			setup_dda_vars(double ray_dir_x, double ray_dir_y,
					t_dda_state *state);

// projection calc
int				calculate_screen_wall_height(double wall_distance);
void			calculate_wall_boundaries(int wall_height,
					int *wall_start, int *wall_end);

// texture sampling
int				get_wall_texture_color(t_texture_context *ctx, int screen_y);
double			calculate_texture_u(t_texture_context *ctx);
double			calculate_texture_v(t_texture_context *ctx, int screen_y);
int				sample_texture_pixel(t_texture_image *tex,
					int tex_x, int tex_y);
t_texture_image	*get_texture_for_direction(int wall_direction);

// wall rendering
void			render_wall_column(int screen_x, double wall_distance,
					int wall_direction);

// screen buffer ops
void			clear_screen_buffer(void);
void			put_pixel(int x, int y, int color);
void			present_frame_to_screen(void);


//  ================== MAP_GRID_CELL ==================

int	put_cell_from_char(t_map *m, t_player *pl, int idx, char c);

//  ================== MAP_GRID_FILL ==================

int	fill_grid(t_map *m, t_player *pl, char **lines, int *count);

//  ================== MAP_GRID_BUILD ==================

int	build_grid_from_lines(t_map *m, t_player *pl, char **lines,
		int *player_found);


#endif
