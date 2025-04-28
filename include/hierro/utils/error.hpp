#pragma once
#include <cstdlib>
#include <expected>
#include <string>

namespace hierro {

template<typename T>
using HierroResult = std::expected<T, std::string>;

#define hierro_check(x) \
  if (auto r = x; !r) \
  return r

#define hierro_err(x) std::unexpected(x)

} // namespace hierro
