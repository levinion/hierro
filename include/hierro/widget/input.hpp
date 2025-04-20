#pragma once

#include "hierro/component/block.hpp"
#include "hierro/component/component.hpp"
#include "hierro/component/label.hpp"
#include "hierro/event/event.hpp"

namespace hierro {

class Input: public Component {
public:
  // impl Component
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

  virtual bool is_hitted(float x, float y) override;
  Input();

  Block block;
  Label label;

private:
  // impl Component
  Component* father = nullptr;
  Size size = { 0.25f, 0.25f };
  Position position = { 0.5 - size.width / 2, 0.5 + size.height / 2 };
  std::vector<Component*> children;
  std::function<void(ClickEvent)> click_callback = [](ClickEvent) {};
  std::function<void(KeyEvent)> key_callback = [](KeyEvent) {};
  std::function<void(unsigned int)> input_callback = [](unsigned int) {};
  std::function<void()> focus_callback = [] {};
  std::function<void(MouseMoveEvent)> mouse_move_callback = [](MouseMoveEvent) {
  };
  std::function<void(MouseWheelEvent)> mouse_wheel_callback =
    [](MouseWheelEvent) {};
};

} // namespace hierro
