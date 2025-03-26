#include <glad/glad.h>
#include "hierro/app.h"
#include <GLFW/glfw3.h>
#include <hierro/error.h>
#include <text_encoding>
#include "hierro/component/component.h"
#include "hierro/utils/data.h"

Application* Application::instance = nullptr;

Application* Application::get_instance() {
  if (Application::instance == nullptr) {
    Application::instance = new Application();
  }
  return Application::instance;
}

HierroResult<void> Application::init() {
  glfwInit();
  glfwInitHint(GLFW_CONTEXT_VERSION_MAJOR, this->gl_version.first);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->gl_version.second);
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(
    // TODO: give an option to set initial window size
    800,
    600,
    this->title.c_str(),
    NULL,
    NULL
  );
  glfwMakeContextCurrent(window);

  this->window = window;

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    return err("GLAD ERROR: glad cannot get load gl proc address.");
  }

  glViewport(0, 0, this->size.width, this->size.height);

  auto& color = this->background;
  glClearColor(color.r, color.g, color.b, color.a);

  if (this->blend) {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
  }

  return ok();
}

void Application::prepare() {
  glfwSetFramebufferSizeCallback(window, this->glfw_frame_buffer_size_callback);
  glfwSetKeyCallback(window, this->glfw_key_callback);
  glfwSetMouseButtonCallback(window, this->glfw_mouse_button_callabck);
}

void Application::run() {
  this->prepare();
  while (!glfwWindowShouldClose(window)) {
    if (this->update())
      this->render();
  }
  this->destroy();
}

bool Application::update() {
  glfwSwapBuffers(window);
  glfwPollEvents();
  return this->update_callback();
}

void Application::render() {
  auto& color = this->background;
  glClearColor(color.r, color.g, color.b, color.a);
  glClear(GL_COLOR_BUFFER_BIT);
  this->draw();
  this->render_callback();
}

void Application::destroy() {
  this->destroy_callback();
  glfwTerminate();
}

Size Application::window_size() {
  int width, height;
  glfwGetWindowSize(this->window, &width, &height);
  return { (double)width, (double)height };
}

Position Application::cursor_pos() {
  double xpos, ypos;
  glfwGetCursorPos(this->window, &xpos, &ypos);
  return { xpos, ypos };
}

void Application::draw() {
  this->draw_children();
}

IMPL_COMPONENT(Application);

// 前向遍历
void __range_tree(Component* node, float x, float y, Component*& focused) {
  if (node) {
    if (node->is_hitted(x, y)) {
      focused = node;
    }
    for (auto& child : node->get_children()) {
      __range_tree(child.get(), x, y, focused);
    }
  }
}

void Application::search_focus(float x, float y) {
  Component* focused = nullptr;
  // skip Application, so that focused will be either nullptr or other component
  for (auto& child : this->get_children()) {
    __range_tree(child.get(), x, y, focused);
  }
  this->focused = focused;
}
