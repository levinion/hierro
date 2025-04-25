#include "hierro/event/event.hpp"

namespace hierro {
bool KeyEvent::is_pressed(Key key) {
  return this->keystate->contains(key);
}
} // namespace hierro
