#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "hierro/backend/glfw.hpp"
#include <expected>
#include <string>
#include "hierro/event/event.hpp"
#include "hierro/utils/data.hpp"
#include "hierro/window.hpp"
#include "hierro/app.hpp"

namespace hierro {
std::expected<void, std::string> GLFWBackend::init(WindowSettings settings) {
  // init GLFW
  glfwInit();
  glfwInitHint(GLFW_CONTEXT_VERSION_MAJOR, this->gl_version.first);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->gl_version.second);
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  glfwWindowHint(GLFW_MAXIMIZED, settings.maximized ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint(GLFW_RESIZABLE, settings.resizable ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint(GLFW_FLOATING, settings.floating ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint(
    GLFW_MOUSE_PASSTHROUGH,
    settings.passthrough ? GLFW_TRUE : GLFW_FALSE
  );
  glfwWindowHint(
    GLFW_TRANSPARENT_FRAMEBUFFER,
    settings.transparent ? GLFW_TRUE : GLFW_FALSE
  );

  glfwWindowHint(GLFW_DECORATED, settings.transparent ? GLFW_TRUE : GLFW_FALSE);

  // create window
  GLFWwindow* window = glfwCreateWindow(
    settings.size.width,
    settings.size.height,
    settings.title.c_str(),
    NULL,
    NULL
  );
  glfwMakeContextCurrent(window);

  // save window
  this->window = window;

  // move window
  glfwSetWindowPos(window, settings.position.x, settings.position.y);

  // require opengl api with glad
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    return std::unexpected("GLAD ERROR: glad cannot get load gl proc address.");
  }

  return {};
}

void GLFWBackend::prepare() {
  glfwSetFramebufferSizeCallback(window, this->glfw_frame_buffer_size_callback);
  glfwSetKeyCallback(window, this->glfw_key_callback);
  glfwSetMouseButtonCallback(window, this->glfw_mouse_button_callabck);
  glfwSetCharCallback(window, this->glfw_char_callback);
}

bool GLFWBackend::should_close() {
  return glfwWindowShouldClose(window);
}

bool GLFWBackend::update() {
  // render only if dirty
  glfwPollEvents();
  return true;
}

void GLFWBackend::render() {
  glfwSwapBuffers(window);
}

void GLFWBackend::destroy() {
  glfwTerminate();
}

void GLFWBackend::resize(Size size) {
  glViewport(0, 0, size.width, size.height);
  auto app = Application::get_instance();
  app->tg->viewport(size.width, size.height);
  glfwSetWindowSize(window, size.width, size.height);
}

void GLFWBackend::maximize() {
  glfwMaximizeWindow(window);
}

void GLFWBackend::minimize() {
  glfwIconifyWindow(window);
}

void GLFWBackend::fullscreen(bool flag) {}

Size GLFWBackend::window_size() {
  int width, height;
  glfwGetWindowSize(this->window, &width, &height);
  return { (double)width, (double)height };
};

Position GLFWBackend::cursor_pos() {
  double xpos, ypos;
  glfwGetCursorPos(this->window, &xpos, &ypos);
  return { xpos, ypos };
};

void GLFWBackend::glfw_frame_buffer_size_callback(
  GLFWwindow* window,
  int width,
  int height
) {
  glViewport(0, 0, width, height);
  auto app = Application::get_instance();
  app->tg->viewport(width, height);
  app->resize_callback(width, height);
}

void GLFWBackend::glfw_key_callback(
  GLFWwindow* window,
  int key,
  int scancode,
  int action,
  int mod
) {
  auto app = Application::get_instance();
  assert(app->focused);
  KeyEvent e;
  e.key = static_cast<Key>(scancode);
  app->focused->send_key_event(e);
}

void GLFWBackend::glfw_mouse_button_callabck(
  GLFWwindow* window,
  int button,
  int action,
  int mods
) {
  auto app = Application::get_instance();
  auto cursor_pos = app->cursor_pos();
  assert(app->focused);
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    app->search_focus(cursor_pos.x, cursor_pos.y);
  }
  // trigger callback of focused element
  assert(app->focused);
  ClickEvent e;

  e.button = static_cast<MouseButton>(button - 1);
  e.press = action;
  e.position = cursor_pos;
  app->focused->send_click_event(e);
}

void GLFWBackend::glfw_char_callback(
  GLFWwindow* window,
  unsigned int codepoint
) {
  auto app = Application::get_instance();
  assert(app->focused);
  app->focused->send_input_event(codepoint);
}
} // namespace hierro
