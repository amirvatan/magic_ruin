#ifndef RENDER_INIT_H
#define RENDER_INIT_H
#include "../include/gl.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include "../types.h"
#include "../include/linmath.h"

typedef struct sprite_sheet {
	f32 width;
	f32 height;
	f32 cell_width;
	f32 cell_height;
	u32 texture_id;
} Sprite_Sheet;
typedef struct batch_vertex {
	vec2 position;
	vec2 uvs;
} Batch_Vertex;

enum {
	MAX_BATCH_QUADS = 10000,
	MAX_BATCH_VERTICES = MAX_BATCH_QUADS * 4,
	MAX_BATCH_ELEMENTS = MAX_BATCH_QUADS * 6,
};
SDL_Window * render_init_window(u32 width,u32 height);
void render_init_triangle(u32 * vao,u32 * vbo,u32 * ebo);
void render_init_quad(u32 * vao,u32 * vbo,u32 * ebo);
void render_init_batch_quads(u32 * vao,u32 * vbo,u32 * ebo);
void render_init_shaders(u32 * shader_default,u32 * shader_batch,f32 render_width,f32 render_height);

#endif
