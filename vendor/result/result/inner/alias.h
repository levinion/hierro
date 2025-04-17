#pragma once

#include "../result.h"

template<typename T, typename E>
Result<T, E> ok(T value) {
  return Result<T, E>::ok(value);
}

template<typename T, typename E>
Result<T, E> err(E error) {
  return Result<T, E>::err(error);
}

template<typename E>
Result<void, E> ok() {
  return Result<void, E>::ok();
}

template<typename E>
Result<void, E> err(E error) {
  return Result<void, E>::err(error);
}
