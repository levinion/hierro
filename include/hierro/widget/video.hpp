#pragma once

#include <mpv/client.h>
#include <mpv/render.h>
#include "hierro/component/component.hpp"
#include <concurrentqueue.h>
#include <mpv/render_gl.h>
#include <memory>

namespace hierro {

class Frame {
public:
  char* data;
  int size;
};

class Video: public Component {
public:
  // impl Component
  COMPONENT_OVERRIDE_METHODS
  Video();
  void push_frame(char* data, int size);
  void render();
  void send_command(std::vector<std::string> cmd);
  void terminate();

private:
  // impl Component
  Component* father = nullptr;
  Size size = { 1.0, 1.0 };
  Position position = { 0.0, 1.0 };
  std::vector<std::unique_ptr<Component>> children;

  mpv_render_context* mpv_gl;
  mpv_handle* mpv;
  std::vector<mpv_render_param> params;

  moodycamel::ConcurrentQueue<Frame> frame_stream;

  mpv_opengl_fbo mpfbo = { .fbo = 0, .w = 0, .h = 0 };
  int flip_y = 1;

  COMPONENT_DEFAULT_CALLBACK
};
} // namespace hierro
