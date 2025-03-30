#include "engine/physics/physics.h"
#include "engine/render/render.h"
#include "engine/render/render_init.h"
#include "engine/render/render_util.h"
#include "engine/time/time.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define EULER_NUMBER_F 2.71828182846

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
void input_processing(bool *keys, vec2 pos, f32 delta) {

  if (keys[SDLK_d]) {
    pos[0] += 500 * delta;
  } else if (keys[SDLK_a]) {
    pos[0] -= 500 * delta;
  } else if (keys[SDLK_w]) {
    pos[1] += 500 * delta;
  } else if (keys[SDLK_s]) {
    pos[1] -= 500 * delta;
  } else if (keys[128]) {
    exit(1);
  }
}

int main() {
  bool KEYS[322] = {0};
  SDL_Window *win;
  bool exit = 0;
  //  render_init(win);
  int width = 800;
  int height = 600;
  win = render_init_window(width, height);
  f32 frame_delay;
  f32 time_last = 0;
  f32 delta;
  d_array objects;
  init_physics(&objects);
  time_init(60, &frame_delay);
  u32 vao;
  u32 vbo;
  u32 ebo;
  vec2 pos = {width * 0.5, height * 0.5};
  vec2 size = {100, 100};
  // render_init_triangle(&vao, &vbo, &ebo);
  render_init_quad(&vao, &vbo, &ebo);
  for (int i = 0; i < 5; i++) {
    f32 x = rand() % width;
    f32 y = rand() % height;
    vec2 p = {x, y};
    create_physics_body(&objects, p, (vec2){(rand() % 200), (rand() % 200)});
    Body *body = (Body *)head_array(&objects);
    body->acceleration[0] = (rand() % 200) - 100;
    body->acceleration[1] = (rand() % 200) - 100;
  }
  for (int i = 0; i < objects.index; i++) {
    Body *body = get_physics_body(&objects, i);
    printf("%f\n", body->aabb.position[0]);
    printf("%f\n", body->aabb.position[1]);
    printf("=====================\n");
  }

  u32 program = render_shader("shaders/default.vert", "shaders/default.frag");
  u32 texture = render_texture("./texture/container.jpg");
  while (!exit) {
    time_update(&time_last, &delta);
    keyboard(KEYS);
    input_processing(KEYS, pos, delta);
    render_begin();
    glBindTexture(GL_TEXTURE_BINDING_2D, texture);
    render_object(pos, size, &vao, &program, 6);
    update_physics(&objects, delta);
    for (int i = 0; i < objects.index; i++) {
      Body *body = get_element(&objects, i);

      render_object(
          body->aabb.position,
          (vec2){body->aabb.half_size[0] * 2, body->aabb.half_size[1] * 2},
          &vao, &program, 6);
      if (body->aabb.position[0] > width || body->aabb.position[0] < 0)
        body->velocity[0] = -body->velocity[0];
      if (body->aabb.position[1] > height || body->aabb.position[1] < 0)
        body->velocity[1] = -body->velocity[1];

      if (body->velocity[0] > 500)
        body->velocity[0] = 500;
      if (body->velocity[0] < -500)
        body->velocity[0] = -500;
      if (body->velocity[1] > 500)
        body->velocity[1] = 500;
      if (body->velocity[1] < -500)
        body->velocity[1] = -500;
    }

    SDL_GL_SwapWindow(win);
    time_update_late(&frame_delay, &time_last);
  }
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}
