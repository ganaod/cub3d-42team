/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_texture.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: blohrer <blohrer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 13:28:19 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/23 13:51:32 by blohrer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static int	tex_png_open_and_meta(const char *path, t_texture_image *out,
		mlx_texture_t **out_tex)
{
	mlx_texture_t	*t;

	if (!path || !out || !out_tex)
		return (0);
	t = mlx_load_png(path);
	if (!t)
	{
		ft_printf("ERROR: mlx_load_png failed for '%s'\n", path);
		return (0);
	}
	out->image_width = (int)t->width;
	out->image_height = (int)t->height;
	out->mlx_image = NULL;
	*out_tex = t;
	return (1);
}

static int	tex_png_copy_pixels_argb_free(mlx_texture_t *t,
		t_texture_image *out)
{
	int	i;
	int	count;

	if (!t || !out || !out->pixels)
		return (0);
	count = out->image_width * out->image_height;
	i = 0;
	while (i < count)
	{
		out->pixels[i] = ((uint32_t)t->pixels[(i << 2)
				+ 3] << 24) | ((uint32_t)t->pixels[(i << 2)
				+ 0] << 16) | ((uint32_t)t->pixels[(i << 2)
				+ 1] << 8) | (uint32_t)t->pixels[(i << 2) + 2];
		i++;
	}
	mlx_delete_texture(t);
	return (1);
}

static int	tex_png_alloc_pixels(t_texture_image *out, const char *path)
{
	int	count;

	if (!out)
		return (0);
	count = out->image_width * out->image_height;
	if (count <= 0)
	{
		ft_printf("ERROR: invalid texture size for '%s'\n", path);
		return (0);
	}
	out->pixels = (uint32_t *)malloc((unsigned long)count * 4);
	if (!out->pixels)
	{
		ft_printf("ERROR: malloc texture pixels failed for '%s'\n", path);
		return (0);
	}
	return (1);
}

int	load_one_texture_png(const char *path, t_texture_image *out)
{
	mlx_texture_t	*t;

	if (!tex_png_open_and_meta(path, out, &t))
		return (0);
	if (!tex_png_alloc_pixels(out, path))
	{
		mlx_delete_texture(t);
		return (0);
	}
	if (!tex_png_copy_pixels_argb_free(t, out))
	{
		free(out->pixels);
		out->pixels = NULL;
		return (0);
	}
	return (1);
}

int	load_all_wall_textures(t_map *m)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (!m->texture_paths[i] || !*m->texture_paths[i])
		{
			ft_printf("ERROR: missing texture path for face %d\n", i);
			return (0);
		}
		i++;
	}
	if (!load_one_texture_png(m->texture_paths[WALL_NORTH],
			&m->wall_textures[WALL_NORTH]))
		return (0);
	if (!load_one_texture_png(m->texture_paths[WALL_SOUTH],
			&m->wall_textures[WALL_SOUTH]))
		return (0);
	if (!load_one_texture_png(m->texture_paths[WALL_WEST],
			&m->wall_textures[WALL_WEST]))
		return (0);
	if (!load_one_texture_png(m->texture_paths[WALL_EAST],
			&m->wall_textures[WALL_EAST]))
		return (0);
	return (1);
}
