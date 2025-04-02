#include "render.h"
#include "../include/linmath.h"
#include "render_init.h"
#include "render_util.h"
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <string.h>
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

static u32 vao_quad;
static u32 vbo_quad;
static u32 ebo_quad;
static u32 shader_default;
static u32 shader_batch;
static u32 vao_batch;
static u32 vbo_batch;
static u32 ebo_batch;
static u32 texture_batch;
static f32 window_width = 1920;
static f32 window_height = 1080;
static f32 render_width = 640;
static f32 render_height = 360;

static d_array *batch_list;
SDL_Window *render_init() {

  SDL_Window *win = render_init_window(window_width, window_height);
  render_init_quad(&vao_quad, &vbo_quad, &ebo_quad);
  render_init_shaders(&shader_default, &shader_batch, render_width,
                      render_height);
  render_init_batch_quads(&vao_batch, &vbo_batch, &ebo_batch);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  stbi_set_flip_vertically_on_load(1);
  batch_list = init_array(20, sizeof(Batch_Vertex));

  return win;
}

void render_begin() {
  glClearColor(0.2, 0.3, 0.3, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  batch_list->index = 0;
}
static void render_batch(Batch_Vertex *vertices, usize count, u32 texture_id) {
  glBindBuffer(GL_ARRAY_BUFFER, vbo_batch);
  glBufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(Batch_Vertex), vertices);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_id);

  glUseProgram(shader_batch);
  glBindVertexArray(vao_batch);
  glDrawElements(GL_TRIANGLES, (count >> 2) * 6, GL_UNSIGNED_INT, NULL);
}
static void append_quad(vec2 position, vec2 size, vec4 texture_coordinates) {
  vec4 uvs = {0, 0, 1, 1};

  if (texture_coordinates != NULL) {
    memcpy(uvs, texture_coordinates, sizeof(vec4));
  }

  push_array(batch_list, &(Batch_Vertex){
                             .position = {position[0], position[1]},
                             .uvs = {uvs[0], uvs[1]},

                         });

  push_array(batch_list, &(Batch_Vertex){
                             .position = {position[0] + size[0], position[1]},
                             .uvs = {uvs[2], uvs[1]},
                         });

  push_array(batch_list,
             &(Batch_Vertex){
                 .position = {position[0] + size[0], position[1] + size[1]},
                 .uvs = {uvs[2], uvs[3]},
             });

  push_array(batch_list, &(Batch_Vertex){
                             .position = {position[0], position[1] + size[1]},
                             .uvs = {uvs[0], uvs[3]},
                         });
}

static void calculate_sprite_texture_coordinates(vec4 result, f32 row,
                                                 f32 column, f32 texture_width,
                                                 f32 texture_height,
                                                 f32 cell_width,
                                                 f32 cell_height) {
  f32 w = 1.0 / (texture_width / cell_width);
  f32 h = 1.0 / (texture_height / cell_height);
  f32 x = column * w;
  f32 y = row * h;
  result[0] = x;
  result[1] = y;
  result[2] = x + w;
  result[3] = y + h;
}

void render_sprite_sheet_frame(Sprite_Sheet *sprite_sheet, f32 row, f32 column,
                               vec2 position) {
  vec4 uvs;
  calculate_sprite_texture_coordinates(
      uvs, row, column, sprite_sheet->width, sprite_sheet->height,
      sprite_sheet->cell_width, sprite_sheet->cell_height);

  vec2 size = {sprite_sheet->cell_width, sprite_sheet->cell_height};
  vec2 bottom_left = {position[0] - size[0] * 0.5, position[1] - size[1] * 0.5};
  append_quad(bottom_left, size, uvs);
}
void render_sprite_sheet_init(Sprite_Sheet *sprite_sheet, char *path,
                              f32 cell_width, f32 cell_height) {
  glGenTextures(1, &sprite_sheet->texture_id);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, sprite_sheet->texture_id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  int width, height, channel_count;
  unsigned char *image_data =
      stbi_load(path, &width, &height, &channel_count, 0);
  if (!image_data) {
    printf("Failed to load image: %s\n", path);
  }

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, image_data);
  stbi_image_free(image_data);

  sprite_sheet->width = (f32)width;
  sprite_sheet->height = (f32)height;
  sprite_sheet->cell_width = cell_width;
  sprite_sheet->cell_height = cell_height;
}

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

  append_quad(aabb->position, size, NULL);
}
void render_end(SDL_Window *win) {

  render_batch(batch_list->data, batch_list->index, texture_batch);
  SDL_GL_SwapWindow(win);
}
void render_set_batch_texture(u32 texture_id) { texture_batch = texture_id; }
