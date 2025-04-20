#pragma once

namespace hierro {
class ClickEvent {
public:
private:
  int key;
  int action;
  int mod;
};

class KeyEvent {
public:
private:
  int key;
  int scancode;
  int action;
  int mod;
};

class FocusEvent {};
} // namespace hierro
