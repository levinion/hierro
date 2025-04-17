#pragma once

#define try(f) \
  { \
    auto v = f; \
    if (!v.has_value()) { \
      return v; \
    } \
  }
