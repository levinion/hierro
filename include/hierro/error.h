#pragma once
#include <cstdlib>
#include <expected>
#include <string>

enum class HierroError { GLADERROR };

template<typename T>
class HierroResult {
  std::expected<T, HierroError> value;

public:
  void unwrap();
  void expected(std::string message);
  static HierroResult<T> ok(T value);
  static HierroResult<T> err(HierroError err);
};

template<>
class HierroResult<void> {
  std::expected<void, HierroError> value;

public:
  void unwrap();
  void expected(std::string message);
  static HierroResult<void> ok();
  static HierroResult<void> err(HierroError err);
};
