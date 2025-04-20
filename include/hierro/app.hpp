#pragma once

#include <functional>
#include <string>
#include <utility>
#include <vector>
#include "hierro/backend/backend.hpp"
#include "hierro/component/component.hpp"
#include "hierro/component/text.hpp"
#include "hierro/error.hpp"
#include "hierro/event/event.hpp"
#include "hierro/utils/data.hpp"
#include "hierro/window.hpp"

namespace hierro {

class Application: public Component {
public:
  // lifetime
  HierroResult<void> init(WindowSettings settings);
  void run();
  void prepare();
  bool update();
  void render();
  void destroy();

  // custom hooks
  Application* on_resize(std::function<void(int, int)> callback);
  Application* on_update(std::function<bool()>);
  Application* on_render(std::function<void()>);
  Application* on_destroy(std::function<void()>);

  // functional method
  Size window_size();
  Position cursor_pos();

  // require the global instance and use anywhere
  static Application* get_instance();

  std::pair<int, int> gl_version = std::pair(3, 3);

  // custom method
  void set_focus(Component* component);
  Application* add_font(std::string font);
  void set_font_size(Size size);
  void fullscreen(bool flag);
  void maximize();
  void resize(Size size);

  TextGenerater* tg = TextGenerater::get_instance();
  std::vector<std::string> fonts;
  Size font_size = { 0.0, 48.0 };

  Color background = { 0.2, 0.2, 0.2 };

  // user hooks
  std::function<void(int, int)> resize_callback = [](int height, int width) {};
  std::function<bool()> update_callback = [] { return true; };
  std::function<void()> render_callback = [] {};
  std::function<void()> destroy_callback = [] {};

  // common hooks
  std::function<void(ClickEvent)> click_callback = [](ClickEvent) {};
  std::function<void(KeyEvent)> key_callback = [](KeyEvent) {};
  std::function<void(unsigned int)> input_callback = [](unsigned int) {};
  std::function<void()> focus_callback = [] {};
  std::function<void(MouseMoveEvent)> mouse_move_callback = [](MouseMoveEvent) {
  };
  std::function<void(MouseWheelEvent)> mouse_wheel_callback =
    [](MouseWheelEvent) {};

  // impl Component
  // Component params
  Size size = { 1.0f, 1.0f };
  Position position = { 0.0f, 1.0f };
  std::vector<Component*> children;
  Component* father = nullptr;

  // Component trait
  virtual void draw() override;
  virtual Position& get_position() override;
  virtual Size& get_size() override;
  virtual std::vector<Component*>& get_children() override;
  virtual Component*& get_father() override;
  virtual std::function<void(ClickEvent)>& get_click_callback() override;
  virtual std::function<void(KeyEvent)>& get_key_callback() override;
  virtual std::function<void(unsigned int)>& get_input_callback() override;
  virtual std::function<void()>& get_focus_callback() override;
  virtual std::function<void(MouseMoveEvent)>&
  get_mouse_move_callback() override;
  virtual std::function<void(MouseWheelEvent)>&
  get_mouse_wheel_callback() override;

  // override root position to break recurse
  virtual Position absolute_position() override {
    auto size = this->window_size();
    return { 0, size.height };
  }

  // override root size to break recurse
  virtual Size absolute_size() override {
    return this->window_size();
  }

private:
  friend SDLBackend;
  friend GLFWBackend;

  static Application* instance;

  // call this to re-search focus
  void search_focus(float x, float y);
  Component* focused = this; // focused should never be nullptr

  Backend* backend = new GLFWBackend {};
};

} // namespace hierro
