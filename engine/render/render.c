#include "render.h"
#include "../include/linmath.h"
#include "render_util.h"
void render_init(SDL_Window *win) { win = render_init_window(800, 600); }

void render_begin() {
  glClearColor(0.2, 0.3, 0.3, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
}

void render_end(SDL_Window *win) { SDL_GL_SwapWindow(win); }

void render_object(vec2 pos, vec2 size, u32 *vao, u32 *program, u32 nElement) {

  glUseProgram(*program);
  mat4x4 model;
  mat4x4_identity(model);
  mat4x4_translate(model, pos[0], pos[1], 0);
  mat4x4_scale_aniso(model, model, size[0], size[1], 0);
  glUniformMatrix4fv(glGetUniformLocation(*program, "model"), 1, GL_FALSE,
                     &model[0][0]);

  glBindVertexArray(*vao);
  //  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  glDrawElements(GL_TRIANGLES, nElement, GL_UNSIGNED_INT, NULL);

  glBindVertexArray(0);
}
