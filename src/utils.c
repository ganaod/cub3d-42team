/* ************************************************************************** */
/*                                                                            */
/*   utils.c - Helper Functions & Utility Operations                        */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void	parse_error(const char *msg)
{
	write(2, "Error: ", 7);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
}
