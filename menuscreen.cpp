#include "color.h"
#include "menuscreen.h"
#include "uicomponents.h"
#include "inputmanager.h"
#include "rlgl.h"
#include "raylib.h"
#include <algorithm>

extern Font DOHYEON_REGULAR;
extern Font ORBITRON_BOLD;

MenuScreen::MenuScreen() {
    // main buttons
	addButton(40, 342, 360, 65, "PRACTICE", true, DOHYEON_REGULAR);
	addButton(40, 442, 360, 65, "CALIBRATE", false, DOHYEON_REGULAR);
	addButton(40, 542, 360, 65, "SETTINGS", false, DOHYEON_REGULAR);
	addButton(40, 642, 360, 65, "CREDITS", false, DOHYEON_REGULAR);
	addButton(40, 742, 360, 65, "EXIT", false, DOHYEON_REGULAR);
	
    // practice set buttons
	addButton(442, 342, 360, 65, "BOTS", false, DOHYEON_REGULAR);
	addButton(442, 442, 360, 65, "TRACKING", false, DOHYEON_REGULAR);
	addButton(442, 542, 360, 65, "FLICKING", true, DOHYEON_REGULAR);
	addButton(442, 642, 360, 65, "MICRO", false, DOHYEON_REGULAR);
	addButton(442, 742, 360, 65, "REACTION", false, DOHYEON_REGULAR);
	addButton(442, 842, 360, 65, "HEADSHOT", false, DOHYEON_REGULAR);
	
	addButton(1640, 944, 240, 65, "BEGIN", false, DOHYEON_REGULAR);
}

void MenuScreen::addButton(int buttonX, int buttonY, int width, int height, const std::string& buttonText, bool buttonSelected, const Font& font) {
    buttons.emplace_back(buttonX, buttonY, width, height, buttonText, buttonSelected, &font);
}

void MenuScreen::drawStat(float aspectScale, int offsetX, int offsetY, int x, std::string statTitle, std::string statValue) {
	#define SCL(val) ((float)(val) * aspectScale)
	
	Vector2 statTitleSize = MeasureTextEx(DOHYEON_REGULAR, statTitle.c_str(), SCL(20), 1);
	Vector2 statValueSize = MeasureTextEx(DOHYEON_REGULAR, statValue.c_str(), SCL(40), 1);
	DrawTextEx(DOHYEON_REGULAR, statValue.c_str(), Vector2{offsetX + SCL(x) - statValueSize.x, offsetY + SCL(124)}, SCL(40), 1, GRAY_6_COLOR_100);
	DrawTextEx(DOHYEON_REGULAR, statTitle.c_str(), Vector2{offsetX + SCL(x) - statTitleSize.x, offsetY + SCL(176)}, SCL(20), 1, GRAY_4_COLOR_100);
	
	#undef SCL
}

MenuAction MenuScreen::render(int screenWidth, int screenHeight, int offsetX, int offsetY, int scaledWidth, int scaledHeight, InputManager& input, int nativeWidth, int nativeHeight, Shader& gradientShader) {
    float aspectScale = (float)scaledWidth / (float)nativeWidth;
    
    #define SCL(val) ((float)(val) * aspectScale)

    int centerX = offsetX + scaledWidth / 2; // new top left for 16:9 (0,0)
    int centerY = offsetY + scaledHeight / 2;

    DrawRectangle(offsetX, offsetY, scaledWidth, scaledHeight, BACKGROUND_COLOR); // background
	
	//top right pattern
	for (int i{0}; i < 12; ++i) {
		Vector2 points[5] = {
            Vector2{offsetX + SCL(490 + i * 120), offsetY + SCL(50)},
            Vector2{offsetX + SCL(680 + i * 120), offsetY + SCL(240)},
            Vector2{offsetX + SCL(740 + i * 120), offsetY + SCL(240)},
            Vector2{offsetX + SCL(550 + i * 120), offsetY + SCL(50)},
        };
        DrawTriangleFan(points, 4, GRAY_0_COLOR_40);
	}
	DrawRectangle(offsetX + SCL(490), offsetY + SCL(20), SCL(200), SCL(240), BACKGROUND_COLOR);
	DrawRectangle(offsetX + SCL(1880), offsetY + SCL(20), SCL(40), SCL(240), BACKGROUND_COLOR);
	
	Gradient::drawGradientRect( // background PRIMARY_0 accent gradient
        gradientShader, 
        Rectangle{ (float)offsetX, (float)offsetY, (float)SCL(500), (float)scaledHeight },
        hexToColor("#273A3B", 1.0f),
        BACKGROUND_COLOR, 
        false
    );
	
    DrawRectangle(offsetX + SCL(40), offsetY + SCL(20), SCL(726), SCL(4), GRAY_7_COLOR_80); // top accent bar
    DrawTriangle(
        Vector2{(float)offsetX + SCL(740), (float)offsetY + SCL(24)},
        Vector2{(float)offsetX + SCL(766), (float)offsetY + SCL(50)},
        Vector2{(float)offsetX + SCL(766), (float)offsetY + SCL(24)},
        GRAY_7_COLOR_80
    );
    Gradient::drawGradientRect(
        gradientShader, 
        Rectangle{ (float)offsetX + SCL(766), (float)offsetY + SCL(20), (float)SCL(1114), (float)SCL(30) },
        GRAY_7_COLOR_80,
        GRAY_7_COLOR_40, 
        false
    );
	DrawRectangle(offsetX + SCL(1527), offsetY + SCL(30), SCL(100), SCL(10), GRAY_5_COLOR_80); //top accent bar details
	DrawRectangle(offsetX + SCL(1647), offsetY + SCL(30), SCL(100), SCL(10), PRIMARY_0_COLOR_100);
	DrawRectangle(offsetX + SCL(1767), offsetY + SCL(30), SCL(100), SCL(10), GRAY_5_COLOR_80);
	std::string dateString = "September 26, 2025";
	//DrawRectangle(offsetX + SCL(766), offsetY + SCL(18), SCL(400), SCL(34), GRAY_1_COLOR_100);
	DrawTextEx(DOHYEON_REGULAR, dateString.c_str(), Vector2{offsetX + SCL(766), offsetY + SCL(25)}, SCL(22), 1, PRIMARY_1_COLOR_100);
	Vector2 dateSize = MeasureTextEx(DOHYEON_REGULAR, dateString.c_str(), SCL(22), 1);
	DrawTextEx(DOHYEON_REGULAR, " | 10:09AM", Vector2{offsetX + SCL(766) + dateSize.x, offsetY + SCL(25)}, SCL(22), 1, PRIMARY_2_COLOR_100);

    DrawRectangle(offsetX + SCL(40), offsetY + SCL(40), SCL(10), SCL(200), GRAY_7_COLOR_100); // title gradient vertical start
    Gradient::drawGradientRect( // title gradient backing
        gradientShader, 
        Rectangle{ (float)offsetX + SCL(50), (float)offsetY + SCL(40), (float)SCL(600), (float)SCL(200) },
        hexToColor("#16846B", 1.0f),
        hexToColor("#16846B", 0.0f), 
        false
    );
	DrawTextEx(ORBITRON_BOLD, "aimfly", Vector2{offsetX + SCL(80), offsetY + SCL(57)}, SCL(125), 1, WHITE_COLOR_100);
	DrawTextEx(DOHYEON_REGULAR, "VALORANT aim trainer", Vector2{offsetX + SCL(80), offsetY + SCL(158)}, SCL(27), 1, WHITE_COLOR_80);
	
	DrawRectangle(offsetX + SCL(757), offsetY + SCL(109), SCL(200), SCL(2), GRAY_3_COLOR_100); // stat panel accent line
	DrawLineEx(Vector2{static_cast<float>(offsetX + SCL(956)), static_cast<float>(offsetY + SCL(110))}, Vector2{static_cast<float>(offsetX + SCL(1018)), static_cast<float>(offsetY + SCL(170))}, 2.4f * aspectScale, GRAY_3_COLOR_100);
	DrawRectangle(offsetX + SCL(1017), offsetY + SCL(169), SCL(800), SCL(2), GRAY_3_COLOR_100);
	DrawTextEx(DOHYEON_REGULAR, "PLAY TIME", Vector2{offsetX + SCL(757), offsetY + SCL(86)}, SCL(20), 1, GRAY_4_COLOR_100);
	DrawTextEx(DOHYEON_REGULAR, "100hr", Vector2{offsetX + SCL(757), offsetY + SCL(120)}, SCL(40), 1, GRAY_6_COLOR_100);
	
	drawStat(aspectScale, offsetX, offsetY, 1217, "SCORE", "670");
	drawStat(aspectScale, offsetX, offsetY, 1417, "ACCURACY", "88.32%");
	drawStat(aspectScale, offsetX, offsetY, 1617, "AVG TIME", "52ms");
	drawStat(aspectScale, offsetX, offsetY, 1817, "SPEED", "82mm/s");
	
	DrawRectangle(offsetX + SCL(761), offsetY + SCL(219), SCL(4), SCL(4), GRAY_5_COLOR_80); // stat panel accent
	DrawRectangle(offsetX + SCL(780), offsetY + SCL(219), SCL(213), SCL(4), GRAY_5_COLOR_80);
	
	for (int i{}; i < 5; ++i) {
		DrawRectangle(offsetX + SCL(20), offsetY + SCL(422 + i * 100), SCL(300), SCL(2), GRAY_2_COLOR_100); //selection column 1 separator bars
		DrawTriangle(
			Vector2{(float)offsetX + SCL(67), (float)offsetY + SCL(424 + i * 100)},
			Vector2{(float)offsetX + SCL(67), (float)offsetY + SCL(428 + i * 100)},
			Vector2{(float)offsetX + SCL(71), (float)offsetY + SCL(424 + i * 100)},
			GRAY_2_COLOR_100
		);
		DrawRectangle(offsetX + SCL(20), offsetY + SCL(424 + i * 100), SCL(47), SCL(4), GRAY_2_COLOR_100);
		
		DrawRectangle(offsetX + SCL(10), offsetY + SCL(434 + i * 100), SCL(4), SCL(4), GRAY_5_COLOR_100); //selection column 1 accent
	}
	
	DrawRectangle(offsetX + SCL(442), offsetY + SCL(336), SCL(360), SCL(2), GRAY_2_COLOR_100); //selection column 2 upper border
	DrawTriangle(
        Vector2{(float)offsetX + SCL(742), (float)offsetY + SCL(336)},
        Vector2{(float)offsetX + SCL(746), (float)offsetY + SCL(336)},
        Vector2{(float)offsetX + SCL(746), (float)offsetY + SCL(332)},
        GRAY_2_COLOR_100
    );
	DrawRectangle(offsetX + SCL(746), offsetY + SCL(332), SCL(56), SCL(4), GRAY_2_COLOR_100);
	
	DrawRectangle(offsetX + SCL(842), offsetY + SCL(336), SCL(1038), SCL(2), GRAY_2_COLOR_100); //details pane upper border
	DrawTriangle(
        Vector2{(float)offsetX + SCL(1820), (float)offsetY + SCL(336)},
        Vector2{(float)offsetX + SCL(1824), (float)offsetY + SCL(336)},
        Vector2{(float)offsetX + SCL(1824), (float)offsetY + SCL(332)},
        GRAY_2_COLOR_100
    );
	DrawRectangle(offsetX + SCL(1824), offsetY + SCL(332), SCL(56), SCL(4), GRAY_2_COLOR_100);
	
	DrawRectangle(offsetX + SCL(842), offsetY + SCL(910), SCL(1038), SCL(2), GRAY_2_COLOR_100); //details pane lower border
	
	DrawTextEx(DOHYEON_REGULAR, "v0.0.0", Vector2{offsetX + SCL(10), offsetY + SCL(1045)}, SCL(20), 1, GRAY_1_COLOR_100); //version text
	
	DrawRectangle(offsetX + SCL(10), offsetY + SCL(1068), SCL(100), SCL(6), GRAY_1_COLOR_100); //bottom left accent
	DrawRectangle(offsetX + SCL(140), offsetY + SCL(1068), SCL(100), SCL(6), GRAY_6_COLOR_100);
	DrawRectangle(offsetX + SCL(270), offsetY + SCL(1068), SCL(100), SCL(6), GRAY_1_COLOR_100);
	DrawRectangle(offsetX + SCL(400), offsetY + SCL(1068), SCL(100), SCL(6), GRAY_1_COLOR_100);
    
	Vector2 mousePos = input.getMousePosition();
	bool mouseClicked = input.isMousePressed(MOUSE_LEFT_BUTTON);

    // map button text to action
    std::unordered_map<std::string, MenuAction> buttonActions = {
        {"PRACTICE", MenuAction::PRACTICE},
        {"CALIBRATE", MenuAction::CALIBRATE},
        {"SETTINGS", MenuAction::SETTINGS},
        {"CREDITS", MenuAction::CREDITS},
        {"EXIT", MenuAction::EXIT}
    };

    for (int i = 0; i < buttons.size(); i++) { // look here kelele!!!!!!!!!!!!!!!!!!!!
        buttons[i].isSelected = (i == selectedButtonIndex);
        
        if (buttons[i].draw(offsetX, offsetY, aspectScale, mousePos, mouseClicked)) {
            selectedButtonIndex = i;  // update selection on click (but this treats sub buttons as main buttons)
            
            auto it = buttonActions.find(buttons[i].text);
            if (it != buttonActions.end()) {
                return it->second;
            }
        }
    }

    #undef SCL    
    return MenuAction::PRACTICE; // default on load
}

bool MenuScreen::isPointInRect(Vector2 point, Rectangle rect) {
    return (point.x >= rect.x && point.x <= rect.x + rect.width && point.y >= rect.y && point.y <= rect.y + rect.height);
}
