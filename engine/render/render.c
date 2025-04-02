#include "render.h"
#include "../include/linmath.h"
#include "render_init.h"
#include "render_util.h"
#include <SDL2/SDL_video.h>
static u32 vao_quad;
static u32 vbo_quad;
static u32 ebo_quad;
static u32 shader_default;
static u32 shader_batch;
static f32 window_width = 1920;
static f32 window_height = 1080;
static f32 render_width = 640;
static f32 render_height = 360;

SDL_Window *render_init() {

  SDL_Window *win = render_init_window(window_width, window_height);
  render_init_quad(&vao_quad, &vbo_quad, &ebo_quad);
  render_init_shaders(&shader_default, &shader_batch, render_width,
                      render_height);
  return win;
}

void render_begin() {
  glClearColor(0.2, 0.3, 0.3, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
}

void render_end(SDL_Window *win) { SDL_GL_SwapWindow(win); }

void render_object(vec2 pos, vec2 size, u32 nElement) {
  glUseProgram(shader_default);
  mat4x4 model;
  mat4x4_identity(model);
  mat4x4_translate(model, pos[0], pos[1], 0);
  mat4x4_scale_aniso(model, model, size[0], size[1], 0);
  glUniformMatrix4fv(glGetUniformLocation(shader_default, "model"), 1, GL_FALSE,
                     &model[0][0]);

  glBindVertexArray(vao_quad);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  glDrawElements(GL_TRIANGLES, nElement, GL_UNSIGNED_INT, NULL);

  glBindVertexArray(0);
}
void render_aabb(AABB *aabb) {
  vec2 size;
  vec2_scale(size, aabb->half_size, 2);
  render_object(aabb->position, size, 6);
}
