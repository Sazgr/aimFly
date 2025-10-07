#pragma once

#include "inputmanager.h"
#include "uicomponents.h"

#include "raylib.h"

#include <string>
#include <vector>

enum class MenuAction {
    NONE,
    START_3D, 
    EXIT_GAME
};

class MenuScreen {
private:
    bool isPointInRect(Vector2 point, Rectangle rect);
	std::vector<Button> buttons;

public:
	MenuScreen();
	void addButton(int buttonX, int buttonY, int width, int height, std::string buttonText, bool mouseClicked);
    MenuAction render(int screenWidth, int screenHeight, int offsetX, int offsetY, 
                      int scaledWidth, int scaledHeight, InputManager& input, 
                      int nativeWidth, int nativeHeight, Shader& gradientShader);
};
