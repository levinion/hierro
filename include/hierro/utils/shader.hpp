#pragma once

#include <glad/glad.h>
#include <GL/gl.h>
#include <string>

namespace hierro {

class Shader {
public:
  Shader(std::string name, const char* vertex, const char* fragment);

  Shader() {}

  inline void use() {
    glUseProgram(this->program);
  }

  inline GLuint id() {
    return this->program;
  }

private:
  GLuint program;
};

} // namespace hierro
