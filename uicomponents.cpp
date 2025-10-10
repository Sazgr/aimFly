#include "color.h"
#include "uicomponents.h"

#include "raylib.h"
#include "raymath.h" 

static bool isPointInRect(Vector2 point, Rectangle rect) {
    return (point.x >= rect.x && point.x <= rect.x + rect.width && point.y >= rect.y && point.y <= rect.y + rect.height);
}

void Gradient::drawGradientRect(Shader shader, Rectangle rect, Color c1, Color c2, bool vertical) {
    BeginShaderMode(shader);

    Vector4 start = { c1.r/255.0f, c1.g/255.0f, c1.b/255.0f, c1.a/255.0f };
    Vector4 end   = { c2.r/255.0f, c2.g/255.0f, c2.b/255.0f, c2.a/255.0f };
    Vector4 bounds = { rect.x, rect.y, rect.width, rect.height };

    SetShaderValue(shader, GetShaderLocation(shader, "startColor"), &start, SHADER_UNIFORM_VEC4);
    SetShaderValue(shader, GetShaderLocation(shader, "endColor"), &end, SHADER_UNIFORM_VEC4);
    SetShaderValue(shader, GetShaderLocation(shader, "rectBounds"), &bounds, SHADER_UNIFORM_VEC4);

    int vertFlag = vertical ? 1 : 0;
    SetShaderValue(shader, GetShaderLocation(shader, "vertical"), &vertFlag, SHADER_UNIFORM_INT);

    DrawRectangleRec(rect, WHITE);

    EndShaderMode();
}

void Button::draw(int offsetX, int offsetY, float aspectScale, Vector2 mousePos, bool mouseClicked) {
	#define SCL(val) ((float)(val) * aspectScale)

	Rectangle buttonRect = { // button rectangle for detection
        offsetX + SCL(x),
        offsetY + SCL(y),
        SCL(width),
        SCL(height)
    };

	bool isHovered = isPointInRect(mousePos, buttonRect); // for hover

	Color normalColor = GRAY_5_COLOR_100; // colors for states
    Color hoverColor = GRAY_5_COLOR_100; // temporary; we can change this
    Color outlineColor = isSelected || isHovered ? normalColor : GRAY_1_COLOR_100;
    Color arrowColor = isSelected || isHovered ? BACKGROUND_COLOR : GRAY_5_COLOR_100;
	
	Vector2 arrowPoints[6] = {
		Vector2{offsetX + SCL(x + width - 36), offsetY + SCL(y + 18)},
		Vector2{offsetX + SCL(x + width - 36), offsetY + SCL(y + 18 + (45.0f / 8.0f))},
		Vector2{offsetX + SCL(x + width - 20), offsetY + SCL(y + 33)},
		Vector2{offsetX + SCL(x + width - 26), offsetY + SCL(y + 33)},
		Vector2{offsetX + SCL(x + width - 36), offsetY + SCL(y + 48)},
		Vector2{offsetX + SCL(x + width - 36), offsetY + SCL(y + 48 - (45.0f / 8.0f))},
	};
	
	if (isSelected || isHovered) {
        Vector2 outlinePoints[5] = {
            Vector2{offsetX + SCL(x), offsetY + SCL(y)},
            Vector2{offsetX + SCL(x), offsetY + SCL(y + height)},
            Vector2{offsetX + SCL(x + width - 24), offsetY + SCL(y + height)},
            Vector2{offsetX + SCL(x + width), offsetY + SCL(y + height - 20)},
            Vector2{offsetX + SCL(x + width), offsetY + SCL(y)},
        };
        DrawTriangleFan(outlinePoints, 5, isHovered ? hoverColor : normalColor);
        DrawTriangleStrip(arrowPoints, 6, arrowColor);
    } else {
        Vector2 outlinePoints[6] = {
            Vector2{offsetX + SCL(x + 1), offsetY + SCL(y + 1)},
            Vector2{offsetX + SCL(x + 1), offsetY + SCL(y + height - 1)},
            Vector2{offsetX + SCL(x + width - 25), offsetY + SCL(y + height - 1)},
            Vector2{offsetX + SCL(x + width - 1), offsetY + SCL(y + height - 21)},
            Vector2{offsetX + SCL(x + width - 1), offsetY + SCL(y + 1)},
            Vector2{offsetX + SCL(x + 1) + 1, offsetY + SCL(y + 1)},
        };
        DrawSplineLinear(outlinePoints, 6, 2 * aspectScale, outlineColor);
        DrawTriangleStrip(arrowPoints, 6, arrowColor);
    }
	
	if (isSelected && showAccent) {
		DrawRectangle(offsetX + SCL(x + 378), offsetY + SCL(y), SCL(4), SCL(45), normalColor); // vertical accent
	}

	if (fontPtr && fontPtr->texture.id != 0) { // checking if font is valid
		float fontSize = SCL(30); 
		Vector2 textSize = MeasureTextEx(*fontPtr, text.c_str(), fontSize, 1);
		float textX = offsetX + SCL(x + width - 50) - textSize.x; // some px offset, trying 60px from right for now
		float textY = offsetY + SCL(y) + (SCL(height) - textSize.y) / 2; // vertically centered
		Color textColor = isSelected || isHovered ? BACKGROUND_COLOR : GRAY_3_COLOR_100;
		DrawTextEx(*fontPtr, text.c_str(), Vector2{textX, textY}, fontSize, 1, textColor);
	}
	
	#undef SCL
}

bool Button::clicked(int offsetX, int offsetY, float aspectScale, Vector2 mousePos, bool mouseClicked) {
	#define SCL(val) ((float)(val) * aspectScale)

	Rectangle buttonRect = { // button rectangle for detection
        offsetX + SCL(x),
        offsetY + SCL(y),
        SCL(width),
        SCL(height)
    };

	bool isHovered = isPointInRect(mousePos, buttonRect); // for hover
	
	#undef SCL
	return isHovered && mouseClicked; // conditions for click
}
