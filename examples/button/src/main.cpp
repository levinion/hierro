#include <cmath>
#include <cstdlib>
#include <ctime>
#include <random>
#include "hierro/app.hpp"
#include "hierro/event/event.hpp"
#include "hierro/event/keycode.hpp"
#include "hierro/widget/button.hpp"
#include "hierro/window.hpp"
#include "hierro/backend/sdl.hpp"

using namespace hierro;

int main() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> random(0, 1);

#define rand random(gen)
  auto app = Application::get_instance();
  WindowSettings settings;
  app->add_font("assets/fonts/LXGWWenKai-Regular.ttf")
    ->init<SDLBackend>(settings)
    .value();

  auto but = app->add_child<Button>();
  but->center();

  but->on_click([&](ClickEvent e) {
    if (e.button == MouseButton::Left && e.press) {
      // TODO: should not visit inside component directly, so we need some wrapper here
      but->block->color = Color(rand, rand, rand);
    }
  });

  app->run().value();
}
