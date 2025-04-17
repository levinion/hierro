#include <glad/glad.h>
#include "hierro/app.hpp"
#include <GLFW/glfw3.h>
#include <strings.h>
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

HierroResult<void> Application::init(WindowSettings settings) {
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
    return err("GLAD ERROR: glad cannot get load gl proc address.");
  }

  // set viewport
  glViewport(0, 0, this->size.width, this->size.height);

  // set clean color
  this->background = settings.background;
  glClearColor(background.r, background.g, background.b, background.a);

  // enable blend mode
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
  // render only if dirty
  glfwSwapBuffers(window);
  glfwPollEvents();
  return this->update_callback();
}

void Application::render() {
  glClearColor(background.r, background.g, background.b, background.a);
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
      __range_tree(child, x, y, focused);
    }
  }
}

void Application::search_focus(float x, float y) {
  Component* focused = this;
  for (auto& child : this->get_children()) {
    __range_tree(child, x, y, focused);
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
  glfwMaximizeWindow(window);
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

void Application::resize(Size size) {
  glfwSetWindowSize(window, size.width, size.height);
}

} // namespace hierro
