#include <glad/glad.h>
#include "hierro/app.hpp"
#include <GLFW/glfw3.h>
#include <hierro/error.hpp>
#include "hierro/component/component.hpp"
#include "hierro/utils/data.hpp"

namespace hierro {

Application* Application::instance = nullptr;

Application* Application::get_instance() {
  if (Application::instance == nullptr) {
    Application::instance = new Application();
  }
  return Application::instance;
}

HierroResult<void> Application::init(int width, int height) {
  // init GLFW
  glfwInit();
  glfwInitHint(GLFW_CONTEXT_VERSION_MAJOR, this->gl_version.first);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->gl_version.second);
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // create window
  GLFWwindow* window =
    glfwCreateWindow(width, height, this->title.c_str(), NULL, NULL);
  glfwMakeContextCurrent(window);

  // save window
  this->window = window;

  // require opengl api with glad
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    return err("GLAD ERROR: glad cannot get load gl proc address.");
  }

  // set viewport
  glViewport(0, 0, this->size.width, this->size.height);

  // set clean color
  auto& color = this->background;
  glClearColor(color.r, color.g, color.b, color.a);

  // enable blend mode
  if (this->blend) {
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
  glfwSetFramebufferSizeCallback(window, this->glfw_frame_buffer_size_callback);
  glfwSetKeyCallback(window, this->glfw_key_callback);
  glfwSetMouseButtonCallback(window, this->glfw_mouse_button_callabck);
  glfwSetCharCallback(window, this->glfw_char_callback);
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

  // std::function<void(std::unique_ptr<Component>&)> range =
  //   [&](std::unique_ptr<Component>& node) {
  //     if (node->get_children().size() != 0) {
  //       for (auto& child : node->get_children()) {
  //         range(node);
  //       }
  //     } else {
  //       delete node.release();
  //     }
  //   };

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
  Component* focused = this;
  for (auto& child : this->get_children()) {
    __range_tree(child.get(), x, y, focused);
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

// TODO: FINISH THIS
void Application::fullscreen() {
  auto monitor = glfwGetPrimaryMonitor();
  auto size = window_size();
  glfwSetWindowMonitor(
    window,
    monitor,
    0,
    0,
    size.width,
    size.height,
    GLFW_DONT_CARE
  );
}

} // namespace hierro
