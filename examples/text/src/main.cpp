#include <cmath>
#include <ctime>
#include <string>
#include "hierro/hierro.hpp"

using namespace hierro;

int main() {
  auto app = Application::get_instance();
  WindowSettings settings;
  app->add_font("assets/fonts/LXGWWenKai-Regular.ttf")
    ->init<SDLBackend>(settings)
    .value();

  auto label1 = app->add_child<Label>();
  auto label2 = app->add_child<Label>();

  label1->color = Color(1, 0, 1);
  label1->set_position(0.1, 0.1);
  label1->horizontal_align = hierro::HorizontalAlign::Top;
  label1->vertical_align = hierro::VerticalAlign::Left;
  label1->content = L"Hello";

  label2->set_position(0.1, 0.9);
  label2->color = Color(1, 1, 0);
  label2->content = L"你好";

  app->run().value();
}
