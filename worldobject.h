#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H

#include "enums.h"

#include "raylib.h"
#include "raymath.h"

struct HitInfo;

class WorldObject {
public:
	WorldObject(Vector3 pos, float width, float height, float length, bool interactable);
	~WorldObject();
	void addShader(Shader& shader);
	void draw();
	HitInfo hitscan(Vector3 camPos, Vector3 forward);	
	
	Vector3 position;
	Model model;
	bool interactable;
};

#endif