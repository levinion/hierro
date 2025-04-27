#pragma once

#include <string>
#include <expected>
#include "hierro/utils/data.hpp"

namespace hierro {
class GLFWBackend;
class SDLBackend;
class WindowSettings;

class Backend {
public:
  virtual ~Backend() = default;

  virtual std::expected<void, std::string> init(WindowSettings settings) = 0;
  virtual void prepare() = 0;
  virtual bool update() = 0;
  virtual void render() = 0;
  virtual void destroy() = 0;
  virtual bool should_close() = 0;
  virtual void resize(Size size) = 0;
  virtual void minimize() = 0;
  virtual void maximize() = 0;
  virtual void fullscreen(bool flag) = 0;
  virtual Size window_size() = 0;
  virtual Position cursor_pos() = 0;

  GLFWBackend* as_glfw() {
    return reinterpret_cast<GLFWBackend*>(this);
  }

  SDLBackend* as_sdl() {
    return reinterpret_cast<SDLBackend*>(this);
  }

private:
};

} // namespace hierro
