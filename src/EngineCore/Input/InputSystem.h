#ifndef INCLUDE_INPUT_INPUTSYSTEM_H_
#define INCLUDE_INPUT_INPUTSYSTEM_H_

#include "Magnum/Platform/Sdl2Application.h"
#include "Utilities/EventManager.hpp"
#include <memory>
class InputSystem {
  using KeyEvent = Magnum::Platform::Sdl2Application::KeyEvent;
  using Key = Magnum::Platform::Sdl2Application::Key;

public:
  InputSystem();
  void handleKeyDown(KeyEvent &event);
  std::shared_ptr<EventManager<EmptyEventArgs>> OnMovePressed;

private:
};
#endif // INCLUDE_INPUT_INPUTSYSTEM_H_
