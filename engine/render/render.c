#include "render.h"
#include "render_util.h"
void render_init(SDL_Window *win) { win = render_init_window(800, 600); }

void render_begin() {
  glClearColor(0.2, 0.3, 0.3, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
}

void render_end(SDL_Window *win) { SDL_GL_SwapWindow(win); }

void render_triangle(u32 *vao, u32 *program) {
  glUseProgram(*program);
  glBindVertexArray(*vao);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, NULL);

  glBindVertexArray(0);
}
void render_quad(u32 *vao, u32 *program) {
  glUseProgram(*program);
  glBindVertexArray(*vao);
//  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

  glBindVertexArray(0);
}
