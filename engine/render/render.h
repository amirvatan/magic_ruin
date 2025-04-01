#ifndef RENDER_H
#define RENDER_H

#include "render_init.h"
#include "../include/linmath.h"
#include "../physics/physics.h"
void render_init(SDL_Window * win);
void render_end(SDL_Window * win);
void render_object(vec2 pos,vec2 size,u32 * vao,u32 * program,u32 nElement);
void render_aabb(AABB *aabb,u32 * voa,u32* program);
void render_begin();

#endif
