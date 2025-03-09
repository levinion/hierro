#include <GLFW/glfw3.h>

bool check_error();

#define try_calling(func) \
  func; \
  if (check_error()) \
    return -1;
