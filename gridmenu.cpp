#include "gridmenu.h"

void GridMenu::generateGrid(const std::vector<std::string>& items, int containerX, int containerY, int containerWidth, int containerHeight, int offsetX, int offsetY, float aspectScale) {
    gridBoxes.clear();
    
    if (items.empty()) return;
    
    int numItems = items.size();
    
    int cols = 2;
    int rows = (numItems + cols - 1) / cols; 
    
    if (numItems <= 2) {
        cols = numItems;
        rows = 1;
    }

    int availableWidth = containerWidth - (marginX * 2) - (padding * (cols - 1));
    int availableHeight = containerHeight - (marginY * 2) - (padding * (rows - 1));
    int boxWidth = availableWidth / cols;
    int boxHeight = availableHeight / rows;
    
    for (int i = 0; i < numItems; i++) {
        int col = i % cols;
        int row = i / cols;
        
        int x = containerX + marginX + (col * (boxWidth + padding));
        int y = containerY + marginY + (row * (boxHeight + padding));
        
        GridBox box;
        box.rect = { 
            static_cast<float>(offsetX + x), 
            static_cast<float>(offsetY + y), 
            static_cast<float>(boxWidth), 
            static_cast<float>(boxHeight) 
        };
        box.name = items[i];
        box.index = i;
        
        gridBoxes.push_back(box);
    }
}

void GridMenu::renderGridBoxes(InputManager& input) {
    Vector2 mousePos = input.getMousePosition();
    bool mouseClicked = input.isMousePressed(MOUSE_LEFT_BUTTON);
    hoveredBoxIndex = -1;
    
    for (size_t i = 0; i < gridBoxes.size(); i++) {
        const GridBox& box = gridBoxes[i];
        
        bool isHovered = isPointInRect(mousePos, box.rect);
        bool isSelected = (selectedBoxIndex == static_cast<int>(i));
        
        if (isHovered) {
            hoveredBoxIndex = i;
            
            if (mouseClicked) {
                selectedBoxIndex = i;
            }
        }
        
        Color boxColor = normalColor;
        if (isSelected) {
            boxColor = selectedColor;
        } else if (isHovered) {
            boxColor = hoverColor;
        }
        
        DrawRectangleRec(box.rect, boxColor);
        
        const char* text = box.name.c_str();
        int fontSize = 20;
        int textWidth = MeasureText(text, fontSize);
        int textX = box.rect.x + (box.rect.width - textWidth) / 2;
        int textY = box.rect.y + (box.rect.height - fontSize) / 2;
        DrawText(text, textX, textY, fontSize, WHITE);
    }
}

std::string GridMenu::getSelectedGridItem() {
    if (selectedBoxIndex >= 0 && selectedBoxIndex < static_cast<int>(gridBoxes.size())) {
        return gridBoxes[selectedBoxIndex].name;
    }
    return "";
}