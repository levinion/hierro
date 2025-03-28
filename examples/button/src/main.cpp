#include <cmath>
#include <cstdlib>
#include <ctime>
#include <random>
#include "GLFW/glfw3.h"
#include "hierro/app.hpp"
#include "hierro/utils/log.hpp"
#include "hierro/widget/button.hpp"

int main() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> random(0, 1);

#define rand random(gen)
  auto app = Application::get_instance();
  app->add_font("assets/fonts/LXGWWenKai-Regular.ttf");
  app->init().unwrap();

  Button but;
  but.center();

  but.on_click([&](int button, int action, int mods) {
    LOG("button is clicked");
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
      but.block.color = Color(rand, rand, rand);
    }
  });

  app->add_child(&but);
  app->run();
}
