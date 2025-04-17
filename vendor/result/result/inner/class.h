#pragma once

#include <expected>
#include <string>

template<typename T, typename E>
class Result {
  std::expected<T, E> value;

public:
  T unwrap();
  T unwrap_or(T value);
  T expected(std::string message);
  E unwrap_err();
  static Result<T, E> ok(T value);
  static Result<T, E> err(E err);
  bool has_value();
};

template<typename E>
class Result<void, E> {
  std::expected<void, E> value;

public:
  void unwrap();
  E unwrap_err();
  void expected(std::string message);
  static Result<void, E> ok();
  static Result<void, E> err(E err);
  bool has_value();
};
