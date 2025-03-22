#pragma once

#include <string>
#include "hierro/utils/color.h"

class Label {
public:
  Label();

  Label(std::string content) : content(content) {}

  void draw();
  float height();

  std::string content;
  std::pair<float, float> position = { 0.5, 0.5 }; // (x, y)
  std::pair<float, float> size = { 1, 1 }; // (width, height)
  float scale = 1.0f;
  Color color = Color::rgb(1, 1, 1);
  bool wrap = false;
  float spacing = 1.0f;
  float line_spacing = 1.5f;

private:
};
