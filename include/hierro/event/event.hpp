#pragma once

#include <vector>
#ifdef ENABLE_GLFW_BACKEND
  #include "hierro/backend/glfw.hpp"
#endif
#ifdef ENABLE_SDL_BACKEND
  #include "hierro/backend/sdl.hpp"
#endif
#include "hierro/event/keycode.hpp"
#include "magic_enum/magic_enum.hpp"
#include "hierro/event/keystate.hpp"

namespace hierro {
class ClickEvent {
public:
  MouseButton button; // button index;
  Position position = { 0, 0 }; // cursor pos
  bool single_click; // true for single click and false for double click
  bool press; // false for release and true for press

private:
#ifdef ENABLE_SDL_BACKEND
  friend SDLBackend;
#endif
#ifdef ENABLE_GLFW_BACKEND
  friend GLFWBackend;
#endif
};

class KeyEvent {
public:
  Key key; // lastest key
  bool press; // false for release and true for press

  inline bool is_pressed(Key key) {
    return this->keystate->contains(key);
  };

  inline bool match(std::string key) {
    if (auto k = magic_enum::enum_cast<Key>(key); !k)
      return false;
    else
      return is_pressed(k.value());
  }

  inline bool match(Key key) {
    return is_pressed(key);
  }

  inline bool match(std::vector<std::string> keys) {
    for (auto& key : keys) {
      if (auto k = magic_enum::enum_cast<Key>(key); !k)
        return false;
      else if (!is_pressed(k.value()))
        return false;
    }
    return true;
  }

  inline bool match(std::vector<Key> keys) {
    for (auto& key : keys) {
      if (!is_pressed(key))
        return false;
    }
    return true;
  }

private:
  KeyState* keystate;
#ifdef ENABLE_SDL_BACKEND
  friend SDLBackend;
#endif
#ifdef ENABLE_GLFW_BACKEND
  friend GLFWBackend;
#endif
};

class FocusEvent {
public:
private:
#ifdef ENABLE_SDL_BACKEND
  friend SDLBackend;
#endif
#ifdef ENABLE_GLFW_BACKEND
  friend GLFWBackend;
#endif
};

class UnFocusEvent {
public:
private:
#ifdef ENABLE_SDL_BACKEND
  friend SDLBackend;
#endif
#ifdef ENABLE_GLFW_BACKEND
  friend GLFWBackend;
#endif
};

class InputEvent {
public:
  std::wstring input;

private:
#ifdef ENABLE_SDL_BACKEND
  friend SDLBackend;
#endif
#ifdef ENABLE_GLFW_BACKEND
  friend GLFWBackend;
#endif
};

class MouseMoveEvent {
public:
  float xrel;
  float yrel;
  Position position = { 0, 0 };

private:
#ifdef ENABLE_SDL_BACKEND
  friend SDLBackend;
#endif
#ifdef ENABLE_GLFW_BACKEND
  friend GLFWBackend;
#endif
};

class MouseWheelEvent {
public:
  float y;

private:
#ifdef ENABLE_SDL_BACKEND
  friend SDLBackend;
#endif
#ifdef ENABLE_GLFW_BACKEND
  friend GLFWBackend;
#endif
};

} // namespace hierro
