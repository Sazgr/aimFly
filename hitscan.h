#ifndef HITSCAN_H
#define HITSCAN_H

#include "target.h"

#include "raylib.h"
#include "raymath.h"

bool hitscan(Vector3 camPos, Vector3 forward, Target& target);

#endif