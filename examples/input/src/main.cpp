#include <cmath>
#include <cstdlib>
#include <ctime>
#include <random>
#include "GLFW/glfw3.h"
#include "hierro/app.h"
#include "hierro/component/block.h"
#include "hierro/component/label.h"
#include "hierro/component/text.h"

int main() {
  auto app = Application::get_instance();
  app->init().unwrap();

  auto tg = TextGenerater::get_instance();
  tg->init("assets/fonts/LXGWWenKai-Regular.ttf", 48);

  Block container;
  Label content;

  content.content = "place holder";

  container.add_child(&content);
  app->add_child(&container);

  app->on_resize([&](int width, int height) { tg->viewport(width, height); });
  app->run();
}
