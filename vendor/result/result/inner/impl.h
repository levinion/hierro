#pragma once

#include "../result.h"
#include "iostream"

// unwrap

template<typename T, typename E>
T Result<T, E>::unwrap() {
  if (!this->value.has_value()) {
    std::cout << this->value.error() << std::endl;
    exit(1);
  }
  return this->value.value();
}

template<typename E>
void Result<void, E>::unwrap() {
  if (!this->value.has_value()) {
    std::cout << this->value.error() << std::endl;
    exit(1);
  }
}

// unwrap_err

template<typename T, typename E>
E Result<T, E>::unwrap_err() {
  if (this->value.has_value()) {
    std::cout << this->value.value() << std::endl;
    exit(1);
  }
  return this->value.error();
}

template<typename E>
E Result<void, E>::unwrap_err() {
  if (this->value.has_value()) {
    std::cout << this->value.value() << std::endl;
    exit(1);
  }
  return this->value.error();
}

// upwrap_or

template<typename T, typename E>
T Result<T, E>::unwrap_or(T value) {
  if (!this->value.has_value()) {
    return value;
  }
  return this->value.value();
}

// expected

template<typename T, typename E>
T Result<T, E>::expected(std::string message) {
  if (!this->value.has_value()) {
    std::cout << this->value.error() << std::endl;
    exit(1);
  }
  return this->value.value();
}

template<typename E>
void Result<void, E>::expected(std::string message) {
  if (!this->value.has_value()) {
    std::cout << this->value.error() << std::endl;
    exit(1);
  }
}

// ok

template<typename T, typename E>
Result<T, E> Result<T, E>::ok(T value) {
  Result<T, E> r;
  r.value = value;
  return r;
}

template<typename E>
Result<void, E> Result<void, E>::ok() {
  Result<void, E> r;
  return r;
}

// err

template<typename T, typename E>
Result<T, E> Result<T, E>::err(E err) {
  return std::unexpected(err);
}

template<typename E>
Result<void, E> Result<void, E>::err(E err) {
  Result<void, E> r;
  r.value = std::unexpected<E>(err);
  return r;
}

// has value

template<typename T, typename E>
bool Result<T, E>::has_value() {
  return this->value.has_value();
}

template<typename E>
bool Result<void, E>::has_value() {
  return this->value.has_value();
}
