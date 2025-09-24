#ifndef HITSCAN_H
#define HITSCAN_H

#include "raylib.h"
#include "raymath.h"

#include "spheretarget.h"

bool hitscan(Vector3 camPos, Vector3 forward, SphereTarget& target);

#endif