#ifndef MENU_H
#define MENU_H

#include "uicomponents.h"

#include <vector>

class Menu {
public:
	Menu() {}
	Menu(bool visible) {
		this->visible = visible;
	}
	void addButton(int buttonX, int buttonY, int width, int height, const std::string& buttonText, bool buttonSelected, const Font& font) {
		buttons.emplace_back(buttonX, buttonY, width, height, buttonText, buttonSelected, &font);
	}
	
	void draw(int offsetX, int offsetY, float aspectScale, Vector2 mousePos, bool mouseClicked) {
		for (int i{}; i < buttons.size(); ++i) {
			buttons[i].draw(offsetX, offsetY, aspectScale, mousePos, mouseClicked);
		}
	}

	std::vector<Button> buttons;
	bool visible = true;
	int selectedIndex;
};

#endif