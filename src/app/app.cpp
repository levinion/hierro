#include <glad/glad.h>
#include "hierro/app.hpp"
#include <strings.h>
#include <functional>
#include <hierro/error.hpp>
#include "hierro/component/component.hpp"
#include "hierro/utils/data.hpp"
#include "hierro/backend/backend.hpp"

namespace hierro {

Application* Application::instance = nullptr;

Application* Application::get_instance() {
  if (Application::instance == nullptr) {
    Application::instance = new Application();
  }
  return Application::instance;
}

HierroResult<void> Application::init(WindowSettings settings) {
  this->backend = settings.backend;
  auto result = backend->init(settings);
  if (!result.has_value()) {
    return result;
  }

  glViewport(0, 0, this->size.width, this->size.height);

  this->background = settings.background;
  glClearColor(background.r, background.g, background.b, background.a);

  if (settings.blend) {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
  }

  // init text generater
  if (!this->fonts.empty()) {
    // TODO: handle multiple fonts
    this->tg->init(this->fonts[0], font_size);
  }

  return ok();
}

void Application::prepare() {
  backend->prepare();
}

void Application::run() {
  this->prepare();
  while (!backend->should_close()) {
    if (this->update())
      this->render();
  }
  this->destroy();
}

bool Application::update() {
  auto a = backend->update();
  auto b = this->update_callback();
  return a || b;
}

void Application::render() {
  auto& color = this->background;
  glClearColor(color.r, color.g, color.b, color.a);
  glClear(GL_COLOR_BUFFER_BIT);
  this->draw();
  backend->render();
  this->render_callback();
}

void Application::destroy() {
  this->destroy_callback();
  backend->destroy();
}

Size Application::window_size() {
  return backend->window_size();
}

Position Application::cursor_pos() {
  return backend->cursor_pos();
}

void Application::draw() {
  this->draw_children();
}

IMPL_COMPONENT(Application);

void Application::search_focus(float x, float y) {
  std::function<void(Component*, float, float, Component*&)> range_tree =
    [&](Component* node, float x, float y, Component*& focused) {
      if (node) {
        if (node->is_hitted(x, y)) {
          focused = node;
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
