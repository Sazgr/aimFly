#pragma once

#include "inputmanager.h"
#include "menu.h"
#include "uicomponents.h"

#include "raylib.h"

#include <string>
#include <vector>

enum class MenuAction {
	NONE,
    PRACTICE,
    CALIBRATE,
    SETTINGS,
    CREDITS, 
    EXIT,
	TRACKING,
	FLICKING,
	HEADSHOT,
	BEGIN,
};

// map button text to action
const std::unordered_map<std::string, MenuAction> buttonActions = {
	{"PRACTICE", MenuAction::PRACTICE},
	{"CALIBRATE", MenuAction::CALIBRATE},
	{"SETTINGS", MenuAction::SETTINGS},
	{"CREDITS", MenuAction::CREDITS},
	{"EXIT", MenuAction::EXIT},
	{"TRACKING", MenuAction::TRACKING},
	{"FLICKING", MenuAction::FLICKING},
	{"HEADSHOT", MenuAction::HEADSHOT},
	{"BEGIN", MenuAction::BEGIN},
};

class MenuScreen {
private:
    bool isPointInRect(Vector2 point, Rectangle rect);
    int selectedButtonIndex = 0; // to track selected button

public:
	std::vector<Menu> menus;
	MenuScreen();
	void drawStat(float aspectScale, int offsetX, int offsetY, int x, std::string statTitle, std::string statValue);
    MenuAction render(int screenWidth, int screenHeight, int offsetX, int offsetY, 
                      int scaledWidth, int scaledHeight, InputManager& input, 
                      int nativeWidth, int nativeHeight, Shader& gradientShader);
};
