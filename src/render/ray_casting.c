/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_casting.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 15:48:41 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/05 15:49:04 by go-donne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// ray geometric ops

#include "../inc/cub3d.h"

/* create peripheral vision
transform raycaster from 1D scanner > 3D vision simulator

transform each screen col > ray dir vector (viewing angle)
visual result: natural FOV instead of "tunnel vision"

without: all cols show same straight-ahead view (telescope effect)
with: l/r cols show angled views (human-like peripheral vision)

dependencies: g_game.player (pos/dir),
			g_game_graphics (screen width)


steps:

1. screen pixel > FOV pos
. leftmost pixel (0) becomes -1 (left edge of vision)
. center pixel becomes 0 (straight ahead)
. rightmost pixel becomes +1 (right edge of vision)
normalisation formula for range mapping:
	. scale to [0,2]
	. shift to [-1,+1]

2. calculate actual ray direction for this viewing angle
. base dir (where player faces) + angular offset (l/r tilt)
. visual: creates "sweep" of vision from left peripheral > right peripheral 

vector addition with scaling */
void	calculate_ray_direction(int screen_x, double *ray_dir_x, double *ray_dir_y)
{
	double	camera_x;

	camera_x = 2.0 * screen_x / (double)g_game.graphics.screen_width - 1.0;
	*ray_dir_x = g_game.player.dir_x + camera_x * g_game.player.camera_plane_x;
	*ray_dir_y = g_game.player.dir_y + camera_x * g_game.player.camera_plane_y;
}


int get_wall_face_hit(t_dda_state *state, int wall_side);