#pragma once

#include <memory>
#include <string>
#include <vector>
#include "hierro/utils/data.h"
#include "hierro/component/component.h"

class Label: public Component {
public:
  Label();

  Label(std::string content) : content(content) {}

  std::string content;
  float scale = 1.0f;
  Color color = Color(1, 1, 1);
  bool wrap = true;
  float spacing = 1.0f;
  float line_spacing = 1.5f;

  // impl Component
  Size size = { 1.0f, 1.0f };
  Position position = { 0.0f, 1.0f };
  std::vector<std::unique_ptr<Component>> children;
  Component* father = nullptr;
  std::function<void(int, int, int)> click_callback = [](int, int, int) {};

  virtual void draw() override;
  virtual Position& get_position() override;
  virtual Size& get_size() override;
  virtual std::vector<std::unique_ptr<Component>>& get_children() override;
  virtual Component*& get_father() override;
  virtual std::function<void(int, int, int)>& get_click_callback() override;

private:
};
