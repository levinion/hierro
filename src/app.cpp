#include <glad/glad.h>
#include "app.h"
#include <GLFW/glfw3.h>

Application* Application::instance = nullptr;

int Application::init() {
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
    return -1;
  }

  glViewport(0, 0, this->size.first, this->size.second);

  auto& color = this->background;
  glClearColor(color.r, color.g, color.b, color.a);
  return 0;
}

bool Application::frame() {
  glfwPollEvents();
  glfwSwapBuffers(window);
  return !glfwWindowShouldClose(window);
}

void Application::destroy() {
  glfwTerminate();
}

void Application::frame_buffer_size_callback(
  GLFWwindow* window,
  int width,
  int height
) {
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

Application* Application::get_instance() {
  if (instance == nullptr) {
    instance = new Application();
  }
  return instance;
}

Application::Application() {}

Application::~Application() {}
