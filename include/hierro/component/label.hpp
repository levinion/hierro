#pragma once

#include <memory>
#include <string>
#include <vector>
#include "hierro/component/text.hpp"
#include "hierro/utils/data.hpp"
#include "hierro/component/component.hpp"

class Label: public Component {
public:
  Label();

  Label(std::wstring content) : content(content) {}

  std::wstring content;
  float scale = 1.0f;
  Color color = Color(1, 1, 1);
  bool wrap = true;
  bool overflow = true;
  float spacing = 1.0f;
  float line_spacing = 1.5f;
  VerticalAlign vertical_align = { VerticalAlign::Center };
  HorizontalAlign horizontal_align = { HorizontalAlign::Center };

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
  virtual std::function<void()>& get_focus_callback() override;

  // label should never be treated as a normal ui element, since its size cannot got easily, and it should be binded with a container(such as block)
  virtual bool is_hitted(float x, float y) override {
    return false;
  }

private:
  std::function<void(int, int, int)> click_callback = [](int, int, int) {};
  std::function<void(int, int, int, int)> key_callback =
    [](int, int, int, int) {};
  std::function<void(unsigned int)> input_callback = [](unsigned int) {};
  std::function<void()> focus_callback = [] {};
};
