#pragma once

#include "hierro/backend/glfw.hpp"
#include "hierro/backend/sdl.hpp"
#include "hierro/event/keys.hpp"

namespace hierro {
class ClickEvent {
public:
  //TODO: make propertities private
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
  Key key;
  Key mod;
  bool press; // false for release and true for press

private:
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
