#pragma once

#include "GLFW/glfw3.h"
#include "hierro/backend/backend.hpp"
#include "hierro/utils/data.hpp"

namespace hierro {
class GLFWBackend: public Backend {
public:
  virtual hierro::HierroResult<void> init(WindowSettings settings) override;
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
  virtual Position cursor_pos() override;

private:
  GLFWwindow* window;
  std::pair<int, int> gl_version = std::pair(3, 3);
  Size size = { 1.0f, 1.0f };
  Position position = { 0.0f, 1.0f };

  // glfw hooks
  static void
  glfw_frame_buffer_size_callback(GLFWwindow* window, int width, int height);
  static void glfw_key_callback(
    GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mod
  );
  static void glfw_mouse_button_callabck(
    GLFWwindow* window,
    int button,
    int action,
    int mods
  );
  static void glfw_char_callback(GLFWwindow* window, unsigned int codepoint);

  // TODO: glfw mouse move callback
};
} // namespace hierro
