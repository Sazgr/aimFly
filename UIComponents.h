#pragma once
#include "raylib.h"

class UIComponents {
private:
    static Shader gradientShader;
    static int color1Loc;
    static int color2Loc;

    static bool drawButtonCore(const char* text, int centerX, int centerY, int width, int height, 
                               Vector2 mousePos, bool isClicked, Color normalColor, Color hoverColor, Color textColor, float aspectScale);

public:
    static bool drawPlayButton(int centerX, int centerY, Vector2 mousePos, bool isClicked, 
                               Color normalColor, Color hoverColor, Color textColor, float aspectScale);
                               
    static bool drawExitButton(int centerX, int centerY, Vector2 mousePos, bool isClicked, 
                               Color normalColor, Color hoverColor, Color textColor, float aspectScale);

    static bool drawButton(const char* text, int centerX, int centerY, int width, int height, 
                           Vector2 mousePos, bool isClicked, Color normalColor, Color hoverColor, Color textColor, float aspectScale);
    
    static void drawGradientRect(Shader shader, Rectangle rect, Color c1, Color c2, bool vertical);
};