#pragma once

#define DEBUG 1

#if DEBUG == 1

  #include <iostream>

namespace hierro {

template<typename T, typename... Args>
void LOG(T value, Args... args) {
  std::cout << value;
  ((std::cout << ' ' << args), ...);
  std::cout << std::endl;
}

} // namespace hierro
#endif
