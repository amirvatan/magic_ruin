#include "engine/include/linmath.h"
#include "engine/physics/physics.h"
#include "engine/render/render.h"
#include "engine/render/render_init.h"
#include "engine/render/render_util.h"
#include "engine/time/time.h"
#include "engine/types.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void keyboard(bool *keys) {
  SDL_Event e;
  while (SDL_PollEvent(&e) != 0) {
    switch (e.type) {
    case SDL_KEYDOWN: {
      keys[e.key.keysym.sym > 127 ? 128 : e.key.keysym.sym] = true;
      break;
    }
    case SDL_KEYUP: {
      keys[e.key.keysym.sym > 127 ? 128 : e.key.keysym.sym] = false;
      break;
    }
    default: {
      break;
    }
    }
  }
}
void input_processing(bool *keys, Body *player) {
  f32 velx = player->velocity[0];
  f32 vely = player->velocity[1];
  if (keys[SDLK_d]) {
    velx = 500;
  } else if (keys[SDLK_a]) {

    velx = -500;

  } else if (keys[SDLK_w]) {
    vely = 500;
  } else if (keys[SDLK_s]) {
    vely = -500;
  } else if (keys[128]) {
    exit(1);
  }
  player->velocity[0] = velx;
  player->velocity[1] = vely;
}

int main() {
  bool KEYS[322] = {0};
  SDL_Window *win;
  bool exit = 0;
  win = render_init();
  i32 width, height;
  SDL_GetWindowSize(win, &width, &height);
  f32 frame_delay;
  f32 time_last = 0;
  f32 delta;
  d_array objects;
  d_array static_objects;
  init_physics(&objects, &static_objects);
  time_init(60, &frame_delay);
  create_physics_body(&objects, (vec2){width * 0.5, height * 0.5},
                      (vec2){500, 500});
  AABB aabb = {.position = {150, 150}, .half_size = {150, 150}};
  create_physics_static_body(&static_objects, aabb.position, aabb.half_size);

  u32 texture = render_texture("./texture/container.jpg");
  while (!exit) {
    time_update(&time_last, &delta);
    keyboard(KEYS);
    Body *body = get_physics_body(&objects, 0);
    Static_Body *static_body = get_physics_static_body(&static_objects, 0);
    input_processing(KEYS, body);
    update_physics(&objects, &static_objects, delta);
    render_begin();

    render_aabb(&body->aabb);
    render_aabb(&static_body->aabb);
    SDL_GL_SwapWindow(win);
    time_update_late(&frame_delay, &time_last);
  }
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}
