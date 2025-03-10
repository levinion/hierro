#include <functional>
#include "hierro/app.h"

void Application::frame_buffer_size_callback(
  GLFWwindow* window,
  int width,
  int height
) {
  glViewport(0, 0, width, height);
  Application::get_instance()->resize_callback(width, height);
}

Application* Application::on_resize(ResizeCallback callback) {
  this->resize_callback = callback;
  return this;
}

void Application::glfw_key_callback(
  GLFWwindow* window,
  int key,
  int scancode,
  int action,
  int mod
) {
  Application::get_instance()->key_callback(key, scancode, action, mod);
}

Application* Application::on_key(KeyCallback callback) {
  this->key_callback = callback;
  return this;
}

Application* Application::on_update(std::function<void()> callback) {
  this->update_callback = callback;
  return this;
}

Application* Application::on_render(std::function<void()> callback) {
  this->render_callback = callback;
  return this;
}
