#ifndef CUB3D_H
# define CUB3D_H

/* system includes */
# include <unistd.h>		// POSIX sys calls
# include <stdlib.h>		// mem allocatn
# include <stdio.h>
# include <fcntl.h>			// file ctrl ops
# include <math.h>
// # include "lib/minilibx-macOS/mlx.h"		// MiniLibX graphics lib
# include "../lib/MLX42/include/MLX42/MLX42.h"
# include "../lib/libft/libft/libft.h"
# include "../lib/libft/gnl/get_next_line.h"
# include "../lib/libft/ft_printf/ft_printf.h"

/* program constants */
# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1
// # define BUFFER_SIZE 1024

# define HDR_NO (1<<0)
# define HDR_SO (1<<1)
# define HDR_WE (1<<2)
# define HDR_EA (1<<3)
# define HDR_F  (1<<4)
# define HDR_C  (1<<5)


/* map cell types
the algorithm casts rays until hitting WALL,
treats EMPTY as traversable,
and VOID handles map parsing edge cases
where spaces might exist beyond defined boundaries. */
# define EMPTY 0		// walkable space
# define WALL 1			// collision / rendering surface
# define VOID 2			// invalid map space (outside boundaries)

/* wall face directions */
# define NORTH 0
# define SOUTH 1
# define WEST 2
# define EAST 3

/* rendering constants */
# define DEFAULT_WIDTH 1024		// screen sampling resolution
# define DEFAULT_HEIGHT 768		// vertical pixel count
# define FOV 0.66				// field of view / radians					- PLAY AROUND WITH THESE

/* DATA STRUCTURES:
Minimal data representation for core game functionality */

/* PLAYER STATE */
typedef struct s_player
{
    double pos_x;           // World position X
    double pos_y;           // World position Y
    double dir_x;           // View direction X
    double dir_y;           // View direction Y
    double camera_plane_x;  // Camera plane X (FOV)
    double camera_plane_y;  // Camera plane Y (FOV)
} t_player;

/* MAP CONFIGURATION */
typedef struct s_map
{
    int *grid;              // 2D map as 1D array
    int width;              // Map width in cells
    int height;             // Map height in cells
    char *texture_paths[4]; // Wall texture file paths
    uint32_t floor_color;   // 0xRRGGBB
    uint32_t ceiling_color; // 0xRRGGBB
    char *first_map_line;
} t_map;

/* GRAPHICS SYSTEM */
typedef struct s_graphics
{
    mlx_t       *mlx;        // MLX42 Kontext
    mlx_image_t *frame;      // Framebuffer (WIN_W x WIN_H, RGBA8888)
    int         screen_width;
    int         screen_height;
} t_graphics;

typedef struct s_texinfo
{
	char		**slot;
	const char	*line;
	int			j;
	int			bit;
	int			*flags;
}				t_texinfo;

/* GAME STATE
type def: defines what a game state looks like
blueprint/template */
typedef struct s_game
{
    t_player player;        // Player state
    t_map map;              // Map data
    t_graphics graphics;    // Graphics system
    int running;            // Game loop flag
    double movement_speed;  // Player movement speed
    double rotation_speed;  // Camera rotation speed
} t_game;

/* GLOBAL GAME STATE
var declaration: declares that somewhere exists a specific instance
of t_game that all files can access */
extern t_game g_game;

/* FUNCTION DECLARATIONS:
	Organize by
	source file responsibility
	program flow? */
// ADD WHEN STARTING TO BUILD PROJECT ELSE TOO FREQUENT CHANGES


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


#endif
