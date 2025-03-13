#include "hierro/error.h"
#include <cstdlib>
#include <expected>
#include <ostream>
#include <iostream>

std::ostream& operator<<(std::ostream& out, const HierroError err) {
  switch (err) {
    case HierroError::GLADERROR:
      out << "GLAD ERROR";
      return out;
    default:
      out << "UNDEFINED ERROR";
      return out;
  }
};

template<typename T>
void HierroResult<T>::unwrap() {
  if (!this->value.has_value()) {
    std::cout << this->value.value() << std::endl;
    exit(1);
  }
}

template<typename T>
void HierroResult<T>::expected(std::string message) {
  if (!this->value.has_value()) {
    std::cout << this->value.error() << std::endl;
    exit(1);
  }
}

template<typename T>
HierroResult<T> HierroResult<T>::ok(T value) {
  HierroResult<T> r;
  r.value = value;
  return r;
}

template<typename T>
HierroResult<T> HierroResult<T>::err(HierroError err) {
  return std::unexpected(err);
}

void HierroResult<void>::unwrap() {
  if (!this->value.has_value()) {
    exit(1);
  }
}

void HierroResult<void>::expected(std::string message) {
  if (!this->value.has_value()) {
    std::cout << this->value.error() << std::endl;
    exit(1);
  }
}

HierroResult<void> HierroResult<void>::ok() {
  HierroResult<void> r;
  return r;
}

HierroResult<void> HierroResult<void>::err(HierroError err) {
  HierroResult<void> r;
  r.value = std::unexpected<HierroError>(err);
  return r;
}
