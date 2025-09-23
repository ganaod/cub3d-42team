/* information flow matches dependency hierarchy:
mathematical foundation → domain concepts
→ system interfaces → computational processes
→ orchestration	*/
#ifndef CUB3D_H
# define CUB3D_H

// ⭐⭐ INCLUDES
// system
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>

// math
# include <math.h>

// external
# include "../lib/MLX42/include/MLX42/MLX42.h"
# include "../lib/libft/libft/libft.h"
# include "../lib/libft/gnl/get_next_line.h"
# include "../lib/libft/ft_printf/ft_printf.h"

/* spaces & their coordinate conventions, relationships

top-left origin (0,0), x → right, y → down
top-down view (world / map)


world space (continuous floats)       map grid (discrete ints)
+--------------------+                +--------------------+
|                    |                | 0,0   1,0   2,0    |
|   p (player)       |                | 0,1   1,1   2,1    |
|    (3.5,2.2)       |                | 0,2   1,2   2,2    |
|                    |                |                    |
+--------------------+                +--------------------+


screen space (pixels, top-left origin)
+---------------------------+
| screen_y=0                |
|                           |
| screen_y=1                |
| ...                       |
| screen_y=height-1         |
+---------------------------+
x → screen_x


. world space: floating-point coordinates, continuous
. map grid: integer indices, (map_x, map_y)
. screen space: pixels, (screen_x, screen_y), origin top-left
. y increases downward in all spaces	*/

// ⭐⭐ DEFINES
// math constants, form geometric framework
// ⭐ SYSTEM ARCHITECTURE
// framebuffer display resolution
# define DEFAULT_WIDTH					1024
# define DEFAULT_HEIGHT					768

// exit codes
# define EXIT_SUCCESS					0
# define EXIT_FAILURE					1

// ⭐ MATHEMATICAL FOUNDATION
// (fixed) field of view (FoV) configuration
# define FOV_DEGREES					60.0
// good perspective without excessive distortion

# define FOV_RADIANS					(FOV_DEGREES * (M_PI / 180.0))
// convert FOV from degrees > radians for C's math fns

# define FOV_HALF_ANGLE_RADIANS			(FOV_RADIANS / 2.0)
// half-angle needed for tangent calculation

# define FOV_CAMERA_PLANE_MAGNITUDE		tan(FOV_HALF_ANGLE_RADIANS)
// camera plane vector magnitude for desired FOV
// this value determines angular spread of vision

// SCREEN > FOV COORDINATE TRANSFORMATION
# define SCREEN_TO_FOV_SCALE_FACTOR		2.0
// Scales normalized screen coordinates [0,1] to [0,2]

# define FOV_CENTER_OFFSET				1.0
// shifts scaled coordinates [0,2] to FOV space [-1,+1]

// ⭐ WORLD DOMAIN PRIMITIVES
// map cell types
# define CELL_VOID						(-1)  /* ' '  -> außerhalb/ungefüllt */
# define CELL_EMPTY						(0)   /* '0'  -> begehbar */
# define CELL_WALL						(1)   /* '1'  -> Wand  */

// wall face directions: absolute world coords
# define WALL_NORTH						0
# define WALL_SOUTH						1
# define WALL_WEST						2
# define WALL_EAST						3

// player direction codes: relative to actor orientation
# define DIR_N							0
# define DIR_S							1
# define DIR_W							2
# define DIR_E							3

// ⭐ PARSER STATE FLAGS
// HDR == header?
# define HDR_NO							(1<<0)
# define HDR_SO							(1<<1)
# define HDR_WE							(1<<2)
# define HDR_EA							(1<<3)
# define HDR_F							(1<<4)
# define HDR_C							(1<<5)

// ⭐⭐ STRUCTS
// PRIMITIVE DATA CONTAINERS
typedef struct s_texture_image
{
	mlx_image_t		*mlx_image;				// mlx42 image pointer
	uint32_t		*pixels;				// pixel buffer
	int				image_width;			// image width
	int				image_height;			// image height
}					t_texture_image;

// ⭐ WORLD ENTITY REPRESENTATION
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
	int				*grid;					// 1D grid, size = width*height
	int				width;
	int				height;
	char			*texture_paths[4];
	uint32_t		floor_color;
	uint32_t		ceiling_color;
	char			*first_map_line;
	t_texture_image	wall_textures[4];		// <— KEINE Pointer: struct-Array erwartet
}					t_map;

// ⭐ SYSTEM INTERFACES
// graphics system
typedef struct s_graphics
{
	mlx_t			*mlx;			// mlx42 context
	mlx_image_t		*frame;			// framebuffer (win_w x win_h, rgba8888)
	int				screen_width;    // window width
    int				screen_height;    // window height
}					t_graphics;

// ⭐ UTILITIES
typedef struct s_ffctx
{
	const t_map		*m;		// map + grid
	char			*vis;	// visited flags, size w*h
	int				*q;		// queue array, size w*h
	int				w;		// width
	int				h;		// height
	int				head;	// queue head
	int				tail;	// queue tail
}					t_ffctx;

// parser contexts
typedef struct s_texinfo
{
	char			**slot;		// texture slot
	const char		*line;		// config line
	int				j;			// index
	int				bit;		// bit flag
	int				*flags;		// pointer to flags
}					t_texinfo;

// ⭐ ORCHESTRATION
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

// ⭐⭐ FUNCTION PROTOTYPES
//  ================== PARSE_UTILS ==================

int		skip_ws(const char *s, int i);
int		parse_u8_component(const char *s, int *i, int *out);
int		expect_comma(const char *s, int *i);
int		parse_rgb_triplet(const char *s, uint32_t *out_rgb);

//  ================== PARSE_HEADER ==================

int		set_texture_field(char **dst_path, const char *after_key);
int		set_color_field(uint32_t *dst_rgb, const char *after_key, int *was_set);
int		parse_header_line(t_map *m, const char *line, int *flags);
int		match2(const char *s, int i, char a, char b);
int		set_tex_entry(char **slot, const char *after_key, int bit, int *flags);
int		parse_header_texture(t_map *m, const char *line, int i, int *flags);
int		parse_header_color(t_map *m, const char *line, int i, int *flags);

//  ================== PARSE_HEADER LINES ==================

int		parse_header_lines(t_map *m, int fd);
int		check_texture_paths_exist(const t_map *m);

//  ================== COLLECT_MAP_LINES ==================

int		collect_map_lines(t_map *m, int fd, char ***out_lines, int *out_h);
int		append_line(char ***lines_ptr, int *cap_ptr, int *h_ptr, char *take);
int		validate_map_line(const char *s);
void	rstrip_eol(char *s);

//  ================== NORMALIZE_MAP ==================

int		normalize_map(char ***lines_io, int h, int *out_w);

//  ================== MAP_GRID_CELL ==================

int		put_cell_from_char(t_map *m, t_player *pl, int idx, char c);

//  ================== MAP_GRID_FILL ==================

int		fill_grid(t_map *m, t_player *pl, char **lines, int *count);

//  ================== MAP_GRID_BUILD ==================

int		build_grid_from_lines(t_map *m, t_player *pl, char **lines,
				int *player_found);

//  ================== MAP_CHECK_FLOOD ==================

void	ffctx_init(t_ffctx *c, const t_map *m, char *vis, int *q);
int		flood_from_start(t_ffctx *c, int start_idx);

//  ================== MAP_CHECK_UTILS ==================

int		idx_2d_to_1d(int x, int y, int w);
int		in_bounds(int x, int y, int w, int h);
void	queue_push(int *q, int *tail, int v);
int		queue_pop(int *q, int *head, int tail, int *out);

//  ================== MAP_CHECK_FLOOD ==================

int		flood_from_border(t_ffctx *c);
int		map_is_closed(const t_map *m);

//  ================== GAME_LOOP ==================

double	get_delta_time(void);
void	rotate_player(double angle);
void	try_move_player(double new_x, double new_y);
void	handle_input(double dt);
void	game_loop_tick(void *param);
void	on_close(void *param);
void	handle_exit_input(void);
void	handle_rotation_input(double ang);
int		map_cell(const t_map *m, int ix, int iy);

//  ================== UTILS ==================

void	parse_error(const char *msg);

//  ================== GRAPHICS / WINDOW ==================

int   gfx_open_window_resizable(t_game *game, const char *title, int w, int h);
int   gfx_maximize_now(t_graphics *g);
int   gfx_rebuild_framebuffer(t_graphics *g, int w, int h);
void  handle_resize(int32_t w, int32_t h, void *param);
int   gfx_rebuild_framebuffer(t_graphics *g, int w, int h);
void  handle_resize(int32_t w, int32_t h, void *param);



#endif
