#pragma once

#include "inputmanager.h"
#include "menu.h"
#include "taskdata.h"
#include "uicomponents.h"

#include "raylib.h"

#include <string>
#include <utility>
#include <vector>

enum class MenuAction {
	NONE,
    PRACTICE,
    CALIBRATE,
    SETTINGS,
    CREDITS, 
    EXIT,
	PEEKING,
	TRACKING,
	FLICKING,
	HEADSHOT,
	BEGIN,
	RESUME,
	RESTART,
	EXIT_TASK,
};

// map button text to action
const std::unordered_map<std::string, MenuAction> buttonActions = {
	{"PRACTICE", MenuAction::PRACTICE},
	{"CALIBRATE", MenuAction::CALIBRATE},
	{"SETTINGS", MenuAction::SETTINGS},
	{"CREDITS", MenuAction::CREDITS},
	{"EXIT", MenuAction::EXIT},
	{"PEEKING", MenuAction::PEEKING},
	{"TRACKING", MenuAction::TRACKING},
	{"FLICKING", MenuAction::FLICKING},
	{"HEADSHOT", MenuAction::HEADSHOT},
	{"BEGIN", MenuAction::BEGIN},
	{"RESUME", MenuAction::RESUME},
	{"RESTART", MenuAction::RESTART},
	{"EXIT TASK", MenuAction::EXIT_TASK},
};

class MenuScreen {
private:
    bool isPointInRect(Vector2 point, Rectangle rect);
    int selectedButtonIndex = 0; // to track selected button

public:
	std::vector<Menu> menus;
	MenuScreen();
	std::pair<std::string, std::string> getDateTime();
	void drawStat(float aspectScale, int offsetX, int offsetY, int x, std::string statTitle, std::string statValue);
    MenuAction renderMenu(int screenWidth, int screenHeight, int offsetX, int offsetY, 
                      int scaledWidth, int scaledHeight, InputManager& input, 
                      int nativeWidth, int nativeHeight, Shader& gradientShader, TaskData& taskData);
	MenuAction renderPauseOverlay(int screenWidth, int screenHeight, int offsetX, int offsetY, 
                      int scaledWidth, int scaledHeight, InputManager& input, 
                      int nativeWidth, int nativeHeight, Shader& gradientShader);
};
