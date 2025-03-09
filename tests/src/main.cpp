#include "glad/glad.h"
#include "hierro/app.h"
#include "hierro/color.h"
#include "hierro/component/block.h"

int main() {
  auto app = Application::get_instance();
  app->on_resize([](int width, int height) { glViewport(0, 0, width, height); }
  )->init();
  // app->init();

  Block block1, block2;
  block2.update([](Block* self) { self->center(); });
  block1.update([](Block* self) {
    self->width = 1;
    self->color = Color::rgba(0.8, 0.2, 0.3, 0.7);
    self->center();
  });

  while (app->frame()) {
    glClear(GL_COLOR_BUFFER_BIT);
    block1.draw();
    block2.draw();
  }
  app->destroy();
}
