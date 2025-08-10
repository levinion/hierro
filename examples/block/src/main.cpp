#include <cstdlib>
#include <random>
#include "hierro/hierro.hpp"

using namespace hierro;

int main() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> random(0, 1);

#define rand random(gen)

  auto app = Application::get_instance();
  WindowSettings settings;
  // this app did not need to render text
  app->init<SDLBackend>(settings).value();
  auto block = app->add_child<Block>();
  auto block2 = block->add_child<Block>();
  block->color = Color(rand, 0, 1);
  block->set_position(0.0, 1.0);
  block->center();
  block2->color = Color(0, 0, 1);
  block2->set_size(0.5, 0.5);
  block2->center();

  block->on_click([&](ClickEvent e) {
    if (e.button == hierro::MouseButton::Left && e.press)
      block->color = Color(rand, rand, rand);
  });

  block2->on_click([&](ClickEvent e) {
    if (e.button == hierro::MouseButton::Left && e.press)
      block2->color = Color(rand, rand, rand);
  });

  app->on_click([&](ClickEvent e) {
    if (e.button == hierro::MouseButton::Left && e.press) {
      app->background = Color(rand, rand, rand);
    }
  });

  app->run().value();
}
