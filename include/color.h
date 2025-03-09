#pragma once

class Color {
public:
  float r, g, b, a;
  static Color rgba(float r, float g, float b, float a);
  static Color rgb(float r, float g, float b);
  Color();
  ~Color();
};
