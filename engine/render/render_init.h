#ifndef RENDER_INIT_H
#define RENDER_INIT_H
#include "../include/gl.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include "../types.h"

SDL_Window * render_init_window(u32 width,u32 height);
void render_init_triangle(u32 * vao,u32 * vbo,u32 * ebo);
void render_init_quad(u32 * vao,u32 * vbo,u32 * ebo);

#endif
