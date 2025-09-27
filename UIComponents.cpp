#include "UIComponents.h"
#include "raylib.h"
#include <cstring> 

static bool isPointInRect(Vector2 point, Rectangle rect) {
    return (point.x >= rect.x && point.x <= rect.x + rect.width && point.y >= rect.y && point.y <= rect.y + rect.height);
}

bool UIComponents::drawButtonCore(const char* text, int centerX, int centerY, int width, int height, Vector2 mousePos, bool isClicked, 
                                  Color normalColor, Color hoverColor, Color textColor) {
    Rectangle rect = { (float)centerX - width/2, (float)centerY - height/2, (float)width, (float)height };
    bool isHovered = isPointInRect(mousePos, rect);
    
    DrawRectangleRec(rect, isHovered ? hoverColor : normalColor);

    int fontSize = 20;
    int textWidth = MeasureText(text, fontSize);
    DrawText(text, centerX - textWidth / 2, centerY - fontSize / 2, fontSize, textColor);

    return isHovered && isClicked;
}

bool UIComponents::drawPlayButton(int centerX, int centerY, Vector2 mousePos, bool isClicked, 
                                  Color normalColor, Color hoverColor, Color textColor) {
    const int width = 200;
    const int height = 50;
    return drawButtonCore("START PRACTICE", centerX, centerY, width, height, mousePos, isClicked, normalColor, hoverColor, textColor);
}

bool UIComponents::drawExitButton(int centerX, int centerY, Vector2 mousePos, bool isClicked, 
                                  Color normalColor, Color hoverColor, Color textColor) {
    const int width = 200;
    const int height = 50;
    return drawButtonCore("EXIT", centerX, centerY, width, height, mousePos, isClicked, normalColor, hoverColor, textColor);
}

bool UIComponents::drawButton(const char* text, int centerX, int centerY, int width, int height, Vector2 mousePos, bool isClicked,
                              Color normalColor, Color hoverColor, Color textColor) {
    return drawButtonCore(text, centerX, centerY, width, height, mousePos, isClicked, normalColor, hoverColor, textColor);
}