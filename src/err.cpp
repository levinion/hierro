#include <GLFW/glfw3.h>
#include <iostream>

bool check_error() {
  auto err = glGetError();
  if (err != GL_NO_ERROR) {
    std::cerr << "Error: " << err << std::endl;
    return true;
  }
  return false;
}

#define try_calling(func) \
  func; \
  if (check_error()) \
    return -1;
