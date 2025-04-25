#include "hierro/widget/button.hpp"
#include "hierro/event/event.hpp"

namespace hierro {

void Button::draw() {};

IMPL_COMPONENT(Button)

Button::Button() {
  this->block = this->add_child<Block>();
  this->label = this->block->add_child<Label>();

  block->set_size(1.0, 1.0);
  block->center();

  // label.set_position(0.3, 0.65);
  label->set_size(1, 1);
  label->center();

  label->content = L"button";
  label->overflow = false;

  block->on_input([&](unsigned int codepoint) {
    this->send_input_event(codepoint);
  });

  block->on_key([&](KeyEvent e) { this->send_key_event(e); });

  block->on_focus([&]() { this->send_focus_event(); });

  block->on_click([&](ClickEvent e) { this->send_click_event(e); });
}

bool Button::is_hitted(float x, float y) {
  return false;
}

} // namespace hierro
