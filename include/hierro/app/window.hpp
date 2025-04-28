#pragma once
#include "hierro/utils/data.hpp"
#include <optional>
#include <string>

namespace hierro {

class WindowSettings {
public:
  Size size = { 800.0, 600.0 };
  Position position = { 0, 0 };
  bool maximized = false;
  bool resizable = true;
  bool floating = false;
  bool decorated = true;
  bool passthrough = false;
  bool transparent = false;
  bool fullscreen = false;
  std::string title = "hierro";
  Color background = Color(0.2, 0.3, 0.3);
  bool blend = true;
  std::optional<double> frame_limit;
  // only with sdl backend
  std::optional<unsigned long> x11_window = {};
};

} // namespace hierro
