#pragma once

#include <memory>
#include <string>
#include <vector>
#include "hierro/utils/color.h"
#include "hierro/component/component.h"

class Label: public Component {
public:
  Label();

  Label(std::string content) : content(content) {}

  std::string content;
  float scale = 1.0f;
  Color color = Color::rgb(1, 1, 1);
  bool wrap = false;
  float spacing = 1.0f;
  float line_spacing = 1.5f;

  // impl Component
  float width = 1.0f;
  float height = 1.0f;
  float x = 0.0f;
  float y = 1.0f;
  std::vector<std::unique_ptr<Component>> children;
  Component* father = nullptr;

  virtual void draw() override;
  virtual std::pair<float*, float*> get_position() override;
  virtual std::pair<float*, float*> get_size() override;
  virtual std::vector<std::unique_ptr<Component>>* get_children() override;
  virtual Component** get_father() override;

private:
};
