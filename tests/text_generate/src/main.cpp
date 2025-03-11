#include <cmath>
#include <ctime>
#include <string>
#include "GLFW/glfw3.h"
#include "hierro/app.h"
#include "hierro/component/text.h"

int main() {
  auto app = Application::get_instance();
  app->init().unwrap();

  auto tg = TextGenerater::get_instance();
  tg->init("assets/fonts/LXGWWenKai-Regular.ttf");

  std::string text = "hello";

  app->on_resize([&](int width, int height) { tg->viewport(width, height); })
    ->on_update([&] {})
    ->on_render([&] {
      auto time = glfwGetTime();
      tg->draw_text(
        std::to_string(time),
        app->window_size().first / 2,
        app->window_size().second / 2,
        1,
        Color::rgb(0, 0, 0)
      );
    })
    ->run();
}
