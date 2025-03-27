#include <cassert>
#include <functional>
#include "GLFW/glfw3.h"
#include "hierro/app.hpp"

void Application::glfw_frame_buffer_size_callback(
  GLFWwindow* window,
  int width,
  int height
) {
  glViewport(0, 0, width, height);
  auto app = Application::get_instance();
  app->tg->viewport(width, height);
  app->resize_callback(width, height);
}

void Application::glfw_key_callback(
  GLFWwindow* window,
  int key,
  int scancode,
  int action,
  int mod
) {
  auto app = Application::get_instance();
  assert(app->focused);
  app->focused->send_key_event(key, scancode, action, mod);
}

void Application::glfw_mouse_button_callabck(
  GLFWwindow* window,
  int button,
  int action,
  int mods
) {
  auto app = Application::get_instance();
  assert(app->focused);
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
    auto [x, y] = app->cursor_pos();
    app->search_focus(x, y);
  }
  // trigger callback of focused element
  if (app->focused) {
    app->focused->send_click_event(button, action, mods);
  }
}

void Application::glfw_char_callback(
  GLFWwindow* window,
  unsigned int codepoint
) {
  auto app = Application::get_instance();
  if (app->focused) {
    app->focused->send_input_event(codepoint);
  }
}

Application* Application::on_resize(std::function<void(int, int)> callback) {
  this->resize_callback = callback;
  return this;
}

Application* Application::on_update(std::function<bool()> callback) {
  this->update_callback = callback;
  return this;
}

Application* Application::on_render(std::function<void()> callback) {
  this->render_callback = callback;
  return this;
}

Application* Application::on_destroy(std::function<void()> callback) {
  this->destroy_callback = callback;
  return this;
}
