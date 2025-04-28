#include "glad/glad.h"
#include "hierro/backend/sdl.hpp"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_hints.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_video.h>
#include "hierro/app/app.hpp"
#include "hierro/utils/error.hpp"
#include "hierro/event/event.hpp"
#include "hierro/app/window.hpp"

namespace hierro {
HierroResult<void> SDLBackend::init(WindowSettings settings) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    auto message = "SDL ERROR: " + std::string(SDL_GetError());
    return hierro_err(message);
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gl_version.first);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, gl_version.second);
  // SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  SDL_PropertiesID props = SDL_CreateProperties();
  if (props == 0) {
    auto message =
      "Unable to create properties: %s" + std::string(SDL_GetError());
    return hierro_err(message);
  }

  SDL_SetStringProperty(
    props,
    SDL_PROP_WINDOW_CREATE_TITLE_STRING,
    settings.title.c_str()
  );
  SDL_SetNumberProperty(
    props,
    SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER,
    settings.size.width
  );
  SDL_SetNumberProperty(
    props,
    SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER,
    settings.size.height
  );

  SDL_SetBooleanProperty(props, SDL_PROP_WINDOW_CREATE_OPENGL_BOOLEAN, true);

  SDL_SetBooleanProperty(
    props,
    SDL_PROP_WINDOW_CREATE_RESIZABLE_BOOLEAN,
    settings.resizable
  );
  SDL_SetBooleanProperty(
    props,
    SDL_PROP_WINDOW_CREATE_MAXIMIZED_BOOLEAN,
    settings.maximized
  );
  SDL_SetBooleanProperty(
    props,
    SDL_PROP_WINDOW_CREATE_ALWAYS_ON_TOP_BOOLEAN,
    settings.floating
  );

  SDL_SetBooleanProperty(
    props,
    SDL_PROP_WINDOW_CREATE_BORDERLESS_BOOLEAN,
    !settings.decorated
  );

  SDL_SetBooleanProperty(
    props,
    SDL_PROP_WINDOW_CREATE_TRANSPARENT_BOOLEAN,
    !settings.transparent
  );

  SDL_SetBooleanProperty(
    props,
    SDL_PROP_WINDOW_CREATE_FULLSCREEN_BOOLEAN,
    settings.fullscreen
  );

  if (settings.x11_window.has_value()) {
    SDL_SetNumberProperty(
      props,
      SDL_PROP_WINDOW_CREATE_X11_WINDOW_NUMBER,
      settings.x11_window.value()
    );
  }

  auto sdl_window = SDL_CreateWindowWithProperties(props);

  if (!sdl_window) {
    return hierro_err(
      "SDL ERROR: Cannot create window. " + std::string(SDL_GetError())
    );
  }

  SDL_GLContext gl_context = SDL_GL_CreateContext(sdl_window);
  if (!gl_context) {
    return hierro_err(
      "SDL ERROR: Cannot create GL context. " + std::string(SDL_GetError())
    );
  }

  SDL_GL_MakeCurrent(sdl_window, gl_context);

  this->window = sdl_window;

  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
    return hierro_err("GLAD ERROR: Cannot load OpenGL.");
  }

  return {};
}

void SDLBackend::prepare() {}

bool SDLBackend::should_close() {
  return sdl_should_close;
}

bool SDLBackend::update() {
  // render only if dirty
  SDL_Event event;
  auto app = Application::get_instance();
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_EVENT_QUIT:
      case SDL_EVENT_WINDOW_CLOSE_REQUESTED: {
        this->sdl_should_close = true;
        break;
      }
      case SDL_EVENT_KEY_DOWN:
      case SDL_EVENT_KEY_UP: {
        KeyEvent e;
        e.key = static_cast<Key>(event.key.scancode);
        // handle keystate
        if (this->keystate.contains(e.key)) {
          this->keystate.erase(e.key);
        } else {
          this->keystate.set(e.key);
        }
        e.press = event.key.down;
        e.keystate = &this->keystate;
        app->focused->send_key_event(e);
        break;
      }
      case SDL_EVENT_MOUSE_BUTTON_DOWN:
      case SDL_EVENT_MOUSE_BUTTON_UP: {
        auto app = Application::get_instance();
        auto cursor_pos = app->cursor_pos();
        if (event.button.button == SDL_BUTTON_LEFT && event.button.down) {
          app->search_focus(cursor_pos.x, cursor_pos.y);
        }
        ClickEvent e;
        e.button = static_cast<MouseButton>(event.button.button);
        e.press = event.button.down;
        e.position = { event.button.x, event.button.y };
        app->focused->send_click_event(e);
        break;
      }
      case SDL_EVENT_TEXT_INPUT: {
        // TODO: char* to keycode
        // app->focused->input_callback(event.text.text);
        break;
      }
      case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED: {
        auto size = window_size();
        glViewport(0, 0, size.width, size.height);
        app->tg->viewport(size.width, size.height);
        app->resize_callback(size.width, size.height);
        break;
      }
      case SDL_EVENT_MOUSE_MOTION: {
        MouseMoveEvent e;
        e.xrel = event.motion.xrel;
        e.yrel = event.motion.yrel;
        e.position = { event.motion.x, event.motion.y };
        app->focused->send_mouse_move_event(e);
        break;
      }
      case SDL_EVENT_MOUSE_WHEEL: {
        MouseWheelEvent e;
        e.y = event.wheel.y;
        app->focused->send_mouse_wheel_event(e);
        break;
      }
    }
  }
  return true;
}

void SDLBackend::render() {
  SDL_GL_SwapWindow((SDL_Window*)this->window);
}

void SDLBackend::destroy() {
  SDL_Quit();
}

void SDLBackend::resize(Size size) {
  glViewport(0, 0, size.width, size.height);
  auto app = Application::get_instance();
  app->tg->viewport(size.width, size.height);
  SDL_SetWindowSize(window, size.width, size.height);
}

void SDLBackend::maximize() {
  SDL_MaximizeWindow(window);
}

void SDLBackend::minimize() {
  SDL_MinimizeWindow(window);
}

void SDLBackend::fullscreen(bool flag) {
  SDL_SetWindowFullscreen(window, flag);
}

Size SDLBackend::window_size() {
  int width, height;
  SDL_GetWindowSize(window, &width, &height);
  return { (double)width, (double)height };
};

Position SDLBackend::cursor_pos() {
  float xpos, ypos;
  SDL_GetMouseState(&xpos, &ypos);
  return { xpos, ypos };
};

} // namespace hierro
