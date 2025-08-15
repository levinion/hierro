#pragma once

#ifdef ENABLE_SDL_BACKEND

  #include <SDL3/SDL_video.h>
  #include "hierro/backend/backend.hpp"
  #include "hierro/utils/data.hpp"

namespace hierro {
class SDLBackend: public Backend {
public:
  virtual std::expected<void, std::string>
  init(WindowSettings settings) override;
  virtual bool should_close() override;
  virtual void prepare() override;
  virtual bool update() override;
  virtual void render() override;
  virtual void destroy() override;
  virtual void resize(Size size) override;
  virtual void maximize() override;
  virtual void minimize() override;
  virtual void fullscreen(bool flag) override;
  virtual Size window_size() override;
  virtual Position window_position() override;
  virtual Position cursor_pos() override;
  virtual void set_vsync(bool flag) override;

private:
  SDL_Window* window;
  std::pair<int, int> gl_version = std::pair(3, 3);
  Size size = { 1.0f, 1.0f };
  Position position = { 0.0f, 1.0f };
  bool sdl_should_close = false;
};
} // namespace hierro

#endif
