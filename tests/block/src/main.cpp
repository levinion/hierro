#include <cmath>
#include <ctime>
#include "GLFW/glfw3.h"
#include "hierro/app.h"
#include "hierro/utils/color.h"
#include "hierro/component/block.h"

int main() {
  auto app = Application::get_instance();
  app->init().unwrap();
  Block block, block2;
  block2.update([](auto self) { self->color = Color::rgba(0.2, 0.9, 0.4, 0.5); }
  );
  app->on_resize([](int width, int height) {})
    ->on_update([&] {
      auto time = glfwGetTime();
      auto timemagic = std::abs(std::sin(time)); // 0-1
      block.update([&](auto self) {
        self->width = timemagic;
        self->color = Color::rgba(1, 1, 0, timemagic);
        self->radius = timemagic;
        self->set_position(timemagic, timemagic);
      });
      return true;
    })
    ->on_render([&] {
      block.draw();
      block2.draw();
    })
    ->run();
}
