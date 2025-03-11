#pragma once

#include <functional>
#include <string>
#include <utility>
#include "GLFW/glfw3.h"
#include "color.h"
#include "hierro/error.h"

class GLFWwindow;

class Application {
public:
  HierroResult init();
  void run();
  bool update();
  void render();
  void destroy();

  Application* on_resize(std::function<void(int, int)> callback);
  Application* on_key(std::function<void(int, int, int, int)> callback);
  Application* on_update(std::function<void()>);
  Application* on_render(std::function<void()>);

  std::pair<int, int> window_size();

  ~Application();
  static Application* get_instance();
  std::pair<int, int> version = std::pair(3, 3);
  std::string title;
  std::pair<int, int> size = std::pair(800, 600);
  Color background = Color::rgb(0.2, 0.3, 0.3);

private:
  Application();
  GLFWwindow* window;
  static Application* instance;
  static void
  frame_buffer_size_callback(GLFWwindow* window, int width, int height);
  static void glfw_key_callback(
    GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mod
  );
  std::function<void(int, int)> resize_callback = [](int height, int width) {};
  std::function<void(int, int, int, int)> key_callback =
    [](int key, int scancode, int action, int mod) {};
  std::function<void()> update_callback = [] {};
  std::function<void()> render_callback = [] {};
};
