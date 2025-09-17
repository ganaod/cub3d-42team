/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: go-donne <go-donne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 13:08:21 by blohrer           #+#    #+#             */
/*   Updated: 2025/09/17 15:28:27 by go-donne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

double get_delta_time(void)
{
    // aktuelle Zeit abrufen
    g_game.time_now = mlx_get_time();

    // Differenz zum letzten Frame
    double dt = g_game.time_now - g_game.time_prev;

    // für Stabilität clampen (z. B. beim Breakpoint)
    if (dt < 0.0) dt = 0.0;
    if (dt > 0.05) dt = 0.05; // max ~50 ms

    // "letzte Zeit" aktualisieren
    g_game.time_prev = g_game.time_now;

    return dt;
}
static int map_cell(const t_map *m, int ix, int iy)
{
    if (!m || !m->grid) return CELL_WALL;
    if (ix < 0 || iy < 0 || ix >= m->width || iy >= m->height)
        return CELL_WALL; // außerhalb behandeln wir als Wand
    return m->grid[iy * m->width + ix];
}

static int is_walkable(const t_map *m, double x, double y)
{
    int ix = (int)x;
    int iy = (int)y;

    // nur leere Zellen sind begehbar; VOID und WALL nicht
    int c = map_cell(m, ix, iy);
    return (c == CELL_EMPTY);
}

void try_move_player(double new_x, double new_y)
{
    // kleiner Radius, damit man nicht in Ecken hängen bleibt
    // (hier einfache Variante: Achsen getrennt testen)
    // double cur_x = g_game.player.world_pos_x;
    double cur_y = g_game.player.world_pos_y;

    // X separat
    if (is_walkable(&g_game.map, new_x, cur_y))
        g_game.player.world_pos_x = new_x;

    // Y separat
    if (is_walkable(&g_game.map, g_game.player.world_pos_x, new_y))
        g_game.player.world_pos_y = new_y;
}

void rotate_player(double angle)
{
    double old_dir_x = g_game.player.world_dir_x;
    double old_plane_x = g_game.player.world_camera_plane_x;

    // Richtung drehen
    g_game.player.world_dir_x = old_dir_x * cos(angle) - g_game.player.world_dir_y * sin(angle);
    g_game.player.world_dir_y = old_dir_x * sin(angle) + g_game.player.world_dir_y * cos(angle);

    // Kamera-Ebene drehen (orthogonal zur Blickrichtung)
    g_game.player.world_camera_plane_x = old_plane_x * cos(angle) - g_game.player.world_camera_plane_y * sin(angle);
    g_game.player.world_camera_plane_y = old_plane_x * sin(angle) + g_game.player.world_camera_plane_y * cos(angle);
}

void handle_input(double dt)
{
    mlx_t *mlx = g_game.graphics.mlx;

    // Beenden mit ESC
    if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE))
        mlx_close_window(mlx);

    double move = g_game.movement_speed;   // Einheiten pro Sekunde (z. B. 3.0)
    double rot  = g_game.rotation_speed;   // Radiant pro Sekunde (z. B. 2.0)
    double step = move * dt;
    double ang  = rot  * dt;

    double px = g_game.player.world_pos_x;
    double py = g_game.player.world_pos_y;
    double dx = g_game.player.world_dir_x;
    double dy = g_game.player.world_dir_y;
    double pxn, pyn;

    // Vor/Zurück (W/S)
    if (mlx_is_key_down(mlx, MLX_KEY_W)) {
        pxn = px + dx * step;
        pyn = py + dy * step;
        try_move_player(pxn, pyn);
    }
    if (mlx_is_key_down(mlx, MLX_KEY_S)) {
        pxn = px - dx * step;
        pyn = py - dy * step;
        try_move_player(pxn, pyn);
    }

    // Strafe links/rechts (A/D) entlang Kameraebene
    double plx = g_game.player.world_camera_plane_x;
    double ply = g_game.player.world_camera_plane_y;

    if (mlx_is_key_down(mlx, MLX_KEY_A)) {
        pxn = px - plx * step;
        pyn = py - ply * step;
        try_move_player(pxn, pyn);
    }
    if (mlx_is_key_down(mlx, MLX_KEY_D)) {
        pxn = px + plx * step;
        pyn = py + ply * step;
        try_move_player(pxn, pyn);
    }

    // Rotation (Q/E oder ←/→)
    if (mlx_is_key_down(mlx, MLX_KEY_Q) || mlx_is_key_down(mlx, MLX_KEY_LEFT))
        rotate_player(-ang);
    if (mlx_is_key_down(mlx, MLX_KEY_E) || mlx_is_key_down(mlx, MLX_KEY_RIGHT))
        rotate_player(+ang);
}

void on_close(void *param)
{
    (void)param;
    g_game.running = 0;
    // Fenster explizit schließen (optional, falls nicht schon durch OS passiert)
    mlx_close_window(g_game.graphics.mlx);
}

void game_loop_tick(void *param)
{
    (void)param;
    if (!g_game.running)
        return;

    // 1) Delta-Time
    double dt = get_delta_time();

    // 2) Input + Movement
    handle_input(dt);

    // 3) Render (zeichnet in g_game.graphics.frame)
    render_complete_frame();
}
