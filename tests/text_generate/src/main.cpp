#include <cmath>
#include <ctime>
#include <string>
#include "GLFW/glfw3.h"
#include "hierro/app.h"
#include "hierro/component/text.h"
#include "hierro/component/label.h"

int main() {
  auto app = Application::get_instance();
  app->init().unwrap();

  auto tg = TextGenerater::get_instance();
  tg->init("assets/fonts/LXGWWenKai-Regular.ttf", 48);

  Label label1, label2;

  label2.position = { 0.1, 0.1 };

  app->on_resize([&](int width, int height) { tg->viewport(width, height); })
    ->on_update([&] {
      label1.position = { 0.5, 0.5 };

      auto time = glfwGetTime();
      label1.content = std::to_string(time);
      label2.content = "你好";
      return true;
    })
    ->on_render([&] {
      label1.draw();
      label2.draw();
    })
    ->on_destroy([&] { tg->destroy(); })
    ->run();
}
