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

bool Button::draw(int offsetX, int offsetY, float aspectScale, Vector2 mousePos, bool mouseClicked) {
	#define SCL(val) ((float)(val) * aspectScale)
	
	Vector2 arrowPoints[6] = {
		Vector2{offsetX + SCL(x + width - 36), offsetY + SCL(y + 18)},
		Vector2{offsetX + SCL(x + width - 36), offsetY + SCL(y + 18 + (45.0f / 8.0f))},
		Vector2{offsetX + SCL(x + width - 20), offsetY + SCL(y + 33)},
		Vector2{offsetX + SCL(x + width - 26), offsetY + SCL(y + 33)},
		Vector2{offsetX + SCL(x + width - 36), offsetY + SCL(y + 48)},
		Vector2{offsetX + SCL(x + width - 36), offsetY + SCL(y + 48 - (45.0f / 8.0f))},
	};
	
	if (isSelected) {
		//DrawRectangle(offsetX + SCL(x), offsetY + SCL(y), SCL(width), SCL(height), GRAY_3_COLOR_100);
		Vector2 outlinePoints[5] = {
			Vector2{offsetX + SCL(x), offsetY + SCL(y)},
			Vector2{offsetX + SCL(x), offsetY + SCL(y + height)},
			Vector2{offsetX + SCL(x + width - 24), offsetY + SCL(y + height)},
			Vector2{offsetX + SCL(x + width), offsetY + SCL(y + height - 20)},
			Vector2{offsetX + SCL(x + width), offsetY + SCL(y)},
		};
		DrawTriangleFan(outlinePoints, 5, GRAY_3_COLOR_100); //24x20 removed corner
		DrawTriangleStrip(arrowPoints, 6, BACKGROUND_COLOR);
	
		DrawRectangle(offsetX + SCL(x + 378), offsetY + SCL(y), SCL(4), SCL(45), GRAY_3_COLOR_100); //vertical accent
		
	} else {
		Vector2 outlinePoints[6] = {
			Vector2{offsetX + SCL(x + 1), offsetY + SCL(y + 1)},
			Vector2{offsetX + SCL(x + 1), offsetY + SCL(y + height - 1)},
			Vector2{offsetX + SCL(x + width - 25), offsetY + SCL(y + height - 1)},
			Vector2{offsetX + SCL(x + width - 1), offsetY + SCL(y + height - 21)},
			Vector2{offsetX + SCL(x + width - 1), offsetY + SCL(y + 1)},
			Vector2{offsetX + SCL(x + 1) + 1, offsetY + SCL(y + 1)},
		};
		DrawSplineLinear(outlinePoints, 6, 2 * aspectScale, GRAY_1_COLOR_100);
		DrawTriangleStrip(arrowPoints, 6, GRAY_3_COLOR_100);
	}
	
	#undef SCL
	return false;
}
/*bool UIComponents::drawButtonCore(const char* text, int centerX, int centerY, int width, int height, Vector2 mousePos, bool isClicked, Color normalColor, Color hoverColor, Color textColor, float aspectScale) {
    int scaledWidth = (int)(width * aspectScale);
    int scaledHeight = (int)(height * aspectScale);
    int scaledFontSize = (int)(20 * aspectScale);

    Rectangle rect = { (float)centerX - scaledWidth/2, (float)centerY - scaledHeight/2, (float)scaledWidth, (float)scaledHeight };
    bool isHovered = isPointInRect(mousePos, rect);
    
    DrawRectangleRec(rect, isHovered ? hoverColor : normalColor);

    int textWidth = MeasureText(text, scaledFontSize);
    DrawText(text, centerX - textWidth / 2, centerY - scaledFontSize / 2, scaledFontSize, textColor);

    return isHovered && isClicked;
}*/

/*bool UIComponents::drawPlayButton(int centerX, int centerY, Vector2 mousePos, bool isClicked, Color normalColor, Color hoverColor, Color textColor, float aspectScale) {
    //const int width = 200;
    //const int height = 50;
    //return drawButtonCore("START PRACTICE", centerX, centerY, width, height, mousePos, isClicked, normalColor, hoverColor, textColor, aspectScale);
}

bool UIComponents::drawExitButton(int centerX, int centerY, Vector2 mousePos, bool isClicked, Color normalColor, Color hoverColor, Color textColor, float aspectScale) {
    //const int width = 200;
    //const int height = 50;
    //return drawButtonCore("EXIT", centerX, centerY, width, height, mousePos, isClicked, normalColor, hoverColor, textColor, aspectScale);
}

bool UIComponents::drawButton(const char* text, int centerX, int centerY, int width, int height, Vector2 mousePos, bool isClicked, Color normalColor, Color hoverColor, Color textColor, float aspectScale) {
    //return drawButtonCore(text, centerX, centerY, width, height, mousePos, isClicked, normalColor, hoverColor, textColor, aspectScale);
}*/
