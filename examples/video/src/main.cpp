#include "hierro/hierro.hpp"
#include "hierro/widget/video.hpp"
#include <stb_image.h>
#include <spdlog/spdlog.h>

using namespace hierro;

int main() {
  auto app = Application::get_instance();

  WindowSettings ws;
  app->init<SDLBackend>(ws).value();

  auto video = app->add_child<Video>();

  int w1, h1, c1;
  auto p1 = stbi_load("assets/p1.jpg", &w1, &h1, &c1, 0);

  VideoSettings vs;
  vs.frame_size = Size(w1, h1);
  vs.flip_y = true;
  vs.format = "rgb24";
  vs.gl_format = GL_RGB;

  video->init(vs);
  video->set_size(0.5, 0.5);
  video->center();

  app
    ->on_update([&]() {
      spdlog::info(
        "p1 size : {}, w: {}, h: {}, c: {}",
        w1 * h1 * c1,
        w1,
        h1,
        c1
      );
      video->update(p1, w1 * h1 * c1);
      return true;
    })
    ->on_render([&]() {
      video->render();
      spdlog::info("frame_rate: {}", app->get_frame_rate());
    })
    ->run()
    .value();
}
