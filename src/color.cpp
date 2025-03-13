#include "hierro/color.h"

Color Color::rgba(float r, float g, float b, float a) {
  Color color;
  color.r = r;
  color.g = g;
  color.b = b;
  color.a = a;
  return color;
};

Color Color::rgb(float r, float g, float b) {
  Color color;
  color.r = r;
  color.g = g;
  color.b = b;
  color.a = 1;
  return color;
};

Color::Color() {};
