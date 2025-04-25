#include "hierro/widget/video.hpp"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>
#include <mpv/client.h>
#include <mpv/render.h>
#include <mpv/render_gl.h>
#include <mpv/stream_cb.h>
#include <concurrentqueue.h>
#include "hierro/app.hpp"
#include <spdlog/spdlog.h>
#include <thread>

namespace hierro {

void Video::draw() {};

IMPL_COMPONENT(Video);

static inline void check_error(int status) {
  if (status < 0) {
    printf("mpv API error: %s\n", mpv_error_string(status));
    exit(1);
  }
}

static int64_t size_fn(void* cookie) {
  return MPV_ERROR_UNSUPPORTED;
}

static int64_t read_fn(void* cookie, char* buf, uint64_t nbytes) {
  auto* fs = static_cast<moodycamel::ConcurrentQueue<Frame>*>(cookie);
  Frame frame;
  while (fs->try_dequeue(frame) == false) {
    std::this_thread::yield();
  }
  uint64_t to_copy = std::min<uint64_t>(frame.size, nbytes);
  spdlog::debug("mpv receive frame: {}", to_copy);
  memcpy(buf, frame.data, to_copy);
  return to_copy;
}

static int64_t seek_fn(void* cookie, int64_t offset) {
  return MPV_ERROR_UNSUPPORTED;
}

static void close_fn(void* cookie) {
  auto* fs = static_cast<moodycamel::ConcurrentQueue<Frame>*>(cookie);
}

static int open_fn(void* user_data, char* uri, mpv_stream_cb_info* info) {
  auto fs = static_cast<moodycamel::ConcurrentQueue<Frame>*>(user_data);
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

Video::Video() {
  auto app = Application::get_instance();
  auto window_size = app->window_size();

  spdlog::info(
    "window_size - width: {}, height: {} ",
    window_size.width,
    window_size.height
  );

  this->mpv = mpv_create();
  mpv_set_option_string(mpv, "vo", "libmpv");
  mpv_set_option_string(mpv, "untimed", "yes");
  mpv_set_option_string(mpv, "demuxer", "rawvideo");
  mpv_set_option_string(
    mpv,
    "demuxer-rawvideo-w",
    std::to_string((int)window_size.width).c_str()
  );
  mpv_set_option_string(
    mpv,
    "demuxer-rawvideo-h",
    std::to_string((int)window_size.height).c_str()
  );
  mpv_set_option_string(mpv, "demuxer-rawvideo-mp-format", "bgra");
  mpv_set_option_string(mpv, "demuxer-rawvideo-fps", "60");
  mpv_set_option_string(mpv, "demuxer-readahead-secs", "0");
  // mpv_set_option_string(mpv, "demuxer-rawvideo-size", "33177600");
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

  mpfbo.w = window_size.width;
  mpfbo.h = window_size.height;

  std::vector<mpv_render_param> params = {
    { MPV_RENDER_PARAM_API_TYPE, (void*)MPV_RENDER_API_TYPE_OPENGL },
    { MPV_RENDER_PARAM_OPENGL_INIT_PARAMS, &op },
    // { MPV_RENDER_PARAM_ADVANCED_CONTROL, &advanced_control },
    { MPV_RENDER_PARAM_OPENGL_FBO, &mpfbo },
    { MPV_RENDER_PARAM_FLIP_Y, &flip_y },
    { MPV_RENDER_PARAM_INVALID, NULL },
  };

  check_error(mpv_render_context_create(&mpv_gl, mpv, params.data()));

  // Play this file.
  const char* cmd[] = { "loadfile", "kaleido://fake", NULL };
  check_error(mpv_command(mpv, cmd));

  this->params = params;
}

void Video::push_frame(char* data, int size) {
  frame_stream.enqueue(Frame { data, size });
  mpv_event* event = mpv_wait_event(mpv, 0);
  if (event->event_id == MPV_EVENT_LOG_MESSAGE) {
    mpv_event_log_message* msg = (mpv_event_log_message*)event->data;
    printf("[%s] %s: %s", msg->prefix, msg->level, msg->text);
  }
}

void Video::render() {
  auto flag = mpv_render_context_update(mpv_gl);
  if (flag & MPV_RENDER_UPDATE_FRAME) {
    auto app = Application::get_instance();
    auto window_size = app->window_size();
    mpfbo.w = window_size.width;
    mpfbo.h = window_size.height;
    check_error(mpv_render_context_render(mpv_gl, params.data()));
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
  mpv_destroy(mpv);
}

} // namespace hierro
