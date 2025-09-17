/* ************************************************************************** */
/*                                                                            */
/*   utils.c - Helper Functions & Utility Operations                        */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

/*
UPSTREAM RESPONSIBILITY:
- Provide fundamental string and memory operations
- Handle file I/O operations safely
- Implement mathematical helper functions
- Manage error handling utilities
*/

/*
STRING OPERATIONS:
Basic string manipulation without external dependencies.
Required for parsing configuration files.
*/

/*
FILE OPERATIONS:
Safe file handling with error checking.
*/
int open_file_safely(char *filename)
{
    int fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        write(STDERR_FILENO, "Error: Cannot open file\n", 24);
        return (-1);
    }
    return (fd);
}

int validate_file_extension(char *filename, char *expected_ext)
{
    int filename_len = ft_strlen(filename);
    int ext_len = ft_strlen(expected_ext);

    if (filename_len < ext_len)
        return (0);

    // Check if filename ends with expected extension
    char *file_ext = filename + (filename_len - ext_len);
    return (ft_strcmp(file_ext, expected_ext) == 0);
}

/*
MATHEMATICAL UTILITIES:
Essential calculations for raycasting and graphics.
*/
double calculate_distance(double x1, double y1, double x2, double y2)
{
    double dx = x2 - x1;
    double dy = y2 - y1;
    return (sqrt(dx * dx + dy * dy));
}

double normalize_angle(double angle)
{
    while (angle < 0)
        angle += 2 * M_PI;
    while (angle >= 2 * M_PI)
        angle -= 2 * M_PI;
    return (angle);
}

/*
MEMORY MANAGEMENT:
Safe allocation and deallocation patterns.
*/
void *safe_malloc(size_t size)
{
    void *ptr = malloc(size);
    if (!ptr)
    {
        write(STDERR_FILENO, "Error: Memory allocation failed\n", 32);
        exit(EXIT_FAILURE);
    }
    return (ptr);
}

void safe_free(void **ptr)
{
    if (ptr && *ptr)
    {
        free(*ptr);
        *ptr = NULL;
    }
}

/*
COLOR OPERATIONS:
RGB color manipulation for rendering.
*/
int create_rgb_color(int red, int green, int blue)
{
    return ((red << 16) | (green << 8) | blue);
}

int extract_red(int color)
{
    return ((color >> 16) & 0xFF);
}

int extract_green(int color)
{
    return ((color >> 8) & 0xFF);
}

int extract_blue(int color)
{
    return (color & 0xFF);
}

void	parse_error(const char *msg)
{
	write(2, "Error: ", 7);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
}

void	render_wall_column2(int screen_column_x, t_ray_result *wall_intersection_data,
		int projected_wall_height)
{
	int	screen_wall_start_y_pixel;
	int	screen_wall_end_y_pixel;

	// Geklammert berechnen (nutze die bereits vorhandene Funktion!)
	calculate_wall_boundaries(projected_wall_height,
		&screen_wall_start_y_pixel, &screen_wall_end_y_pixel);

	// Render drei Sektionen mit den GECLAMPTEN Grenzen
	render_ceiling_section(screen_column_x, screen_wall_start_y_pixel);
	render_wall_section(screen_column_x, screen_wall_start_y_pixel,
		screen_wall_end_y_pixel, wall_intersection_data);
	render_floor_section(screen_column_x, screen_wall_end_y_pixel);
}

// void	put_pixel2(int screen_x, int screen_y, int pixel_color)
// {
// 	int w = g_game.graphics.screen_width;
// 	int h = g_game.graphics.screen_height;

// 	if (screen_x < 0 || screen_x >= w || screen_y < 0 || screen_y >= h)
// 		return; // out of bounds -> nicht schreiben

// 	int screen_pixel_index = screen_y * w + screen_x;
// 	g_game.graphics.frame->pixels[screen_pixel_index] = pixel_color;
// }


// with more bounds checking:
void put_pixel2(int screen_x, int screen_y, int pixel_color)
{
    if (screen_x < 0 || screen_x >= (int)g_game.graphics.frame->width || 
        screen_y < 0 || screen_y >= (int)g_game.graphics.frame->height)
        return;
    
    int screen_pixel_index = screen_y * (int)g_game.graphics.frame->width + screen_x;
    g_game.graphics.frame->pixels[screen_pixel_index] = pixel_color;
}



void	render_wall_section2(int screen_column_x, int wall_start_y_pixel,
						int wall_end_y_pixel, t_ray_result *wall_hit_data)
{
	int					current_pixel_y;
	uint32_t			pixel_color;
	t_texture_context	texture_mapping_info;


	texture_mapping_info.world_wall_face = wall_hit_data->world_wall_face;
	texture_mapping_info.world_wall_intersection_x = wall_hit_data->world_intersection_x;
	texture_mapping_info.world_wall_intersection_y = wall_hit_data->world_intersection_y;
	texture_mapping_info.screen_wall_height = wall_end_y_pixel - wall_start_y_pixel;
	current_pixel_y = wall_start_y_pixel;
	while (current_pixel_y < wall_end_y_pixel)
	{
		pixel_color = screen_pixel_texture_colour(&texture_mapping_info,
						current_pixel_y);
		put_pixel2(screen_column_x, current_pixel_y, pixel_color);
		current_pixel_y++;
	}
}
