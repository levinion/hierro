#include "hierro/widget/input.hpp"
#include "hierro/component/text.hpp"
#include "hierro/event/event.hpp"

namespace hierro {

void Input::draw() {
  // this->block.draw();
  // this->label.draw();
};

IMPL_COMPONENT(Input)

Input::Input() {
  auto label = this->block.add_child<Label>();
  auto block = this->add_child<Block>();

  block->set_size(1.0, 1.0);
  block->center();

  label->set_size(0.9, 1);
  label->center();
  label->overflow = false;
  label->vertical_align = VerticalAlign::Left;
  label->content = L"";

  block->on_input([&](unsigned int codepoint) {
    label->content.push_back(codepoint);
    this->send_input_event(codepoint);
  });

  block->on_key([&](KeyEvent e) {
    // if (action == GLFW_PRESS || action == GLFW_REPEAT) {
    //   if (key == GLFW_KEY_BACKSPACE) {
    //     if (!label.content.empty()) {
    //       label.content.pop_back();
    //     }
    //   }
    // }
    // if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
    //   label.content.push_back('\n');
    // }
    this->send_key_event(e);
  });
}

bool Input::is_hitted(float x, float y) {
  return false;
}

} // namespace hierro
