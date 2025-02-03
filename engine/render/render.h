#ifndef RENDER_H
#define RENDER_H

#include "render_init.h"
#include "../include/linmath.h"

void render_init(SDL_Window * win);
void render_end(SDL_Window * win);
void render_triangle(u32 * vao,u32 * program);
void render_quad(vec2 pos,vec2 size,u32 * vao,u32 * program);
void render_begin();

#endif
