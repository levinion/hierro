#include <cmath>
#include <cstdlib>
#include <ctime>
#include "hierro/app.hpp"
#include "hierro/component/label.hpp"
#include "hierro/component/text.hpp"
<<<<<<< HEAD
#include "hierro/utils/log.hpp"
  =======
#include "hierro/event/event.hpp"
#include "hierro/event/keycode.hpp"
  >>>>>>> kaleido
#include "hierro/widget/input.hpp"
#include "hierro/widget/button.hpp"

  using namespace hierro;

int main() {
  auto app = Application::get_instance();
  WindowSettings settings;
  app->add_font("assets/fonts/LXGWWenKai-Regular.ttf")
    ->init<SDLBackend>(settings)
    .value();

  auto container = app->add_child<Block>();
  container->set_size(0.5, 0.5);

  auto label = container->add_child<Label>();
  label->content = L"Name: ";
  label->set_position(0.1, 1);
  label->vertical_align = VerticalAlign::Left;

  auto input = container->add_child<Input>();
  input->set_size(0.5, 0.2);
  input->center();
  input->label.wrap = false;
  input->label.overflow = false;

  auto button = app->add_child<Button>();
  button->label->content = L"submit";
  button->set_position(0, 1);
  button->set_size(0.1, 0.1);
  button->on_click([&](ClickEvent e) {
    if (e.button == hierro::MouseButton::Left && e.press) {
      label->content += input->label.content;
    }
  });

  app->run().value();
}
