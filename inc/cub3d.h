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

/* program constants */
# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1
# define BUFFER_SIZE 1024


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
} t_map;

/* GRAPHICS SYSTEM */
typedef struct s_graphics
{
    mlx_t       *mlx;        // MLX42 Kontext
    mlx_image_t *frame;      // Framebuffer (WIN_W x WIN_H, RGBA8888)
    int         screen_width;
    int         screen_height;
} t_graphics;


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



#endif
