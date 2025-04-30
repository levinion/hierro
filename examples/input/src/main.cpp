#include <cmath>
#include <cstdlib>
#include <ctime>
#include "hierro/hierro.hpp"

using namespace hierro;

int main() {
  auto app = Application::get_instance();

  WindowSettings settings;
  app->add_font("assets/fonts/LXGWWenKai-Regular.ttf")
    ->init<SDLBackend>(settings)
    .value();

  auto input = app->add_child<Input>()->init();
  input->set_size(0.3, 0.1);
  input->center();

  input->label->wrap = false;
  input->label->overflow = false;

  app->run().value();
}
