#pragma once

#define DEBUG 1

#if DEBUG == 1
  #include <iostream>
  #define LOG(x) std::cout << x << std::endl
#endif
