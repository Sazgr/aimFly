#ifndef GRIDMENU_H
#define GRIDMENU_H

#include "color.h"
#include "inputmanager.h"
#include "util.h"

#include "raylib.h"

#include <string>
#include <vector>

struct GridBox {
    Rectangle rect;
    std::string name;
    int index;
};

class GridMenu {
	std::vector<GridBox> gridBoxes;
    int hoveredBoxIndex = -1;
    int selectedBoxIndex = -1;

	int padding = 16;      
	int marginX = 8;      
	int marginY = 8;     
	Color normalColor = DARKGRAY;
	Color hoverColor = GRAY;
	Color selectedColor = LIGHTGRAY;
	
public:
	void generateGrid(const std::vector<std::string>& items, 
                 int containerX, int containerY, 
                 int containerWidth, int containerHeight,
                 int offsetX, int offsetY, float aspectScale);
    
	void setupExampleGrid(int offsetX, int offsetY);
    void renderGridBoxes(InputManager& input);
    std::string getSelectedGridItem();
};

#endif