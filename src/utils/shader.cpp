#include <glad/glad.h>
#include <GL/gl.h>
#include "hierro/utils/shader.hpp"
#include <GL/glext.h>
#include <cstddef>
#include <string>
#include <spdlog/spdlog.h>

namespace hierro {

Shader::Shader(std::string name, const char* vertex, const char* fragment) {
  GLuint vertex_shader;
  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex, NULL);
  glCompileShader(vertex_shader);

  {
    GLint status = GL_TRUE;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
      spdlog::error("error when compile fragment_shader: {}", name);
    }
  }

  GLuint fragment_shader;
  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment, NULL);
  glCompileShader(fragment_shader);

  {
    GLint status = GL_TRUE;
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
      spdlog::error("error when compile fragment_shader:", name);
    }
  }

  GLuint shader_program;
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
      spdlog::error("error when link program:", name);
    }
  }

  this->program = shader_program;
}

} // namespace hierro
