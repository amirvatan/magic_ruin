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
void render_set_batch_texture(u32 texture_id);
void render_sprite_sheet_frame(Sprite_Sheet *sprite_sheet, f32 row, f32 column,
                               vec2 position); 
 void render_sprite_sheet_init(Sprite_Sheet *sprite_sheet, char *path,
                              f32 cell_width, f32 cell_height);
 
#endif
