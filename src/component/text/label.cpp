#include "hierro/component/label.hpp"
#include "hierro/component/component.hpp"
#include "hierro/component/text.hpp"

Label::Label() {}

void Label::draw() {
  // assert tg is inited
  auto tg = TextGenerater::get_instance();
  tg->draw_text(
    this->content,
    this->absolute_position(),
    this->absolute_size(),
    this->wrap,
    this->overflow,
    this->spacing,
    this->line_spacing,
    this->scale,
    this->color,
    this->vertical_align,
    this->horizontal_align
  );
}

IMPL_COMPONENT(Label)
