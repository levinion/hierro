#pragma once
#include <cstdlib>
#include <expected>
#include <string>

namespace hierro {

template<typename T>
using HierroResult = std::expected<T, std::string>;

#define check(x) \
  if (auto r = x; !r) \
  return r

#define err(x) std::unexpected(x)

} // namespace hierro
