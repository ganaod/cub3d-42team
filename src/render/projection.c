/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   projection.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 11:18:50 by go-donne          #+#    #+#             */
/*   Updated: 2025/09/07 11:25:43 by go-donne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// convert world distances to screen pixel heights

#include "../inc/cub3d.h"

int calculate_screen_wall_height(double wall_distance);
void calculate_wall_boundaries(int wall_height, int *wall_start, int *wall_end);