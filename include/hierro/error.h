#pragma once
#include <cstdlib>
#include <optional>

enum HierroError { GLADERROR = 1 };

class HierroResult {
public:
  std::optional<HierroError> value;
  void unwrap();

  static HierroResult err(HierroError err) {
    HierroResult r;
    r.value = err;
    return r;
  };

  static HierroResult ok() {
    return HierroResult();
  };
};
