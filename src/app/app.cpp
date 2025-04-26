#include <glad/glad.h>
#include "hierro/app.hpp"
#include <strings.h>
#include <chrono>
#include <functional>
#include <hierro/error.hpp>
#include <memory>
#include "hierro/component/component.hpp"
#include "hierro/utils/data.hpp"
#include "hierro/backend/backend.hpp"
#include <spdlog/spdlog.h>

namespace hierro {

std::unique_ptr<Application> Application::instance = nullptr;

Application* Application::get_instance() {
  if (Application::instance.get() == nullptr) {
    instance.reset(new Application);
  }
  return instance.get();
}

void Application::prepare() {
  backend->prepare();
}

HierroResult<void> Application::run() {
  this->prepare();
  std::chrono::steady_clock clock;
  while (!backend->should_close()) {
    auto start = clock.now();
    if (this->update())
      hierro_check(this->render());
    auto end = clock.now();
    const std::chrono::duration<double> _delta { end - start };
    auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(_delta);

    auto frame_rate = 1000.0 / delta.count();

    if (this->frame_limit && this->frame_limit.value() < frame_rate) {
      this->frame_rate = this->frame_limit.value();
      auto time_per_frame = 1000.0 / this->frame_limit.value();
      usleep((time_per_frame - delta.count()) * 1000.0);
    } else {
      this->frame_rate = frame_rate;
    }
  }
  this->destroy();
  return {};
}

bool Application::update() {
  auto a = backend->update();
  auto b = this->update_callback();
  return a || b;
}

HierroResult<void> Application::render() {
  auto& color = this->background;
  glClearColor(color.r, color.g, color.b, color.a);
  glClear(GL_COLOR_BUFFER_BIT);
  hierro_check(this->draw());
  this->render_callback();
  backend->render();
  return {};
}

void Application::destroy() {
  this->destroy_callback();
  backend->destroy();
}

Size Application::window_size() {
  assert(
    backend.get() && "backend should be initialized before calling window_size"
  );
  return backend->window_size();
}

Position Application::cursor_pos() {
  assert(
    backend.get() && "backend should be initialized before calling window_size"
  );
  return backend->cursor_pos();
}

HierroResult<void> Application::draw() {
  return this->draw_children();
}

IMPL_COMPONENT(Application);

void Application::search_focus(float x, float y) {
  std::function<void(std::unique_ptr<Component>&, float, float, Component*&)>
    range_tree = [&](
                   std::unique_ptr<Component>& node,
                   float x,
                   float y,
                   Component*& focused
                 ) {
      if (node) {
        if (node->is_hitted(x, y)) {
          focused = node.get();
        }
        for (auto& child : node->get_children()) {
          range_tree(child, x, y, focused);
        }
      }
    };

  Component* focused = this;
  for (auto& child : this->get_children()) {
    range_tree(child, x, y, focused);
  }
  this->focused = focused;

  focused->send_focus_event();
}

void Application::set_focus(Component* component) {
  this->focused = component;
}

Application* Application::add_font(std::string font) {
  this->fonts.push_back(font);
  return this;
}

void Application::maximize() {
  backend->maximize();
}

// TODO: FINISH THIS
void Application::fullscreen(bool flag) {
  backend->fullscreen(flag);
}

void Application::resize(Size size) {
  backend->resize(size);
}

} // namespace hierro
