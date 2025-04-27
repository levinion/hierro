#pragma once

#include <string>

namespace hierro {

class Shader {
public:
  Shader(std::string name, const char* vertex, const char* fragment);
  Shader();

  void use();
  unsigned int id();

private:
  unsigned int program;
};

} // namespace hierro
