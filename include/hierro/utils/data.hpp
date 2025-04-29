#pragma once

namespace hierro {

class Color {
public:
  float r, g, b, a;

  Color() {}

  Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}

  Color(float r, float g, float b) : r(r), g(g), b(b), a(1.0f) {}
};

class Position {
public:
  double x = 0.0;
  double y = 0.0;

  Position() {}

  Position(double x, double y) : x(x), y(y) {}
};

class Size {
public:
  double width = 0.0;
  double height = 0.0;

  Size() {}

  Size(double width, double height) : width(width), height(height) {}

  Size(int width, int height) : width(width), height(height) {}
};

} // namespace hierro
