#include "inputmanager.h"

void InputManager::update() {
    mouseDelta = GetMouseDelta();
    mousePosition = GetMousePosition();

    int keysToCheck[] = {KEY_W, KEY_A, KEY_S, KEY_D, KEY_ESCAPE, KEY_ENTER}; // keys to check
    for (int key : keysToCheck) updateKey(key);

    int mouseButtons[] = {MOUSE_LEFT_BUTTON, MOUSE_RIGHT_BUTTON}; // mouse things to check
    for (int button : mouseButtons) updateMouse(button);
}

void InputManager::updateKey(int key) {
    if (IsKeyPressed(key)) keyStates[key] = KeyState::PRESSED;
    else if (IsKeyReleased(key)) keyStates[key] = KeyState::RELEASED;
    else if (IsKeyDown(key)) keyStates[key] = KeyState::HELD;
    else keyStates[key] = KeyState::UP;
}

void InputManager::updateMouse(int button) {
    if (IsMouseButtonPressed(button)) mouseStates[button] = KeyState::PRESSED;
    else if (IsMouseButtonReleased(button)) mouseStates[button] = KeyState::RELEASED;
    else if (IsMouseButtonDown(button)) mouseStates[button] = KeyState::HELD;
    else mouseStates[button] = KeyState::UP;
}

// queries
bool InputManager::isKeyPressed(int key)    { return keyStates[key] == KeyState::PRESSED; }
bool InputManager::isKeyReleased(int key)   { return keyStates[key] == KeyState::RELEASED; }
bool InputManager::isKeyHeld(int key)       { return keyStates[key] == KeyState::HELD; }

bool InputManager::isMousePressed(int btn)  { return mouseStates[btn] == KeyState::PRESSED; }
bool InputManager::isMouseReleased(int btn) { return mouseStates[btn] == KeyState::RELEASED; }
bool InputManager::isMouseHeld(int btn)     { return mouseStates[btn] == KeyState::HELD; }