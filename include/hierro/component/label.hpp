#pragma once

#include <memory>
#include <string>
#include <vector>
#include "hierro/component/text.hpp"
#include "hierro/utils/data.hpp"
#include "hierro/component/component.hpp"

namespace hierro {

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

  COMPONENT_OVERRIDE_METHODS

  // label should never be treated as a normal ui element, since its size cannot got easily, and it should be binded with a container(such as block)
  virtual bool is_hitted(float x, float y) override {
    return false;
  }

private:
  COMPONENT_DEFAULT_CALLBACK
};

} // namespace hierro
