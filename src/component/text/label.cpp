#include "hierro/component/label.h"
#include "hierro/component/text.h"

Label::Label() {}

void Label::draw() {
  // assert tg is inited
  auto tg = TextGenerater::get_instance();
  tg->draw_text(
    this->content,
    this->position,
    this->size,
    this->line_spacing,
    this->scale,
    this->color
  );
}
