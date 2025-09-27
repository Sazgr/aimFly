#include "menuscreen.h"
#include "uicomponents.h"
#include "inputmanager.h"
#include "rlgl.h"
#include "raylib.h"
#include <cstdlib>
#include <algorithm>

Color MenuScreen::hexToColor(const char* hex, float opacity) {
    if (hex[0] == '#') hex++;
    
    unsigned int hexValue = strtoul(hex, nullptr, 16);
    
    Color color;
    color.r = (hexValue >> 16) & 0xFF;
    color.g = (hexValue >> 8) & 0xFF;
    color.b = hexValue & 0xFF;
    color.a = (unsigned char)(255 * opacity);
    
    return color;
}

bool MenuScreen::isPointInRect(Vector2 point, Rectangle rect) {
    return (point.x >= rect.x && point.x <= rect.x + rect.width &&
            point.y >= rect.y && point.y <= rect.y + rect.height);
}

MenuAction MenuScreen::render(int screenWidth, int screenHeight, int offsetX, int offsetY, int scaledWidth, int scaledHeight, InputManager& input, int nativeWidth, int nativeHeight) {
    const Color BACKGROUND_COLOR = hexToColor("#2A2D32", 1.0f);
    const Color PRIMARY_COLOR = hexToColor("#09B58A", 1.0f);
    const Color ACCENT_LIGHT_COLOR = hexToColor("#E4F4F4", 1.0f);
    const Color ACCENT_MEDIUM_COLOR = hexToColor("#B9C9C7", 1.0f);
    const Color ACCENT_DARK_COLOR = hexToColor("#82858A", 1.0f);

    rlViewport(offsetX, offsetY, scaledWidth, scaledHeight);

    DrawRectangle(0, 0, scaledWidth, scaledHeight, BACKGROUND_COLOR);
    
    float aspectScale = (float)scaledWidth / (float)nativeWidth;

    #define SCL(val) ((int)((float)(val) * aspectScale))

    DrawRectangleGradientH(0, 0, SCL(440), scaledHeight, PRIMARY_COLOR, BACKGROUND_COLOR);
    
    int centerX = scaledWidth / 2;
    int centerY = scaledHeight / 2;
    
    Vector2 mousePos = input.getMousePosition();
    mousePos.x -= offsetX;
    mousePos.y -= offsetY;
    
    bool isClicked = input.isMousePressed(MOUSE_LEFT_BUTTON);
    
    Color titleColor = ACCENT_LIGHT_COLOR;
    int titleX = SCL(nativeWidth / 2 - 100);
    int titleY = SCL(nativeHeight / 2 - 120);
    DrawText("AIMFLY", titleX, titleY, SCL(48), titleColor);
    
    int playButtonY = centerY - SCL(20);
    int exitButtonY = centerY + SCL(40);

    if (UIComponents::drawPlayButton(centerX, playButtonY, mousePos, isClicked, PRIMARY_COLOR, ACCENT_LIGHT_COLOR, BACKGROUND_COLOR, aspectScale)) {
        return MenuAction::START_3D;
    }
    
    if (UIComponents::drawExitButton(centerX, exitButtonY, mousePos, isClicked, ACCENT_DARK_COLOR, ACCENT_MEDIUM_COLOR, ACCENT_LIGHT_COLOR, aspectScale)) {
        return MenuAction::EXIT_GAME;
    }
    
    Color versionColor = hexToColor("#82858A", 0.6f);
    int versionX = SCL(nativeWidth - 60);
    int versionY = SCL(nativeHeight - 30);
    DrawText("v1.0", versionX, versionY, SCL(16), versionColor);

    #undef SCL
    
    return MenuAction::NONE;
}