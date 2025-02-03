#include "engine/render/render.h"
#include "engine/render/render_init.h"
#include "engine/render/render_util.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h>
int main() {

  SDL_Window *win;
  bool exit = 0;
  //  render_init(win);
  win = render_init_window(800, 600);
  u32 vao;
  u32 vbo;
  u32 ebo;
  vec2 pos = {0.5, 0.5};
  vec2 size = {1.0, 1.0};
  render_init_triangle(&vao, &vbo, &ebo);
  // render_init_quad(&vao, &vbo, &ebo);
  u32 program = render_shader("shaders/default.vert", "shaders/default.frag");
  u32 texture = render_texture("./texture/container.jpg");
  SDL_Event e;
  while (!exit) {
    while (SDL_PollEvent(&e) != 0) {
      switch (e.type) {
      case SDL_QUIT:
        exit = true;
        break;
      }
    }
    render_begin();
    glBindTexture(GL_TEXTURE_BINDING_2D, texture);
    render_object(pos, size, &vao, &program, 12);
    SDL_GL_SwapWindow(win);
  }
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}
