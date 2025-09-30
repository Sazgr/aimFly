#pragma once

#include "raylib.h"

#include <unordered_map>

enum class KeyState {
    UP,
    PRESSED,
    HELD,
    RELEASED
};

class InputManager {
public:
    void update();  // update on per frame basis

    // types
    bool isKeyPressed(int key);
    bool isKeyReleased(int key);
    bool isKeyHeld(int key);

    bool isMousePressed(int button);
    bool isMouseReleased(int button);
    bool isMouseHeld(int button);

    Vector2 getMouseDelta() const { return mouseDelta; }
    Vector2 getMousePosition() const { return mousePosition; }

private:
    std::unordered_map<int, KeyState> keyStates;
    std::unordered_map<int, KeyState> mouseStates;

    Vector2 mouseDelta = {0, 0};
    Vector2 mousePosition = {0, 0};

    void updateKey(int key);
    void updateMouse(int button);
};