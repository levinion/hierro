#include "hierro/app/app.hpp"
#include "hierro/event/event.hpp"
#include "hierro/utils/texture.hpp"
#include "hierro/widget/video.hpp"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>
#include <mpv/client.h>
#include <mpv/render.h>
#include <mpv/render_gl.h>
#include <mpv/stream_cb.h>
#include <concurrentqueue.h>
#include <spdlog/spdlog.h>

namespace hierro {

HierroResult<void> Video::draw() {
  return {};
};

IMPL_COMPONENT(Video);

static inline void check_error(int status) {
  if (status < 0) {
    spdlog::error("mpv api error: {}", mpv_error_string(status));
    exit(1);
  }
}

static int64_t size_fn(void* cookie) {
  return MPV_ERROR_UNSUPPORTED;
}

static int64_t read_fn(void* cookie, char* buf, uint64_t nbytes) {
  auto* fs = static_cast<FrameStream*>(cookie);
  Frame frame;
  while (fs->stream.try_dequeue(frame) == false) {
    if (fs->should_close)
      return 0;
    fs->update_flag.wait(false);
    fs->update_flag.store(false);
  }
  uint64_t to_copy = std::min<uint64_t>(frame.size, nbytes);
  memcpy(buf, frame.data, to_copy);
  return to_copy;
}

static int64_t seek_fn(void* cookie, int64_t offset) {
  return MPV_ERROR_UNSUPPORTED;
}

static void close_fn(void* cookie) {}

static int open_fn(void* user_data, char* uri, mpv_stream_cb_info* info) {
  auto fs = static_cast<FrameStream*>(user_data);
  info->cookie = fs;
  info->size_fn = size_fn;
  info->read_fn = read_fn;
  info->seek_fn = seek_fn;
  info->close_fn = close_fn;
  return 0;
}

static void* get_proc_address_mpv(void* fn_ctx, const char* name) {
  return (void*)SDL_GL_GetProcAddress(name);
}

Video::Video() {}

void Video::init(VideoSettings settings) {
  this->frame_size = settings.frame_size;

  // init block;
  this->block = this->add_child<Block>();
  block->set_size(1.0, 1.0);
  block->center();
  if (settings.flip_y)
    block->flip_y();

  // bind events
  block->on_input([&](unsigned int codepoint) {
    this->send_input_event(codepoint);
  });
  block->on_key([&](KeyEvent e) { this->send_key_event(e); });
  block->on_click([&](ClickEvent e) { this->send_click_event(e); });
  block->on_mouse_move([&](MouseMoveEvent e) { this->send_mouse_move_event(e); }
  );
  block->on_mouse_wheel([&](MouseWheelEvent e) {
    this->send_mouse_wheel_event(e);
  });

  auto app = Application::get_instance();
  auto window_size = app->window_size();

  this->mpv = mpv_create();
  mpv_set_option_string(mpv, "vo", "libmpv");
  mpv_set_option_string(mpv, "untimed", "yes");
  mpv_set_option_string(mpv, "demuxer", "rawvideo");
  mpv_set_option_string(
    mpv,
    "demuxer-rawvideo-w",
    std::to_string((int)frame_size.width).c_str()
  );
  mpv_set_option_string(
    mpv,
    "demuxer-rawvideo-h",
    std::to_string((int)frame_size.height).c_str()
  );
  mpv_set_option_string(
    mpv,
    "demuxer-rawvideo-mp-format",
    settings.format.c_str()
  );
  mpv_set_option_string(mpv, "demuxer-rawvideo-fps", "60");
  mpv_set_option_string(mpv, "demuxer-readahead-secs", "0");
  if (mpv_initialize(mpv) < 0) {
    spdlog::error("mpv initialize failed");
  }
  mpv_request_log_messages(mpv, "warn");

  check_error(mpv_stream_cb_add_ro(mpv, "kaleido", &this->frame_stream, open_fn)
  );

  auto op = (mpv_opengl_init_params) {
    .get_proc_address = get_proc_address_mpv,
  };
  // int advanced_control = 1;

  // create fbo
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  auto texture =
    Texture(NULL, frame_size.width, frame_size.height, settings.gl_format);
  glFramebufferTexture2D(
    GL_FRAMEBUFFER,
    GL_COLOR_ATTACHMENT0,
    GL_TEXTURE_2D,
    texture.id(),
    0
  );

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    spdlog::error("fbo failed created");
  }
  // reset fbo to 0
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  mpfbo.w = frame_size.width;
  mpfbo.h = frame_size.height;
  mpfbo.fbo = this->fbo;
  this->texture = texture;

  block->set_texture(this->texture);

  std::vector<mpv_render_param> params = {
    { MPV_RENDER_PARAM_API_TYPE, (void*)MPV_RENDER_API_TYPE_OPENGL },
    { MPV_RENDER_PARAM_OPENGL_INIT_PARAMS, &op },
    // { MPV_RENDER_PARAM_ADVANCED_CONTROL, &advanced_control },
    { MPV_RENDER_PARAM_OPENGL_FBO, &mpfbo },
    { MPV_RENDER_PARAM_INVALID, NULL },
  };

  check_error(mpv_render_context_create(&mpv_gl, mpv, params.data()));

  this->params = params;

  // Play this file.
  const char* cmd[] = { "loadfile", "kaleido://fake", NULL };
  check_error(mpv_command(mpv, cmd));
}

void Video::update(unsigned char* data, int size) {
  frame_stream.stream.enqueue(Frame { data, size });
  frame_stream.update_flag.store(true);
  frame_stream.update_flag.notify_one();

  // handle event
  mpv_event* event = mpv_wait_event(mpv, 0);
  if (event->event_id == MPV_EVENT_LOG_MESSAGE) {
    mpv_event_log_message* msg = (mpv_event_log_message*)event->data;
    spdlog::warn("[{}] {}: {}", msg->prefix, msg->level, msg->text);
  }
}

void Video::render() {
  auto flag = mpv_render_context_update(mpv_gl);
  if (flag & MPV_RENDER_UPDATE_FRAME) {
    auto app = Application::get_instance();
    auto window_size = app->window_size();
    mpfbo.w = frame_size.width;
    mpfbo.h = frame_size.height;
    glViewport(0, 0, frame_size.width, frame_size.height);
    check_error(mpv_render_context_render(mpv_gl, params.data()));
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, window_size.width, window_size.height);
  }
}

void Video::send_command(std::vector<std::string> cmd) {
  std::vector<const char*> cstrings;
  for (auto& s : cmd) {
    auto data = s.c_str();
    cstrings.push_back(data);
  };
  cstrings.push_back(0);
  mpv_command(mpv, cstrings.data());
}

void Video::terminate() {
  this->frame_stream.should_close = true;
  // notify to prevent dead lock
  frame_stream.update_flag.store(true);
  frame_stream.update_flag.notify_one();
  mpv_render_context_free(mpv_gl);
  mpv_terminate_destroy(mpv);
}

bool Video::is_hitted(float x, float y) {
  return false;
}

Video::~Video() {
  this->texture.free();
  glDeleteFramebuffers(1, &fbo);
}
} // namespace hierro
