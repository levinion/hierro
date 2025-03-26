#include <cmath>
#include <cstdlib>
#include <ctime>
#include "GLFW/glfw3.h"
#include "hierro/app.hpp"
#include "hierro/component/block.hpp"
#include "hierro/component/label.hpp"
#include "hierro/utils/log.hpp"

int main() {
  auto app = Application::get_instance();
  app->add_font("assets/fonts/LXGWWenKai-Regular.ttf");
  app->init().unwrap();

  Block container;
  Label input;

  container.set_size(0.6, 0.3);
  container.center();

  input.set_position(0.1, 1);
  input.set_size(0.8, 1);

  input.content = L"";
  container.on_input([&](unsigned int codepoint) {
    LOG("codepoint get", codepoint);
    input.content.push_back(codepoint);
  });

  container.on_key([&](int key, int scancode, int action, int mod) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
      if (key == GLFW_KEY_BACKSPACE) {
        LOG("press backspace");
        if (!input.content.empty()) {
          input.content.pop_back();
        }
      }
    }

    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
      LOG("press enter");
      input.content.push_back('\n');
    }
  });

  // never set focus to the text element
  app->set_focus(&container);
  container.add_child(&input);
  app->add_child(&container);
  app->run();
}
