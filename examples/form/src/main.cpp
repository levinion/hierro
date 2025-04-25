#include <cmath>
#include <cstdlib>
#include <ctime>
#include "hierro/app.hpp"
#include "hierro/component/label.hpp"
#include "hierro/component/text.hpp"
#include "hierro/widget/input.hpp"
#include "hierro/widget/button.hpp"

using namespace hierro;

int main() {
  auto app = Application::get_instance();
  app->add_font("assets/fonts/LXGWWenKai-Regular.ttf")->init(800, 600).unwrap();

  Block container;
  container.set_size(0.5, 0.5);

  Label label;
  label.content = L"Name: ";
  label.set_position(0.1, 1);
  label.vertical_align = VerticalAlign::Left;

  Input input;
  input.set_size(0.5, 0.2);
  input.center();
  input.label.wrap = false;
  input.label.overflow = false;

  Button button;
  button.label.content = L"submit";
  button.set_position(0, 1);
  button.set_size(0.1, 0.1);
  button.on_click([&](auto but, auto action, auto mods) {
    if (but == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {}
  });

  container.add_child(&label)->add_child(&input);

  app->add_child(&container);
  app->add_child(&button);
  app->run();
}
