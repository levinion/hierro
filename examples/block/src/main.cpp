#include <cmath>
#include <cstdlib>
#include <ctime>
#include <random>
#include "GLFW/glfw3.h"
#include "hierro/app.h"
#include "hierro/component/block.h"

int main() {
  auto app = Application::get_instance();
  app->init().unwrap();
  Block block, block2;
  block.color = Color(1, 0, 1);
  block.set_position(0.0, 1.0);
  block.center();
  block2.color = Color(0, 0, 1);
  block2.set_size(0.5, 0.5);
  block2.center();

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> rand(0, 1);

  block.on_click([&](int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
      block.color = Color(rand(gen), rand(gen), rand(gen));
    }
  });

  block2.on_click([&](int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
      block2.color = Color(rand(gen), rand(gen), rand(gen));
    }
  });

  app->on_click([&](int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
      app->background = Color(rand(gen), rand(gen), rand(gen));
    }
  });

  block.add_child(&block2);
  app->add_child(&block);
  app->run();
}
