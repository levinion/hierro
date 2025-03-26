#pragma once

class Color {
public:
  float r, g, b, a;

  Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}

  Color(float r, float g, float b) : r(r), g(g), b(b), a(1.0f) {}
};

class Position {
public:
  double x;
  double y;

  Position(double x, double y) : x(x), y(y) {}
};

class Size {
public:
  double width;
  double height;

  Size(double width, double height) : width(width), height(height) {}
};
