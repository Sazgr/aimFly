#include "uicomponents.h"
#include "raylib.h"
#include "raymath.h"
#include <cstring> 

static bool isPointInRect(Vector2 point, Rectangle rect) {
    return (point.x >= rect.x && point.x <= rect.x + rect.width && point.y >= rect.y && point.y <= rect.y + rect.height);
}

void UIComponents::drawGradientRect(Shader shader, Rectangle rect, Color c1, Color c2, bool vertical) {
    BeginShaderMode(shader);

    Vector4 start = { c1.r/255.0f, c1.g/255.0f, c1.b/255.0f, c1.a/255.0f };
    Vector4 end   = { c2.r/255.0f, c2.g/255.0f, c2.b/255.0f, c2.a/255.0f };
    Vector4 bounds = { rect.x, rect.y, rect.width, rect.height };

    SetShaderValue(shader, GetShaderLocation(shader, "startColor"), &start, SHADER_UNIFORM_VEC4);
    SetShaderValue(shader, GetShaderLocation(shader, "endColor"), &end, SHADER_UNIFORM_VEC4);
    SetShaderValue(shader, GetShaderLocation(shader, "rectBounds"), &bounds, SHADER_UNIFORM_VEC4);

    int vertFlag = vertical ? 1 : 0;
    SetShaderValue(shader, GetShaderLocation(shader, "vertical"), &vertFlag, SHADER_UNIFORM_INT);

    DrawRectangleRec(rect, WHITE);

    EndShaderMode();
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