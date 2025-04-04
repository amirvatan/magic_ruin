#include "render_init.h"
#include "../include/linmath.h"
#include "render_util.h"
#include <SDL2/SDL_video.h>
SDL_Window *render_init_window(u32 width, u32 height) {

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("Could not init SDL: %s\n", SDL_GetError());
  }
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,
                      SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_Window *window = SDL_CreateWindow("Ruin Magic", SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED, width, height,
                                        SDL_WINDOW_OPENGL);

  if (!window) {
    printf("Failed to init window: %s\n", SDL_GetError());
  }

  SDL_GLContext context = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, context);
  if (!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress)) {
    printf("Failed to load GL: %s\n", SDL_GetError());
  }

  puts("OpenGL Loaded");
  printf("Vendor:   %s\n", glGetString(GL_VENDOR));
  printf("Renderer: %s\n", glGetString(GL_RENDERER));
  printf("Version:  %s\n", glGetString(GL_VERSION));

  return window;
}
void render_init_shaders(u32 *shader_default, u32 *shader_batch,
                         f32 rednder_width, f32 render_height) {
  *shader_default =
      render_shader("shaders/default.vert", "shaders/default.frag");
  mat4x4 projection;
  mat4x4_ortho(projection, 0, rednder_width, 0, render_height, -2, 2);
  glUseProgram(*shader_default);
  glUniformMatrix4fv(glGetUniformLocation(*shader_default, "projection"), 1,
                     GL_FALSE, &projection[0][0]);
  *shader_batch = render_shader("shaders/batch_quad_vert.glsl",
                                "shaders/batch_quad_frag.glsl");

  glUseProgram(*shader_batch);
  glUniformMatrix4fv(glGetUniformLocation(*shader_batch, "projection"), 1,
                     GL_FALSE, &projection[0][0]);
}

void render_init_batch_quads(u32 *vao, u32 *vbo, u32 *ebo) {
  glGenVertexArrays(1, vao);
  glBindVertexArray(*vao);

  u32 indices[MAX_BATCH_ELEMENTS];
  for (u32 i = 0, offset = 0; i < MAX_BATCH_ELEMENTS; i += 6, offset += 4) {
    indices[i + 0] = offset + 0;
    indices[i + 1] = offset + 1;
    indices[i + 2] = offset + 2;
    indices[i + 3] = offset + 2;
    indices[i + 4] = offset + 3;
    indices[i + 5] = offset + 0;
  }

  glGenBuffers(1, vbo);
  glBindBuffer(GL_ARRAY_BUFFER, *vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * MAX_BATCH_VERTICES * 4, NULL,
               GL_DYNAMIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Batch_Vertex),
                        (void *)offsetof(Batch_Vertex, position));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Batch_Vertex),
                        (void *)offsetof(Batch_Vertex, uvs));
  glEnableVertexAttribArray(2);

  glGenBuffers(1, ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void render_init_triangle(u32 *vao, u32 *vbo, u32 *ebo) {
  f32 vertices[] = {
      -0.5, -0.5, 0, // bottom left
      -0.5, 0.5,  0, // top left
      0.0,  0.0,  0, // center
      0.5,  0.5,  0, // top right
      0.5,  -0.5, 0, // bottom right
      -0.5, 0,    0, // center left
      0.5,  0,    0, // center right
  };

  u32 indices[] = {
      0, 5, 2, // left bottom triangle
      1, 5, 2, // left top triangle
      3, 6, 2, // right top triangle
      4, 6, 2, // right bottom triangle
  };
  glGenVertexArrays(1, vao);
  glGenBuffers(1, vbo);
  glGenBuffers(1, ebo);

  glBindVertexArray(*vao);
  glBindBuffer(GL_ARRAY_BUFFER, *vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(f32), (void *)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);
}
void render_init_quad(u32 *vao, u32 *vbo, u32 *ebo) {
  f32 vertices[] = {
      -0.5, -0.5, 0, 1.0, 1.0, // bottom left
      -0.5, 0.5,  0, 1.0, 0.0, // top left
      0.5,  0.5,  0, 0.0, 0.0, // top right
      0.5,  -0.5, 0, 0.0, 1.0, // bottom right

  };
  u32 indices[] = {
      0, 1, 3, 3, 2, 1,

  };
  glGenVertexArrays(1, vao);
  glGenBuffers(1, vbo);
  glGenBuffers(1, ebo);

  glBindVertexArray(*vao);
  glBindBuffer(GL_ARRAY_BUFFER, *vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(f32),
                        (void *)(3 * sizeof(f32)));

  glEnableVertexAttribArray(1);
}
