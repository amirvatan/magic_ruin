#include "engine/render/render.h"
#include "engine/render/render_init.h"
#include "engine/render/render_util.h"
#include "engine/time/time.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h>
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
void input_processing(bool *keys, vec2 pos, f32 delta) {

  if (keys[SDLK_d]) {
    pos[0] += 0.5 * delta;
  } else if (keys[SDLK_a]) {
    pos[0] -= 0.5 * delta;
  } else if (keys[SDLK_w]) {
    pos[1] += 0.5 * delta;
  } else if (keys[SDLK_s]) {
    pos[1] -= 0.5 * delta;
  } else if (keys[128]) {
    exit(1);
  }
}

int main() {
  bool KEYS[322] = {0};
  SDL_Window *win;
  bool exit = 0;
  //  render_init(win);
  win = render_init_window(800, 600);
  f32 frame_delay;
  f32 time_last = 0;
  f32 delta;
  time_init(60, &frame_delay);
  u32 vao;
  u32 vbo;
  u32 ebo;
  vec2 pos = {0.0, 0.0};
  vec2 size = {0.3, 0.3};
  // render_init_triangle(&vao, &vbo, &ebo);
  render_init_quad(&vao, &vbo, &ebo);
  u32 program = render_shader("shaders/default.vert", "shaders/default.frag");
  u32 texture = render_texture("./texture/container.jpg");
  while (!exit) {
    time_update(&time_last, &delta);
    keyboard(KEYS);
    input_processing(KEYS, pos, delta);
    render_begin();
    glBindTexture(GL_TEXTURE_BINDING_2D, texture);
    render_object(pos, size, &vao, &program, 6);
    SDL_GL_SwapWindow(win);
    time_update_late(&frame_delay, &time_last);
  }
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}
