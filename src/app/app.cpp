#include <glad/glad.h>
#include "hierro/app.h"
#include <GLFW/glfw3.h>
#include <hierro/error.h>

Application* Application::instance = nullptr;

Application* Application::get_instance() {
  if (Application::instance == nullptr) {
    Application::instance = new Application();
  }
  return Application::instance;
}

HierroResult Application::init() {
  glfwInit();
  glfwInitHint(GLFW_CONTEXT_VERSION_MAJOR, this->version.first);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->version.second);
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(
    this->size.first,
    this->size.second,
    this->title.c_str(),
    NULL,
    NULL
  );
  glfwMakeContextCurrent(window);

  this->window = window;

  glfwSetFramebufferSizeCallback(window, this->frame_buffer_size_callback);
  glfwSetKeyCallback(window, this->glfw_key_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    return HierroResult::err(HierroError::GLADERROR);
  }

  glViewport(0, 0, this->size.first, this->size.second);

  auto& color = this->background;
  glClearColor(color.r, color.g, color.b, color.a);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);

  return HierroResult::ok();
}

void Application::run() {
  while (this->update()) {
    this->render();
  }
  this->destroy();
}

bool Application::update() {
  glfwPollEvents();
  glfwSwapBuffers(window);
  this->update_callback();
  return !glfwWindowShouldClose(window);
}

void Application::render() {
  glClear(GL_COLOR_BUFFER_BIT);
  this->render_callback();
}

void Application::destroy() {
  glfwTerminate();
}

Application::Application() {}

Application::~Application() {}

std::pair<int, int> Application::window_size() {
  int width, height;
  glfwGetWindowSize(this->window, &width, &height);
  return { width, height };
}
