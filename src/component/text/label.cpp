#include "hierro/component/label.h"
#include "hierro/component/component.h"
#include "hierro/component/text.h"

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
    this->color
  );
}

IMPL_COMPONENT(Label)
