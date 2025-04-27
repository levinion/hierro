#pragma once

#include <bitset>
#include "hierro/event/keycode.hpp"

namespace hierro {
class KeyState {
public:
  inline void set(Key key) {
    state.set(static_cast<int>(key));
  }

  inline bool contains(Key key) {
    return state.test(static_cast<int>(key));
  }

  inline void erase(Key key) {
    state.reset(static_cast<int>(key));
  }

private:
  std::bitset<256> state = { 0 };
};
} // namespace hierro
