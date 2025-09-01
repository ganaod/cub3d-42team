/* ************************************************************************** */
/*                                                                            */
/*   utils.c - Helper Functions & Utility Operations                        */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
int ft_strlen(char *str)
{
    int len = 0;
    if (!str)
        return (0);
    while (str[len])
        len++;
    return (len);
}

char *ft_strdup(char *src)
{
    int len = ft_strlen(src);
    char *dst = malloc(len + 1);
    int i = 0;
    
    if (!dst)
        return (NULL);
    while (i < len)
    {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
    return (dst);
}

int ft_strcmp(char *s1, char *s2)
{
    int i = 0;
    while (s1[i] && s2[i] && s1[i] == s2[i])
        i++;
    return (s1[i] - s2[i]);
}

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

/*
PARSING UTILITIES:
Helper functions for configuration file parsing.
*/
int ft_atoi(char *str)
{
    int result = 0;
    int sign = 1;
    int i = 0;
    
    // Skip whitespace
    while (str[i] == ' ' || str[i] == '\t')
        i++;
    
    // Handle sign
    if (str[i] == '-')
    {
        sign = -1;
        i++;
    }
    else if (str[i] == '+')
        i++;
    
    // Convert digits
    while (str[i] >= '0' && str[i] <= '9')
    {
        result = result * 10 + (str[i] - '0');
        i++;
    }
    
    return (result * sign);
}

char **ft_split(char *str, char separator)
{
    // Count words separated by separator
    int word_count = count_words(str, separator);
    
    // Allocate array for word pointers
    char **result = safe_malloc(sizeof(char*) * (word_count + 1));
    
    // Extract each word
    int word_index = 0;
    int i = 0;
    
    while (str[i] && word_index < word_count)
    {
        // Skip separators
        while (str[i] == separator)
            i++;
        
        // Find end of current word
        int word_start = i;
        while (str[i] && str[i] != separator)
            i++;
        
        // Extract word
        int word_len = i - word_start;
        result[word_index] = safe_malloc(word_len + 1);
        ft_strncpy(result[word_index], str + word_start, word_len);
        result[word_index][word_len] = '\0';
        
        word_index++;
    }
    
    result[word_index] = NULL;
    return (result);
}