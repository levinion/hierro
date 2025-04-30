#include "hierro/widget/input.hpp"
#include "hierro/component/text.hpp"
#include "hierro/utils/error.hpp"
#include "hierro/event/event.hpp"
#include <SDL3/SDL_keyboard.h>
#include <spdlog/spdlog.h>

namespace hierro {

HierroResult<void> Input::draw() {
  return {};
};

IMPL_COMPONENT(Input)

Input::Input() {}

Input* Input::init() {
  block = this->add_child<Block>();
  label = this->block->add_child<Label>();

  block->set_size(1.0, 1.0);
  block->color = { 0.5, 0.2, 0.3 };
  block->center();

  label->set_size(0.9, 1);
  label->center();
  label->overflow = false;
  label->vertical_align = VerticalAlign::Left;
  label->content = L"";

  block->on_input([&](InputEvent e) {
    label->content += e.input;
    this->send_input_event(e);
  });

  block->on_key([&](KeyEvent e) {
    if (e.press) {
      if (e.key == hierro::Key::Backspace) {
        if (!label->content.empty()) {
          label->content.pop_back();
        }
      }
    }
    if (e.key == hierro::Key::Enter && e.press) {
      label->content.push_back('\n');
    }
    this->send_key_event(e);
  });
  return this;
}

bool Input::is_hitted(float x, float y) {
  return false;
}

} // namespace hierro
