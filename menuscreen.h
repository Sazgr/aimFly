#pragma once

#include "inputmanager.h"
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
	std::vector<Button> buttons;
    int selectedButtonIndex = 0; // to track selected button

public:
	MenuScreen();
	void addButton(int buttonX, int buttonY, int width, int height, const std::string& buttonText, bool isSelected, const Font& font);
    MenuAction render(int screenWidth, int screenHeight, int offsetX, int offsetY, 
                      int scaledWidth, int scaledHeight, InputManager& input, 
                      int nativeWidth, int nativeHeight, Shader& gradientShader);
};
