#pragma once

#include "inputmanager.h"
#include "menu.h"
#include "uicomponents.h"

#include "raylib.h"

#include <string>
#include <vector>

enum class MenuAction {
    PRACTICE,
    CALIBRATE,
    SETTINGS,
    CREDITS, 
    EXIT
};

class MenuScreen {
private:
    bool isPointInRect(Vector2 point, Rectangle rect);
	std::vector<Menu> menus;
    int selectedButtonIndex = 0; // to track selected button

public:
	MenuScreen();
	void drawStat(float aspectScale, int offsetX, int offsetY, int x, std::string statTitle, std::string statValue);
    MenuAction render(int screenWidth, int screenHeight, int offsetX, int offsetY, 
                      int scaledWidth, int scaledHeight, InputManager& input, 
                      int nativeWidth, int nativeHeight, Shader& gradientShader);
};
