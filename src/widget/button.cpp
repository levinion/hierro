#include "hierro/widget/button.hpp"

void Button::draw() {};

IMPL_COMPONENT(Button)

Button::Button() {
  block.set_size(1.0, 1.0);
  block.center();

  // label.set_position(0.3, 0.65);
  label.set_size(1, 1);
  label.center();

  label.content = L"button";
  label.overflow = false;

  block.on_input([&](unsigned int codepoint) {
    this->send_input_event(codepoint);
  });

  block.on_key([&](int key, int scancode, int action, int mod) {
    this->send_key_event(key, scancode, action, mod);
  });

  block.on_focus([&]() { this->send_focus_event(); });

  block.on_click([&](int button, int action, int mods) {
    this->send_click_event(button, action, mods);
  });

  this->block.add_child(&this->label);
  this->add_child(&this->block);
}

bool Button::is_hitted(float x, float y) {
  return false;
}
