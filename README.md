# Cub3D: Ray-Casting Engine

## Context

[42 Network](https://www.42network.org/): [software engineering curriculum](https://42.fr/en/the-program/software-engineer-degree/)

## Objective

Introduction to graphics rendering.

Computational geometry meets real-time rendering: 
[ray-casting](https://en.wikipedia.org/wiki/Ray_casting) projection of 2D maze topology 
into dynamic 1st-person 2.5D perspective.

Inspired by *Wolfenstein 3D* (id Software, 1992)—foundational ray-casting FPS.

## Implementation

**Language**: C  

**Graphics**: [OpenGL](https://www.opengl.org/) via [MLX42](https://github.com/codam-coding-college/MLX42) (developed at [Codam Coding College](https://www.codam.nl/en/))

## Architecture

**Benjamin Lohrer**: Infrastructure: parsing, game loop

**Ganesha Hugh-Roe O'Donnell**: Rendering module: ray-casting engine 
via Digital Differential Analyser (DDA) algorithm, texture creation
