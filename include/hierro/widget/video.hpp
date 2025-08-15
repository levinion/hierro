#pragma once

#ifdef ENABLE_VIDEO_COMPONENT

  #include "hierro/component/block.hpp"
  #include "hierro/utils/texture.hpp"
  #include <GL/gl.h>
  #include <mpv/client.h>
  #include <mpv/render.h>
  #include "hierro/component/component.hpp"
  #include <concurrentqueue.h>
  #include <mpv/render_gl.h>
  #include <memory>

namespace hierro {

class Frame {
public:
  unsigned char* data;
  int size;
};

class FrameStream {
public:
  moodycamel::ConcurrentQueue<Frame> stream;
  std::atomic<bool> update_flag;
  std::atomic<bool> should_close;
};

class VideoSettings {
public:
  Size frame_size;
  std::string format;
  GLenum gl_format;
  bool flip_y = false;
};

class Video: public Component {
public:
  // impl Component
  COMPONENT_OVERRIDE_METHODS
  Video();
  ~Video();
  Video* init(VideoSettings);
  void update(unsigned char* data, int size);
  void render();
  void send_command(std::vector<std::string> cmd);
  void terminate();

  virtual bool is_hitted(float x, float y) override;

private:
  // impl Component
  Component* father = nullptr;
  Size size = { 1.0, 1.0 };
  Position position = { 0.0, 1.0 };
  std::vector<std::unique_ptr<Component>> children;

  mpv_render_context* mpv_gl;
  mpv_handle* mpv;
  std::vector<mpv_render_param> params;

  FrameStream frame_stream;

  mpv_opengl_fbo mpfbo = { .fbo = 0, .w = 0, .h = 0 };

  GLuint fbo = 0;
  Texture texture;
  Block* block;
  Size frame_size = { 0, 0 };

  COMPONENT_DEFAULT_CALLBACK
};
} // namespace hierro
#endif
