#ifndef HITSCAN_H
#define HITSCAN_H

#include "spheretarget.h"

#include "raylib.h"
#include "raymath.h"

bool hitscan(Vector3 camPos, Vector3 forward, SphereTarget& target);

#endif