#include <cmath>
#include <ctime>
#include "hierro/app.h"
#include "hierro/component/block.h"
#include "hierro/utils/color.h"

int main() {
  auto app = Application::get_instance();
  app->init().unwrap();
  Block block, block2;
  block.color = Color::rgb(1, 0, 1);
  block.set_position(0.0, 1.0);
  block.center();
  block2.color = Color::rgb(0, 0, 1);
  block2.set_size(0.5, 0.5);
  block2.center();
  block.add_child(&block2);
  app->add_child(&block);
  app->run();
}
