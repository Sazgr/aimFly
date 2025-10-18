#pragma once

#include "inputmanager.h"
#include "menu.h"
#include "task.h"
#include "taskdata.h"
#include "uicomponents.h"

#include "raylib.h"

#include <string>
#include <utility>
#include <vector>

#define MENU_ACTIONS(X) \
    X(NONE,        "NONE") \
    X(PRACTICE,    "PRACTICE") \
    X(CALIBRATE,   "CALIBRATE") \
    X(SETTINGS,    "SETTINGS") \
    X(CREDITS,     "CREDITS") \
    X(EXIT,        "EXIT") \
    X(PEEKING,     "PEEKING") \
    X(TRACKING,    "TRACKING") \
    X(FLICKING,    "FLICKING") \
	X(MICRO,       "MICRO") \
    X(HEADSHOT,    "HEADSHOT") \
    X(BEGIN,       "BEGIN") \
    X(RESUME,      "RESUME") \
    X(RESTART,     "RESTART") \
    X(EXIT_TASK,   "EXIT TASK")

enum class MenuAction {
#define X(name, text) name,
    MENU_ACTIONS(X)
#undef X
};

const std::unordered_map<std::string, MenuAction> buttonActions = {
#define X(name, text) {text, MenuAction::name},
    MENU_ACTIONS(X)
#undef X
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
	void renderCountdown(int screenWidth, int screenHeight, int offsetX, int offsetY, 
                      int scaledWidth, int scaledHeight, InputManager& input, 
                      int nativeWidth, int nativeHeight, Shader& gradientShader, Task& task);
};
