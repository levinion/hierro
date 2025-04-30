#include "hierro/widget/button.hpp"
#include "hierro/utils/error.hpp"

namespace hierro {

HierroResult<void> Button::draw() {
  return {};
};

IMPL_COMPONENT(Button)

Button::Button() {}

Button* Button::init() {
  this->block = this->add_child<Block>();
  this->label = this->block->add_child<Label>();

  block->set_size(1.0, 1.0);
  block->center();

  // label.set_position(0.3, 0.65);
  label->set_size(1, 1);
  label->center();

  label->content = L"button";
  label->overflow = false;

  block->set_proxy(this);

  return this;
}

bool Button::is_hitted(float x, float y) {
  return false;
}

} // namespace hierro
