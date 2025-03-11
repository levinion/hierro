#include <string>
#include "hierro/color.h"

class Label {
public:
  Label();

  Label(std::string content) : content(content) {}

  void draw();

  std::string content;
  std::pair<float, float> position = { 0.5, 0.5 }; // (x, y)
  std::pair<float, float> size = { 1, 1 }; // (width, height)
  float scale = 1.0f;
  Color color = Color::rgb(1, 1, 1);
  bool wrap = false;
  float line_spacing = 1.5f;

private:
};
