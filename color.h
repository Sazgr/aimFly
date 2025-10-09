#ifndef COLOR_H
#define COLOR_H

#include "raylib.h"
#include <cstdlib>

Color hexToColor(const char* hex, float opacity);

const Color BACKGROUND_COLOR = hexToColor("#2A2D32", 1.0f);
const Color PRIMARY_0_COLOR_100 = hexToColor("#09B58A", 1.0f);
const Color PRIMARY_0_COLOR_10 = hexToColor("#09B58A", 0.1f);

const Color PRIMARY_1_COLOR_100 = hexToColor("#23AF8C", 1.0f);

const Color PRIMARY_2_COLOR_100 = hexToColor("#52736B", 1.0f);

const Color GRAY_0_COLOR_40 = hexToColor("#313439", 0.8f);

const Color GRAY_1_COLOR_100 = hexToColor("#5A6164", 1.0f);

const Color GRAY_2_COLOR_100 = hexToColor("#82858A", 1.0f);

const Color GRAY_3_COLOR_100 = hexToColor("#9A9DA2", 1.0f);

const Color GRAY_4_COLOR_100 = hexToColor("#A7AFB2", 1.0f);

const Color GRAY_5_COLOR_100 = hexToColor("#B9C9C7", 1.0f);
const Color GRAY_5_COLOR_80 = hexToColor("#B9C9C7", 0.8f);

const Color GRAY_6_COLOR_100 = hexToColor("#C4CDD0", 1.0f);

const Color GRAY_7_COLOR_100 = hexToColor("#E4F4F4", 1.0f);
const Color GRAY_7_COLOR_80 = hexToColor("#E4F4F4", 0.8f);
const Color GRAY_7_COLOR_40 = hexToColor("#E4F4F4", 0.4f);

const Color WHITE_COLOR_100 = hexToColor("#FFFFFF", 1.0f);
const Color WHITE_COLOR_80 = hexToColor("#FFFFFF", 0.8f);

#endif