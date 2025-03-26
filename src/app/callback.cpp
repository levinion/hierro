#include <functional>
#include "hierro/app.h"

void Application::glfw_frame_buffer_size_callback(
  GLFWwindow* window,
  int width,
  int height
) {
  glViewport(0, 0, width, height);
  Application::get_instance()->resize_callback(width, height);
}

Application* Application::on_resize(std::function<void(int, int)> callback) {
  this->resize_callback = callback;
  return this;
}

void Application::glfw_key_callback(
  GLFWwindow* window,
  int key,
  int scancode,
  int action,
  int mod
) {}

void Application::glfw_mouse_button_callabck(
  GLFWwindow* window,
  int button,
  int action,
  int mods
) {
  auto app = Application::get_instance();
  // find the most above component and it should action on the event;
  // and hook on app itself should always action on the event as a global hook;

  // set focused element
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
    auto [x, y] = app->cursor_pos();
    app->search_focus(x, y);
  }
  // trigger callback of focused element
  if (app->focused != nullptr) {
    (app->focused->get_click_callback())(button, action, mods);
  }
  // trigger global callback
  (app->get_click_callback())(button, action, mods);
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
