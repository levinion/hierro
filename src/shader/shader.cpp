#include <glad/glad.h>
#include "hierro/shader/shader.hpp"
#include <GL/glext.h>
#include <cstddef>
#include <string>
#include "hierro/utils/log.hpp"

namespace hierro {

Shader::Shader(std::string name, const char* vertex, const char* fragment) {
  unsigned int vertex_shader;
  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex, NULL);
  glCompileShader(vertex_shader);

  {
    GLint status = GL_TRUE;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) { /* 查询并打印错误日志 */
      LOG("error when compile fragment_shader:", name);
    }
  }

  unsigned int fragment_shader;
  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment, NULL);
  glCompileShader(fragment_shader);

  {
    GLint status = GL_TRUE;
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
      LOG("error when compile fragment_shader:", name);
    }
  }

  unsigned int shader_program;
  shader_program = glCreateProgram();

  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  {
    GLint status = GL_TRUE;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &status);

    if (status != GL_TRUE) {
      LOG("error when link program:", name);
    }
  }

  this->program = shader_program;
}

void Shader::use() {
  glUseProgram(this->program);
}

unsigned int Shader::id() {
  return this->program;
}

Shader::Shader() {}

} // namespace hierro
