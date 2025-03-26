#pragma once
#include <cstdlib>
#include <expected>
#include <result/result.h>

using HierroError = const char*;

template<typename T>
using HierroResult = Result<T, HierroError>;

template<typename T>
HierroResult<T> ok(T value);

template<typename T>
HierroResult<T> err(HierroError error);

HierroResult<void> ok();

HierroResult<void> err(HierroError error);

template<typename T>
HierroResult<T> ok(T value) {
  return HierroResult<T>::ok(value);
}

template<typename T>
HierroResult<T> err(HierroError error) {
  return HierroResult<T>::err(error);
}
