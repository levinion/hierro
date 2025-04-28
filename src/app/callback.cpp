#include <cassert>
#include <functional>
#include "hierro/app/app.hpp"

namespace hierro {

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

} // namespace hierro
