#pragma once

class Shader {
public:
  Shader(const char* vertex, const char* fragment);
  Shader();

  void use();
  unsigned int id();

private:
  unsigned int program;
};
