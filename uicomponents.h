#pragma once

#include "raylib.h"
#include <string>

class Gradient {
private:
    static Shader gradientShader;
    static int color1Loc;
    static int color2Loc;

public:    
    static void drawGradientRect(Shader shader, Rectangle rect, Color c1, Color c2, bool vertical);
};

class Button {
public:
	Button() {}
	Button(int x, int y, int width, int height, const std::string& text, bool isSelected, const Font* font = nullptr, bool showAccent = false) {
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
		this->text = text;
		this->isSelected = isSelected;
		this->fontPtr = font;
	}
    bool draw(int offsetX, int offsetY, float aspectScale, Vector2 mousePos, bool mouseClicked);

	std::string text;
	int x;
	int y;
	int width;
	int height;
	float aspect;
	bool isSelected = false;
	const Font* fontPtr;
	bool showAccent;
};