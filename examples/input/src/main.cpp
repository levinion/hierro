#include <cmath>
#include <cstdlib>
#include <ctime>
#include "hierro/app.hpp"
#include "hierro/widget/input.hpp"

int main() {
  auto app = Application::get_instance();
  app->add_font("assets/fonts/LXGWWenKai-Regular.ttf");
  app->init().unwrap();

  Input input;
  input.set_size(0.3, 0.1);
  input.center();

  input.label.wrap = false;
  input.label.overflow = false;

  app->add_child(&input);
  app->run();
}
