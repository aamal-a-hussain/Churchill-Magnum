#ifndef INCLUDE_INPUT_INPUTSYSTEM_H_
#define INCLUDE_INPUT_INPUTSYSTEM_H_

#include <memory>
#include "Magnum/Platform/Sdl2Application.h"
#include "Utilities/EventManager.hpp"
class InputSystem {
    using KeyEvent = Magnum::Platform::Sdl2Application::KeyEvent;
    using Key = Magnum::Platform::Sdl2Application::Key;

public:
    /**
     * @brief Create an input system
     *
     * Sets up the OnMovePressed EventManager which can be passed
     * to other systems to implement the moving logic.
     */
    InputSystem();

    /**
     * @brief Handle the KeyDown event from SDL
     *
     * Switches the key type to trigger the appropriate EventManager
     */
    void handleKeyDown(KeyEvent& event);


    // MEMBERS
public:
    std::shared_ptr<EventManager<EmptyEventArgs>> OnMovePressed;
};
#endif // INCLUDE_INPUT_INPUTSYSTEM_H_
