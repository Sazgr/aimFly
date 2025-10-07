#include "menuscreen.h"
#include "uicomponents.h"
#include "inputmanager.h"
#include "rlgl.h"
#include "raylib.h"
#include <cstdlib>
#include <algorithm>

MenuAction MenuScreen::render(int screenWidth, int screenHeight, int offsetX, int offsetY, int scaledWidth, int scaledHeight, InputManager& input, int nativeWidth, int nativeHeight, Shader& gradientShader) {
    float aspectScale = (float)scaledWidth / (float)nativeWidth;
    
    #define SCL(val) ((int)((float)(val) * aspectScale))

    const Color BACKGROUND_COLOR = hexToColor("#2A2D32", 1.0f);
    const Color PRIMARY_COLOR_100 = hexToColor("#09B58A", 1.0f);
    const Color PRIMARY_COLOR_10 = hexToColor("#09B58A", 0.1f);
	
	const Color GRAY_0_COLOR_100 = hexToColor("#5A6164", 1.0f);
	
	const Color GRAY_1_COLOR_100 = hexToColor("#82858A", 1.0f);
	
	const Color GRAY_2_COLOR_100 = hexToColor("#9A9DA2", 1.0f);
	
	const Color GRAY_3_COLOR_100 = hexToColor("#B9C9C7", 1.0f);
    const Color GRAY_3_COLOR_80 = hexToColor("#B9C9C7", 0.8f);
	
    const Color GRAY_4_COLOR_100 = hexToColor("#E4F4F4", 1.0f);
    const Color GRAY_4_COLOR_80 = hexToColor("#E4F4F4", 0.8f);
    const Color GRAY_4_COLOR_40 = hexToColor("#E4F4F4", 0.4f);

    int centerX = offsetX + scaledWidth / 2; // new top left for 16:9 (0,0)
    int centerY = offsetY + scaledHeight / 2;

    DrawRectangle(offsetX, offsetY, scaledWidth, scaledHeight, BACKGROUND_COLOR); // background
   
    UIComponents::drawGradientRect( // background primary accent gradient
        gradientShader, 
        Rectangle{ (float)offsetX, (float)offsetY, (float)SCL(500), (float)scaledHeight },
        hexToColor("#273A3B", 1.0f),
        BACKGROUND_COLOR, 
        false
    );

    DrawRectangle(offsetX + SCL(40), offsetY + SCL(20), SCL(726), SCL(4), GRAY_4_COLOR_80); // top accent bar
    DrawTriangle(
        Vector2{(float)offsetX + SCL(740), (float)offsetY + SCL(24)},
        Vector2{(float)offsetX + SCL(766), (float)offsetY + SCL(50)},
        Vector2{(float)offsetX + SCL(766), (float)offsetY + SCL(24)},
        GRAY_4_COLOR_80
    );
    UIComponents::drawGradientRect(
        gradientShader, 
        Rectangle{ (float)offsetX + SCL(766), (float)offsetY + SCL(20), (float)SCL(1114), (float)SCL(30) },
        GRAY_4_COLOR_80,
        GRAY_4_COLOR_40, 
        false
    );
	DrawRectangle(offsetX + SCL(1527), offsetY + SCL(30), SCL(100), SCL(10), GRAY_3_COLOR_80); //top accent bar details
	DrawRectangle(offsetX + SCL(1647), offsetY + SCL(30), SCL(100), SCL(10), PRIMARY_COLOR_100);
	DrawRectangle(offsetX + SCL(1767), offsetY + SCL(30), SCL(100), SCL(10), GRAY_3_COLOR_80);

    DrawRectangle(offsetX + SCL(40), offsetY + SCL(40), SCL(10), SCL(200), GRAY_4_COLOR_100); // title gradient vertical start
    UIComponents::drawGradientRect( // title gradient backing
        gradientShader, 
        Rectangle{ (float)offsetX + SCL(50), (float)offsetY + SCL(40), (float)SCL(600), (float)SCL(200) },
        hexToColor("#16846B", 1.0f),
        hexToColor("#16846B", 0.0f), 
        false
    );
	
	DrawRectangle(offsetX + SCL(757), offsetY + SCL(109), SCL(200), SCL(2), GRAY_2_COLOR_100); // stat panel accent line
	DrawLineEx(Vector2{static_cast<float>(offsetX + SCL(956)), static_cast<float>(offsetY + SCL(110))}, Vector2{static_cast<float>(offsetX + SCL(1018)), static_cast<float>(offsetY + SCL(170))}, 3.5f, GRAY_2_COLOR_100);
	DrawRectangle(offsetX + SCL(1017), offsetY + SCL(169), SCL(800), SCL(2), GRAY_2_COLOR_100);
	
	DrawRectangle(offsetX + SCL(761), offsetY + SCL(219), SCL(4), SCL(4), GRAY_3_COLOR_80); // stat panel accent
	DrawRectangle(offsetX + SCL(780), offsetY + SCL(219), SCL(213), SCL(4), GRAY_3_COLOR_80);
	
	for (int i{}; i < 5; ++i) {
		DrawRectangle(offsetX + SCL(20), offsetY + SCL(422 + i * 100), SCL(300), SCL(2), GRAY_1_COLOR_100); //selection column 1 separator bars
		DrawTriangle(
			Vector2{(float)offsetX + SCL(67), (float)offsetY + SCL(424 + i * 100)},
			Vector2{(float)offsetX + SCL(67), (float)offsetY + SCL(428 + i * 100)},
			Vector2{(float)offsetX + SCL(71), (float)offsetY + SCL(424 + i * 100)},
			GRAY_1_COLOR_100
		);
		DrawRectangle(offsetX + SCL(20), offsetY + SCL(424 + i * 100), SCL(47), SCL(4), GRAY_1_COLOR_100);
		
		DrawRectangle(offsetX + SCL(10), offsetY + SCL(434 + i * 100), SCL(4), SCL(4), GRAY_3_COLOR_100); //selection column 1 accent
	}
	
	DrawRectangle(offsetX + SCL(442), offsetY + SCL(336), SCL(360), SCL(2), GRAY_1_COLOR_100); //selection column 2 upper border
	DrawTriangle(
        Vector2{(float)offsetX + SCL(742), (float)offsetY + SCL(336)},
        Vector2{(float)offsetX + SCL(746), (float)offsetY + SCL(336)},
        Vector2{(float)offsetX + SCL(746), (float)offsetY + SCL(332)},
        GRAY_1_COLOR_100
    );
	DrawRectangle(offsetX + SCL(746), offsetY + SCL(332), SCL(56), SCL(4), GRAY_1_COLOR_100);
	
	DrawRectangle(offsetX + SCL(842), offsetY + SCL(336), SCL(1038), SCL(2), GRAY_1_COLOR_100); //details pane upper border
	DrawTriangle(
        Vector2{(float)offsetX + SCL(1820), (float)offsetY + SCL(336)},
        Vector2{(float)offsetX + SCL(1824), (float)offsetY + SCL(336)},
        Vector2{(float)offsetX + SCL(1824), (float)offsetY + SCL(332)},
        GRAY_1_COLOR_100
    );
	DrawRectangle(offsetX + SCL(1824), offsetY + SCL(332), SCL(56), SCL(4), GRAY_1_COLOR_100);
	
	DrawRectangle(offsetX + SCL(842), offsetY + SCL(910), SCL(1038), SCL(2), GRAY_1_COLOR_100); //details pane lower border
	
	DrawRectangle(offsetX + SCL(10), offsetY + SCL(1068), SCL(100), SCL(6), GRAY_0_COLOR_100); //bottom left accent
	DrawRectangle(offsetX + SCL(140), offsetY + SCL(1068), SCL(100), SCL(6), GRAY_4_COLOR_100);
	DrawRectangle(offsetX + SCL(270), offsetY + SCL(1068), SCL(100), SCL(6), GRAY_0_COLOR_100);
	DrawRectangle(offsetX + SCL(400), offsetY + SCL(1068), SCL(100), SCL(6), GRAY_0_COLOR_100);
    
    Vector2 mousePos = input.getMousePosition();
    bool isClicked = input.isMousePressed(MOUSE_LEFT_BUTTON);

    int playButtonY = centerY - SCL(20);
    int exitButtonY = centerY + SCL(40);

    if (UIComponents::drawPlayButton(centerX, playButtonY, mousePos, isClicked, PRIMARY_COLOR_100, GRAY_4_COLOR_80, BACKGROUND_COLOR, aspectScale)) {
        return MenuAction::START_3D;
    }
    
    if (UIComponents::drawExitButton(centerX, exitButtonY, mousePos, isClicked, GRAY_2_COLOR_100, GRAY_3_COLOR_80, GRAY_4_COLOR_80, aspectScale)) {
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
