#pragma once

#include "hierro/component/block.hpp"
#include "hierro/component/component.hpp"
#include "hierro/component/label.hpp"

class Input: public Component {
public:
  // impl Component
  virtual void draw() override;
  virtual Position& get_position() override;
  virtual Size& get_size() override;
  virtual std::vector<std::unique_ptr<Component>>& get_children() override;
  virtual Component*& get_father() override;
  virtual std::function<void(int, int, int)>& get_click_callback() override;
  virtual std::function<void(int, int, int, int)>& get_key_callback() override;
  virtual std::function<void(unsigned int)>& get_input_callback() override;
  virtual std::function<void()>& get_focus_callback() override;

  virtual bool is_hitted(float x, float y) override;
  Input();

  Block block;
  Label label;

private:
  // impl Component
  Component* father = nullptr;
  Size size = { 0.25f, 0.25f };
  Position position = { 0.5 - size.width / 2, 0.5 + size.height / 2 };
  std::vector<std::unique_ptr<Component>> children;
  std::function<void(int, int, int)> click_callback = [](int, int, int) {};
  std::function<void(int, int, int, int)> key_callback =
    [](int, int, int, int) {};
  std::function<void(unsigned int)> input_callback = [](unsigned int) {};
  std::function<void()> focus_callback = [] {};
};
