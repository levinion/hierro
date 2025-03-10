#include <cmath>
#include <ctime>
#include "GLFW/glfw3.h"
#include "hierro/app.h"
#include "hierro/color.h"
#include "hierro/component/block.h"

int main() {
  auto app = Application::get_instance();
  app->init().unwrap();
  Block block, block2;
  block2.update([](auto self) { self->color = Color::rgba(0.2, 0.9, 0.4, 0.5); }
  );
  app
    ->on_update([&] {
      auto time = glfwGetTime();
      auto timemagic = std::abs(std::sin(time));
      block.update([&](auto self) {
        self->width = timemagic;
        self->color = Color::rgba(timemagic, timemagic, timemagic, timemagic);
        self->set_position((timemagic - 0.5) * 2, (timemagic - 0.5) * 2);
      });
    })
    ->on_render([&] {
      block.draw();
      block2.draw();
    })
    ->run();
}
