#pragma once

#include "hierro/backend/glfw.hpp"
#include "hierro/backend/sdl.hpp"
#include "hierro/event/keycode.hpp"
#include "magic_enum/magic_enum.hpp"

namespace hierro {
class ClickEvent {
public:
  MouseButton button; // button index;
  Position position = { 0, 0 }; // cursor pos
  bool single_click; // true for single click and false for double click
  bool press; // false for release and true for press

private:
  friend SDLBackend;
  friend GLFWBackend;
};

class KeyEvent {
public:
  Key key; // lastest key
  bool press; // false for release and true for press

  inline bool is_pressed(Key key) {
    return this->keystate->contains(key);
  };

  inline bool match(std::string key1) {
    if (auto k = magic_enum::enum_cast<Key>(key1); !k)
      return false;
    else
      return is_pressed(k.value());
  }

  inline bool match(std::string key1, std::string key2) {
    if (auto k1 = magic_enum::enum_cast<Key>(key1); !k1)
      return false;
    else if (auto k2 = magic_enum::enum_cast<Key>(key2); !k2)
      return false;
    else
      return is_pressed(k1.value()) && is_pressed(k2.value());
  }

  inline bool match(std::string key1, std::string key2, std::string key3) {
    if (auto k1 = magic_enum::enum_cast<Key>(key1); !k1)
      return false;
    else if (auto k2 = magic_enum::enum_cast<Key>(key2); !k2)
      return false;
    else if (auto k3 = magic_enum::enum_cast<Key>(key3); !k3)
      return false;
    else
      return is_pressed(k1.value()) && is_pressed(k2.value())
        && is_pressed(k3.value());
  }

  inline bool match(
    std::string key1,
    std::string key2,
    std::string key3,
    std::string key4
  ) {
    if (auto k1 = magic_enum::enum_cast<Key>(key1); !k1)
      return false;
    else if (auto k2 = magic_enum::enum_cast<Key>(key2); !k2)
      return false;
    else if (auto k3 = magic_enum::enum_cast<Key>(key3); !k3)
      return false;
    else if (auto k4 = magic_enum::enum_cast<Key>(key4); !k4)
      return false;
    else
      return is_pressed(k1.value()) && is_pressed(k2.value())
        && is_pressed(k3.value()) && is_pressed(k4.value());
  }

  inline bool match(
    std::string key1,
    std::string key2,
    std::string key3,
    std::string key4,
    std::string key5
  ) {
    if (auto k1 = magic_enum::enum_cast<Key>(key1); !k1)
      return false;
    else if (auto k2 = magic_enum::enum_cast<Key>(key2); !k2)
      return false;
    else if (auto k3 = magic_enum::enum_cast<Key>(key3); !k3)
      return false;
    else if (auto k4 = magic_enum::enum_cast<Key>(key4); !k4)
      return false;
    else if (auto k5 = magic_enum::enum_cast<Key>(key5); !k5)
      return false;
    else
      return is_pressed(k1.value()) && is_pressed(k2.value())
        && is_pressed(k3.value()) && is_pressed(k4.value())
        && is_pressed(k5.value());
  }

  inline bool match(Key key1) {
    return is_pressed(key1);
  }

  inline bool match(Key key1, Key key2) {
    return is_pressed(key1) && is_pressed(key2);
  }

  inline bool match(Key key1, Key key2, Key key3) {
    return is_pressed(key1) && is_pressed(key2) && is_pressed(key3);
  }

  inline bool match(Key key1, Key key2, Key key3, Key key4) {
    return is_pressed(key1) && is_pressed(key2) && is_pressed(key3)
      && is_pressed(key4);
  }

  inline bool match(Key key1, Key key2, Key key3, Key key4, Key key5) {
    return is_pressed(key1) && is_pressed(key2) && is_pressed(key3)
      && is_pressed(key4) && is_pressed(key5);
  }

private:
  KeyState* keystate;
  friend SDLBackend;
  friend GLFWBackend;
};

class FocusEvent {
public:
private:
  friend SDLBackend;
  friend GLFWBackend;
};

class MouseMoveEvent {
public:
  float xrel;
  float yrel;
  Position position = { 0, 0 };

private:
  friend SDLBackend;
  friend GLFWBackend;
};

class MouseWheelEvent {
public:
  float y;

private:
  friend SDLBackend;
  friend GLFWBackend;
};

} // namespace hierro
