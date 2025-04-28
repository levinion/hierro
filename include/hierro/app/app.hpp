#pragma once

#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "hierro/component/text.hpp"
#include "hierro/backend/backend.hpp"
#include "hierro/component/component.hpp"
#include "hierro/utils/data.hpp"
#include "hierro/app/window.hpp"
#include "hierro/utils/error.hpp"

namespace hierro {

class Application: public Component {
public:
  // lifetime
  template<typename T>
  HierroResult<void> init(WindowSettings settings) {
    static_assert(
      std::is_base_of<Backend, T>::value,
      "T should impl hierro::Backend"
    );
    this->backend.reset(new T);

    hierro_check(backend->init(settings));

    glViewport(0, 0, this->size.width, this->size.height);

    this->background = settings.background;
    glClearColor(background.r, background.g, background.b, background.a);

    if (settings.blend) {
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_BLEND);
    }

    this->frame_limit = settings.frame_limit;

    // init text generater
    if (!this->fonts.empty()) {
      // TODO: handle multiple fonts
      hierro_check(this->tg->init(this->fonts[0], font_size));
    }

    return {};
  }

  HierroResult<void> run();
  void prepare();
  bool update();
  HierroResult<void> render();
  void destroy();

  // custom hooks
  Application* on_resize(std::function<void(int, int)> callback);
  Application* on_update(std::function<bool()>);
  Application* on_render(std::function<void()>);
  Application* on_destroy(std::function<void()>);

  // functional method
  Size window_size();
  Position cursor_pos();

  // require the global instance and use anywhere
  static Application* get_instance();

  std::pair<int, int> gl_version = std::pair(3, 3);

  // custom method
  void set_focus(Component* component);
  Application* add_font(std::string font);
  void set_font_size(Size size);
  void fullscreen(bool flag);
  void maximize();
  void resize(Size size);

  TextGenerater* tg = TextGenerater::get_instance();
  std::vector<std::string> fonts;
  Size font_size = { 0.0, 48.0 };

  Color background = { 0.2, 0.2, 0.2 };

  // user hooks
  std::function<void(int, int)> resize_callback = [](int height, int width) {};
  std::function<bool()> update_callback = [] { return true; };
  std::function<void()> render_callback = [] {};
  std::function<void()> destroy_callback = [] {};

  // common hooks
  COMPONENT_DEFAULT_CALLBACK

  // impl Component
  // Component params
  Size size = { 1.0f, 1.0f };
  Position position = { 0.0f, 1.0f };
  std::vector<std::unique_ptr<Component>> children;
  Component* father = nullptr;

  // Component trait
  COMPONENT_OVERRIDE_METHODS

  // override root position to break recurse
  inline virtual Position absolute_position() override {
    auto size = this->window_size();
    return { 0, size.height };
  }

  // override root size to break recurse
  inline virtual Size absolute_size() override {
    return this->window_size();
  }

  inline double get_frame_rate() {
    return frame_rate;
  }

private:
  friend SDLBackend;
  friend GLFWBackend;

  static std::unique_ptr<Application> instance;

  // call this to re-search focus
  void search_focus(float x, float y);
  Component* focused = this; // focused should never be nullptr

  std::unique_ptr<Backend> backend;

  double frame_rate;
  std::optional<double> frame_limit;
};

} // namespace hierro
