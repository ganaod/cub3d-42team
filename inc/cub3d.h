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

# define HDR_NO (1<<0)
# define HDR_SO (1<<1)
# define HDR_WE (1<<2)
# define HDR_EA (1<<3)
# define HDR_F  (1<<4)
# define HDR_C  (1<<5)



// screen space:
// 2D array of discrete pixels representing the 3D view
# define DEFAULT_WIDTH		1024		// screen sampling resolution
# define DEFAULT_HEIGHT		768			// vertical pixel count
/* Coordinate Mapping:
Screen X: [0 to 1023] - Left to right pixel columns
Screen Y: [0 to 767]  - Top to bottom pixel rows
Origin: (0,0) at top-left corner	*/



/* Zelltypen fürs Grid */
# define CELL_VOID  (-1)  /* ' '  -> außerhalb/ungefüllt */
# define CELL_EMPTY (0)   /* '0'  -> begehbar */
# define CELL_WALL  (1)   /* '1'  -> Wand  */



// FOV GEOMETRIC CONFIGURATION
# define FOV_DEGREES					60.0
// good perspective without excessive distortion

# define FOV_RADIANS					(FOV_DEGREES * (M_PI / 180.0))
/* convert FOV from degrees > radians for C's math fns
M_PI is a constant defined in <math.h> for the value of Pi */

# define FOV_HALF_ANGLE_RADIANS			(FOV_RADIANS / 2.0)
// Half-angle needed for tangent calculation

# define FOV_CAMERA_PLANE_MAGNITUDE		tan(FOV_HALF_ANGLE_RADIANS)
// Camera plane vector magnitude for desired FOV
// This value determines angular spread of vision


// SCREEN > FOV COORDINATE TRANSFORMATION
# define SCREEN_TO_FOV_SCALE_FACTOR		2.0
// Scales normalized screen coordinates [0,1] to [0,2]

# define FOV_CENTER_OFFSET				1.0
// Shifts scaled coordinates [0,2] to FOV space [-1,+1]


/* Richtungscodes */
# define DIR_N 0
# define DIR_S 1
# define DIR_W 2
# define DIR_E 3


/* WALL GEOMETRY
wall face directions: abs world coords, fixed to map
not relative to player
for texture selection */
# define NORTH	0
# define SOUTH	1
# define WEST	2
# define EAST	3

// WALL SIDE TYPES (for DDA)
# define VERTICAL_WALL		0
# define HORIZONTAL_WALL	1

/* RENDERING CONSTRAINTS
wall height projection - dist/height limits */
#define MINIMUM_WALL_DISTANCE_THRESHOLD  0.001
#define MAXIMUM_WALL_HEIGHT_PIXELS       (g_game.graphics.screen_height * 2)



typedef struct s_texture_image
{
	mlx_image_t		*mlx_image;				// mlx42 image pointer
	uint32_t		*pixels;				// pixel buffer
	int				image_width;			// image width
	int				image_height;			// image height
}					t_texture_image;

// player state
typedef struct s_player
{
	double			world_pos_x;			// world position x
	double			world_pos_y;			// world position y
	double			world_dir_x;			// view direction x
	double			world_dir_y;			// view direction y
	double			world_camera_plane_x;	// camera plane x (fov)
	double			world_camera_plane_y;	// camera plane y (fov)
}					t_player;

// map configuration
typedef struct s_map
{
	int             *grid;               // 1D grid, size = width*height
	int             width;
	int             height;
	char            *texture_paths[4];
	uint32_t        floor_color;         // <— war floor_; jetzt floor_color
	uint32_t        ceiling_color;
	char            *first_map_line;
	t_texture_image wall_textures[4];    // <— KEINE Pointer: struct-Array erwartet
} t_map;


// graphics system
typedef struct s_graphics
{
	mlx_t			*mlx;			// mlx42 context
	mlx_image_t		*frame;			// framebuffer (win_w x win_h, rgba8888)
	int				screen_width;	// window width
	int				screen_height;	// window height
}					t_graphics;

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

// column rendering state
typedef struct s_column_render
{
	double			world_wall_distance;					// perpendicular distance to wall
	int				screen_wall_height;						// calculated wall height
	int				world_wall_face;						// wall face hit (0-3)
}					t_column_render;

typedef struct	s_ray_result
{
	double			world_distance;					// perpendicular wall dist
	int				world_wall_side;				// VERT / HORIZ
	double			world_intersection_x;
	double			world_intersection_y;
	int				world_wall_face;
}					t_ray_result;

typedef struct	s_texture_context
{
	int				world_wall_face;
	double			world_wall_intersection_x;
	double			world_wall_intersection_y;
	int				screen_wall_height;
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
	double			time_prev;
	double			time_now;
}					t_game;

// global game state
extern t_game		g_game;

typedef struct s_ffctx
{
	const t_map *m;   /* map + grid */
	char       *vis;  /* visited flags, size w*h */
	int        *q;    /* queue array, size w*h */
	int         w;    /* width  */
	int         h;    /* height */
	int         head; /* queue head */
	int         tail; /* queue tail */
}	t_ffctx;

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
int	check_texture_paths_exist(const t_map *m);

//  ================== COLLECT_MAP_LINES ==================

int	collect_map_lines(t_map *m, int fd, char ***out_lines, int *out_h);
int	append_line(char ***lines_ptr, int *cap_ptr, int *h_ptr, char *take);
int	validate_map_line(const char *s);
void	rstrip_eol(char *s);

//  ================== NORMALIZE_MAP ==================

int	normalize_map(char ***lines_io, int h, int *out_w);

//  ================== RENDER ==================
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
t_ray_result	cast_ray_to_wall(double world_ray_dir_x, double world_ray_dir_y);
int				determine_intersected_wall_face(t_ray_result *wall_intersection_data);

// dda.c
void			execute_dda_traversal(t_dda_state *dda_state, int *world_wall_side);
double			calculate_wall_distance(t_dda_state *dda_state, int world_wall_side);

// dda_setup.c
void			setup_dda_vars(double world_ray_dir_x, double world_ray_dir_y, t_dda_state *dda_state);

// projection.c
int				calculate_screen_wall_height(double world_wall_distance);
void			calculate_wall_boundaries(int wall_height, int *wall_start, int *wall_end);

// texture.c
int				screen_pixel_texture_colour(t_texture_context *ctx, int current_pixel_y);

// pixel_buffer.c
void			clear_screen_buffer(void);
void			put_pixel(int x, int y, int color);

//  ================== MAP_GRID_CELL ==================

int	put_cell_from_char(t_map *m, t_player *pl, int idx, char c);

//  ================== MAP_GRID_FILL ==================

int	fill_grid(t_map *m, t_player *pl, char **lines, int *count);

//  ================== MAP_GRID_BUILD ==================

int	build_grid_from_lines(t_map *m, t_player *pl, char **lines,
		int *player_found);

//  ================== MAP_CHECK_FLOOD ==================

void	ffctx_init(t_ffctx *c, const t_map *m, char *vis, int *q);
int	flood_from_start(t_ffctx *c, int start_idx);

//  ================== MAP_CHECK_UTILS ==================

int	idx_2d_to_1d(int x, int y, int w);
int	in_bounds(int x, int y, int w, int h);
void	queue_push(int *q, int *tail, int v);
int	queue_pop(int *q, int *head, int tail, int *out);

//  ================== MAP_CHECK_FLOOD ==================

int	flood_from_border(t_ffctx *c);
int	map_is_closed(const t_map *m);

//  ================== UTILS ==================

void	parse_error(const char *msg);
// void	render_wall_column2(int screen_column_x, t_ray_result *wall_intersection_data,
// 		int projected_wall_height);
// void	put_pixel2(int screen_x, int screen_y, int pixel_color);


double get_delta_time(void);
void rotate_player(double angle);
void try_move_player(double new_x, double new_y);
void handle_input(double dt);
void game_loop_tick(void *param);
void on_close(void *param);

#endif
