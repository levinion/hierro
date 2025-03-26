#include <cmath>
#include <ctime>
#include <string>
#include "hierro/app.h"
#include "hierro/component/text.h"
#include "hierro/component/label.h"

int main() {
  auto app = Application::get_instance();
  app->init().unwrap();

  auto tg = TextGenerater::get_instance();
  tg->init("assets/fonts/LXGWWenKai-Regular.ttf", 48);

  Label label1, label2;

  label1.color = Color(1, 0, 1);
  label1.set_position(0.1, 0.1);
  label1.content = L"Hello";

  label2.set_position(0.1, 0.9);
  label2.color = Color(1, 1, 0);
  label2.content = L"你好";

  app->add_child(&label1)->add_child(&label2);
  app->on_resize([&](int width, int height) { tg->viewport(width, height); })
    ->on_destroy([&] { tg->destroy(); })
    ->run();
}
