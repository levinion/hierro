#include "hierro/hierro.hpp"

using namespace hierro;

int main() {
  auto app = Application::get_instance();
  WindowSettings settings;
  // this app did not need to render text
  app->init<SDLBackend>(settings).value();
  auto block = app->add_child<Block>();
  block->set_size(1, 1);
  block->center();
  block->flip_y();

  auto texture = Texture("assets/p1.jpg", GL_RGB);

  block->set_texture(texture);

  app->run().value();
}
