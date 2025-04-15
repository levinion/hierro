#include <cmath>
#include <cstdlib>
#include <ctime>
#include <random>
#include "GLFW/glfw3.h"
#include "hierro/app.hpp"
#include "hierro/component/block.hpp"

using namespace hierro;

int main() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> random(0, 1);

#define rand random(gen)

  auto app = Application::get_instance();
  // this app did not need to render text
  app->init(800, 600).unwrap();
  Block block, block2;
  block.color = Color(rand, 0, 1);
  block.set_position(0.0, 1.0);
  block.center();
  block2.color = Color(0, 0, 1);
  block2.set_size(0.5, 0.5);
  block2.center();

  block.on_click([&](int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
      block.color = Color(rand, rand, rand);
    }
  });

  block2.on_click([&](int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
      block2.color = Color(rand, rand, rand);
    }
  });

  app->on_click([&](int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
      app->background = Color(rand, rand, rand);
    }
  });

  block.add_child(&block2);
  app->add_child(&block);
  app->run();
}
