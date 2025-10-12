#ifndef TARGET_H
#define TARGET_H

#include "raylib.h"
#include "raymath.h"

#include <vector>

enum class TargetType {
    SPHERE,
	BODY,
};

enum HitType {
    MISS,
	HEADSHOT,
	BODYSHOT,
};

struct HitInfo;

class Target {
public:
	Target(TargetType t, Vector3 pos, float sz);
	~Target();
	void addShader(Shader& shader);
	void draw();
	HitInfo hitscan(Vector3 camPos, Vector3 forward);	
	
	Vector3 position;
	Vector3 velocity;
	float size;
	int hitType;
	double lastHit{0.0};
	std::vector<Model> models;
	TargetType type;
	int health;
};

#endif