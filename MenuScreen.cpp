#include "menuscreen.h"
#include "uicomponents.h"
#include "inputmanager.h"
#include "rlgl.h"
#include "raylib.h"
#include <cstdlib>
#include <algorithm>



MenuAction MenuScreen::render(int screenWidth, int screenHeight, int offsetX, int offsetY, int scaledWidth, int scaledHeight, InputManager& input, int nativeWidth, int nativeHeight) {
    float aspectScale = (float)scaledWidth / (float)nativeWidth;
    
    #define SCL(val) ((int)((float)(val) * aspectScale))

    const Color BACKGROUND_COLOR = hexToColor("#2A2D32", 1.0f);
    const Color PRIMARY_COLOR_100 = hexToColor("#09B58A", 1.0f);
    const Color ACCENT_LIGHT_COLOR_100 = hexToColor("#E4F4F4", 1.0f);
    const Color ACCENT_LIGHT_COLOR_80 = hexToColor("#E4F4F4", 0.8f);
    const Color ACCENT_MEDIUM_COLOR = hexToColor("#B9C9C7", 1.0f);
    const Color ACCENT_DARK_COLOR = hexToColor("#82858A", 1.0f);

    int centerX = offsetX + scaledWidth / 2; // new top left for 16:9 (0,0)
    int centerY = offsetY + scaledHeight / 2;

    DrawRectangle(offsetX, offsetY, scaledWidth, scaledHeight, BACKGROUND_COLOR); // background
    DrawRectangleGradientH(offsetX, offsetY, SCL(440), scaledHeight, hexToColor("#273A3B", 1.0f), hexToColor("#273A3B", 0.0f)); // background primary accent gradient


    DrawRectangle(offsetX + SCL(40), offsetY + SCL(20), SCL(726), SCL(4), ACCENT_LIGHT_COLOR_80); // top accent bar
    DrawTriangle(
        Vector2{(float)offsetX + SCL(740), (float)offsetY + SCL(24)},      
        Vector2{(float)offsetX + SCL(766), (float)offsetY + SCL(50)}, 
        Vector2{(float)offsetX + SCL(766), (float)offsetY + SCL(24)},         
        ACCENT_LIGHT_COLOR_80
    );
    DrawRectangleGradientH(offsetX + SCL(766), offsetY + SCL(20), SCL(1114), SCL(30), ACCENT_LIGHT_COLOR_80, hexToColor("#727A7B", 1.0f));

    DrawRectangle(offsetX + SCL(40), offsetY + SCL(40), SCL(10), SCL(200), ACCENT_LIGHT_COLOR_100);
    DrawRectangleGradientH(offsetX + SCL(50), offsetY + SCL(40), SCL(600), SCL(200), hexToColor("#16846B", 1.0f), hexToColor("#16846B", 0.0f));

    
    Vector2 mousePos = input.getMousePosition();
    bool isClicked = input.isMousePressed(MOUSE_LEFT_BUTTON);

    int playButtonY = centerY - SCL(20);
    int exitButtonY = centerY + SCL(40);

    if (UIComponents::drawPlayButton(centerX, playButtonY, mousePos, isClicked, PRIMARY_COLOR_100, ACCENT_LIGHT_COLOR_80, BACKGROUND_COLOR, aspectScale)) {
        return MenuAction::START_3D;
    }
    
    if (UIComponents::drawExitButton(centerX, exitButtonY, mousePos, isClicked, ACCENT_DARK_COLOR, ACCENT_MEDIUM_COLOR, ACCENT_LIGHT_COLOR_80, aspectScale)) {
        return MenuAction::EXIT_GAME;
    }

    #undef SCL
    
    return MenuAction::NONE;
}

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
    return (point.x >= rect.x && point.x <= rect.x + rect.width && point.y >= rect.y && point.y <= rect.y + rect.height);
}

