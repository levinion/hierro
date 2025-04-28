#pragma once

#include <memory>
#include "hierro/component/block.hpp"
#include "hierro/component/component.hpp"
#include "hierro/component/label.hpp"

namespace hierro {

class Button: public Component {
public:
  // impl Component
  COMPONENT_OVERRIDE_METHODS

  virtual bool is_hitted(float x, float y) override;

  Button();

  Block* block;
  Label* label;

private:
  // impl Component
  Component* father = nullptr;
  Size size = { 0.25f, 0.25f };
  Position position = { 0.5 - size.width / 2, 0.5 + size.height / 2 };
  std::vector<std::unique_ptr<Component>> children;
  COMPONENT_DEFAULT_CALLBACK
};

} // namespace hierro
