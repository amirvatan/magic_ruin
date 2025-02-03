#include "render_util.h"
#include <stdio.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

u32 render_shader(char *vert_path, char *freg_path) {
  int success;
  char log[512];

  char *src_vertex = read_file(vert_path);
  if (!src_vertex) {
    printf("Error reading file: %s\n", vert_path);
  }

  u32 shader_vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(shader_vertex, 1, (const char *const *)&src_vertex, NULL);
  glCompileShader(shader_vertex);
  glGetShaderiv(shader_vertex, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader_vertex, 512, NULL, log);
    printf("Error compiling vertex shader. %s\n", log);
  }

  char *src_fragment = read_file(freg_path);
  if (!src_fragment) {
    printf("Error reading file: %s\n", freg_path);
  }

  u32 shader_fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(shader_fragment, 1, (const char *const *)&src_fragment, NULL);
  glCompileShader(shader_fragment);
  glGetShaderiv(shader_fragment, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader_fragment, 512, NULL, log);
    printf("Error compiling fragment shader. %s\n", log);
  }

  u32 shader = glCreateProgram();
  glAttachShader(shader, shader_vertex);
  glAttachShader(shader, shader_fragment);
  glLinkProgram(shader);
  glGetProgramiv(shader, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shader, 512, NULL, log);
    printf("Error linking shader. %s\n", log);
  }

  free(src_vertex);
  free(src_fragment);

  return shader;
}

u32 render_texture(char *tex_path) {

  u32 texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  int width, height, nChannels;
  unsigned char *buffer = stbi_load(tex_path, &width, &height, &nChannels, 0);
  if (buffer) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, buffer);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    printf("Failed to generate texture");
    exit(1);
  }
  stbi_image_free(buffer);
  return texture;
}
