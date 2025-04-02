#ifndef RENDER_H
#define RENDER_H

#include "render_init.h"
#include "../include/linmath.h"
#include "../physics/physics.h"
#include <SDL2/SDL_video.h>
SDL_Window * render_init();
void render_end(SDL_Window * win);
void render_object(vec2 pos,vec2 size,u32 nElement);
void render_aabb(AABB *aabb);
void render_begin();

#endif
