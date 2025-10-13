#pragma once

#include "color.h"

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

struct ColorStyle {
	ColorStyle() {};
	ColorStyle(bool solid, Color color1, Color color2, bool showAccent = false) {
		this->solid = solid;
		this->color1 = color1;
		this->color2 = color2;
		this->showAccent = showAccent;
	}
	bool solid{true};
	Color color1{WHITE};
	Color color2{BLACK};
	bool showAccent{false};
};

const ColorStyle BUTTON_STYLE_0 = ColorStyle(false, GRAY_1_COLOR_100, GRAY_5_COLOR_100);
const ColorStyle SELECTED_STYLE_0 = ColorStyle(true, GRAY_5_COLOR_100, BACKGROUND_COLOR_100, true);

const ColorStyle BUTTON_STYLE_1 = ColorStyle(true, GRAY_5_COLOR_100, BACKGROUND_COLOR_100);
const ColorStyle BUTTON_STYLE_2 = ColorStyle(true, GRAY_6_COLOR_100, PRIMARY_3_COLOR_100);
const ColorStyle BUTTON_STYLE_3 = ColorStyle(false, PRIMARY_4_COLOR_100, GRAY_6_COLOR_100);

class Button {
public:
	Button() {}
	Button(int x, int y, int width, int height, const std::string& text, bool isSelected, ColorStyle normalStyle, ColorStyle selectedStyle, const Font* font = nullptr) {
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
		this->text = text;
		this->isSelected = isSelected;
		this->fontPtr = font;
		this->normalStyle = normalStyle;
		this->selectedStyle = selectedStyle;
	};
	void drawOutline(int offsetX, int offsetY, float aspectScale, Color shapeColor, Color arrowColor);
	void drawSolid(int offsetX, int offsetY, float aspectScale, Color shapeColor, Color arrowColor);
	void drawBase(int offsetX, int offsetY, float aspectScale, ColorStyle colorStyle);
    void draw(int offsetX, int offsetY, float aspectScale, Vector2 mousePos, bool mouseClicked);
	bool clicked(int offsetX, int offsetY, float aspectScale, Vector2 mousePos, bool mouseClicked);

	std::string text;
	int x;
	int y;
	int width;
	int height;
	float aspect;
	bool isSelected = false;
	const Font* fontPtr;
	ColorStyle normalStyle;
	ColorStyle selectedStyle;
};