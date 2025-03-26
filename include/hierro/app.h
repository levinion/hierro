#pragma once

#include <functional>
#include <memory>
#include <string>
#include <utility>
#include "GLFW/glfw3.h"
#include "hierro/component/component.h"
#include "hierro/error.h"
#include "hierro/utils/data.h"

class GLFWwindow;

class Application: public Component {
public:
  HierroResult<void> init();
  void run();
  void prepare();
  bool update();
  void render();
  void destroy();

  Application* on_resize(std::function<void(int, int)> callback);
  Application* on_update(std::function<bool()>);
  Application* on_render(std::function<void()>);
  Application* on_destroy(std::function<void()>);

  Size window_size();
  Position cursor_pos();

  static Application* get_instance();

  std::pair<int, int> gl_version = std::pair(3, 3);
  std::string title;

  // only means window size when created,
  // use window_size instead if u want to get window size runtime

  Color background = Color(0.2, 0.3, 0.3);
  bool blend = true;

  void set_focus(Component* component);

  // impl Component
  Size size = { 1.0f, 1.0f };
  Position position = { 0.0f, 1.0f };

  std::vector<std::unique_ptr<Component>> children;
  Component* father = nullptr;

  virtual void draw() override;
  virtual Position& get_position() override;
  virtual Size& get_size() override;
  virtual std::vector<std::unique_ptr<Component>>& get_children() override;
  virtual Component*& get_father() override;
  virtual std::function<void(int, int, int)>& get_click_callback() override;
  virtual std::function<void(int, int, int, int)>& get_key_callback() override;
  virtual std::function<void(unsigned int)>& get_input_callback() override;

  // override root position and size to break recurse
  virtual Position absolute_position() override {
    auto size = this->window_size();
    return { 0, size.height };
  }

  virtual Size absolute_size() override {
    return this->window_size();
  }

private:
  GLFWwindow* window;
  static Application* instance;
  static void
  glfw_frame_buffer_size_callback(GLFWwindow* window, int width, int height);
  static void glfw_key_callback(
    GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mod
  );
  static void glfw_mouse_button_callabck(
    GLFWwindow* window,
    int button,
    int action,
    int mods
  );
  static void glfw_char_callback(GLFWwindow* window, unsigned int codepoint);
  std::function<void(int, int)> resize_callback = [](int height, int width) {};
  std::function<bool()> update_callback = [] { return true; };
  std::function<void()> render_callback = [] {};
  std::function<void()> destroy_callback = [] {};

  std::function<void(int, int, int)> click_callback = [](int, int, int) {};
  std::function<void(int, int, int, int)> key_callback =
    [](int, int, int, int) {};
  std::function<void(unsigned int)> input_callback = [](unsigned int) {};

  void search_focus(float x, float y);
  Component* focused;
};
