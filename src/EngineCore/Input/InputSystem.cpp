#include "InputSystem.h"
#include "Corrade/Utility/Debug.h"
#include "Utilities/EventManager.hpp"

InputSystem::InputSystem() {
  OnMovePressed = EventManager<EmptyEventArgs>::create();
}
void InputSystem::handleKeyDown(KeyEvent &event) {
  if (event.key() == Key::D) {
    OnMovePressed->publish({});
  }
}
