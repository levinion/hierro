#pragma once

#include <functional>
#include <memory>
#include <string>
#include <utility>
#include "GLFW/glfw3.h"
#include "hierro/component/component.h"
#include "hierro/utils/color.h"
#include "hierro/error.h"

class GLFWwindow;

class Application: public Component {
public:
  HierroResult<void> init();
  void run();
  bool update();
  void render();
  void destroy();

  Application* on_resize(std::function<void(int, int)> callback);
  Application* on_key(std::function<void(int, int, int, int)> callback);
  Application* on_update(std::function<bool()>);
  Application* on_render(std::function<void()>);
  Application* on_destroy(std::function<void()>);

  std::pair<int, int> window_size();

  ~Application();
  static Application* get_instance();
  std::pair<int, int> gl_version = std::pair(3, 3);
  std::string title;

  // only means window size when created,
  // use window_size instead if u want to get window size runtime
  std::pair<int, int> size = std::pair(800, 600);
  Color background = Color::rgb(0.2, 0.3, 0.3);
  bool blend = true;

  // impl Component
  float width = 1.0;
  float height = 1.0;
  float x = 0.0;
  float y = 0.0;

  std::vector<std::unique_ptr<Component>> children;
  Component* father = nullptr;

  virtual void draw() override;
  virtual std::pair<float*, float*> get_position() override;
  virtual std::pair<float*, float*> get_size() override;
  virtual std::vector<std::unique_ptr<Component>>* get_children() override;
  virtual Component** get_father() override;

  // override root position and size to break recurse
  virtual std::pair<float, float> absolute_position() override {
    auto size = this->window_size();
    return { 0, size.second };
  }

  virtual std::pair<float, float> absolute_size() override {
    return this->window_size();
  }

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
  std::function<bool()> update_callback = [] { return true; };
  std::function<void()> render_callback = [] {};
  std::function<void()> destroy_callback = [] {};
};
