#include "color.h"

Color hexToColor(const char* hex, float opacity) {
    if (hex[0] == '#') hex++;
    
    unsigned int hexValue = strtoul(hex, nullptr, 16);
    
    Color color;
    color.r = (hexValue >> 16) & 0xFF;
    color.g = (hexValue >> 8) & 0xFF;
    color.b = hexValue & 0xFF;
    color.a = (unsigned char)(255 * opacity);
    
    return color;
}