#pragma once

#include "inputmanager.h"
#include "uicomponents.h"

#include "raylib.h"

enum class MenuAction {
    NONE,
    START_3D, 
    EXIT_GAME
};

class MenuScreen {
private:
    Color hexToColor(const char* hex, float opacity);
    bool isPointInRect(Vector2 point, Rectangle rect);

public:
    MenuAction render(int screenWidth, int screenHeight, int offsetX, int offsetY, 
                      int scaledWidth, int scaledHeight, InputManager& input, 
                      int nativeWidth, int nativeHeight, Shader& gradientShader);
};
