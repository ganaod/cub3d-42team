## math.c

Parametric line intersection with discrete grid
ray casting & DDA algos

## projection.c

3D world coordinates → 2D screen coordinates
screen space projection calculations

## texture.c

Surface parameterization and texture sampling
texture mapping & sampling

## screen_buffer.c

Rasterization and pixel buffer operations
direct screen buffer ops



Justification for separation:
Ray Math: Pure geometric algorithms - grid traversal, intersection testing
Projection: Perspective mathematics - similar triangles, distance scaling
Texture: Surface parameterization - UV coordinate mapping, image sampling
Screen Buffer: Direct memory operations - pixel manipulation, buffer management
Each file encapsulates one mathematical domain with minimal cross-dependencies

Dependency Flow: 
ray_math.c → projection.c → texture.c → screen_buffer.c