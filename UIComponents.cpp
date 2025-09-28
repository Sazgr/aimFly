#include "UIComponents.h"
#include "raylib.h"
#include <cstring> 

static bool isPointInRect(Vector2 point, Rectangle rect) {
    return (point.x >= rect.x && point.x <= rect.x + rect.width && point.y >= rect.y && point.y <= rect.y + rect.height);
}

bool UIComponents::drawButtonCore(const char* text, int centerX, int centerY, int width, int height, Vector2 mousePos, bool isClicked, Color normalColor, Color hoverColor, Color textColor, float aspectScale) {
    int scaledWidth = (int)(width * aspectScale);
    int scaledHeight = (int)(height * aspectScale);
    int scaledFontSize = (int)(20 * aspectScale);

    Rectangle rect = { (float)centerX - scaledWidth/2, (float)centerY - scaledHeight/2, (float)scaledWidth, (float)scaledHeight };
    bool isHovered = isPointInRect(mousePos, rect);
    
    DrawRectangleRec(rect, isHovered ? hoverColor : normalColor);

    int textWidth = MeasureText(text, scaledFontSize);
    DrawText(text, centerX - textWidth / 2, centerY - scaledFontSize / 2, scaledFontSize, textColor);

    return isHovered && isClicked;
}

bool UIComponents::drawPlayButton(int centerX, int centerY, Vector2 mousePos, bool isClicked, Color normalColor, Color hoverColor, Color textColor, float aspectScale) {
    const int width = 200;
    const int height = 50;
    return drawButtonCore("START PRACTICE", centerX, centerY, width, height, mousePos, isClicked, normalColor, hoverColor, textColor, aspectScale);
}

bool UIComponents::drawExitButton(int centerX, int centerY, Vector2 mousePos, bool isClicked, Color normalColor, Color hoverColor, Color textColor, float aspectScale) {
    const int width = 200;
    const int height = 50;
    return drawButtonCore("EXIT", centerX, centerY, width, height, mousePos, isClicked, normalColor, hoverColor, textColor, aspectScale);
}

bool UIComponents::drawButton(const char* text, int centerX, int centerY, int width, int height, Vector2 mousePos, bool isClicked, Color normalColor, Color hoverColor, Color textColor, float aspectScale) {
    return drawButtonCore(text, centerX, centerY, width, height, mousePos, isClicked, normalColor, hoverColor, textColor, aspectScale);
}