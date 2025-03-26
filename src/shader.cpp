#include <glad/glad.h>
#include "hierro/shader.hpp"
#include <GL/glext.h>
#include <cstddef>

Shader::Shader(const char* vertex, const char* fragment) {
  unsigned int vertex_shader;
  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex, NULL);
  glCompileShader(vertex_shader);

  unsigned int fragment_shader;
  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment, NULL);
  glCompileShader(fragment_shader);

  unsigned int shader_program;
  shader_program = glCreateProgram();

  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  this->program = shader_program;
}

void Shader::use() {
  glUseProgram(this->program);
}

unsigned int Shader::id() {
  return this->program;
}

Shader::Shader() {}
