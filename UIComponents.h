#pragma once
#include "raylib.h"

class UIComponents {
private:
    static bool drawButtonCore(const char* text, int centerX, int centerY, int width, int height, 
                               Vector2 mousePos, bool isClicked, Color normalColor, Color hoverColor, Color textColor);

public:
    static bool drawPlayButton(int centerX, int centerY, Vector2 mousePos, bool isClicked, 
                               Color normalColor, Color hoverColor, Color textColor);
                               
    static bool drawExitButton(int centerX, int centerY, Vector2 mousePos, bool isClicked, 
                               Color normalColor, Color hoverColor, Color textColor);

    static bool drawButton(const char* text, int centerX, int centerY, int width, int height, 
                           Vector2 mousePos, bool isClicked, Color normalColor, Color hoverColor, Color textColor);
};