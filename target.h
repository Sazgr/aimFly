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

class Target {
public:
	Target(TargetType t, Vector3 pos, float sz) {
		type = t;
		position = pos;
		size = sz;
		if (type == TargetType::SPHERE) {
			Mesh mesh = GenMeshSphere(size, 32, 32);
			models.emplace_back(LoadModelFromMesh(mesh));
		} else if (t == TargetType::BODY) {
			Mesh mesh = GenMeshSphere(size, 32, 32);
			models.emplace_back(LoadModelFromMesh(mesh));
			mesh = GenMeshSphere(1.5 * size, 32, 32);
			models.emplace_back(LoadModelFromMesh(mesh));
			mesh = GenMeshCylinder(1.5 * size, 2.5 * size, 32);
			models.emplace_back(LoadModelFromMesh(mesh));
			health = 4;
		}
		//UnloadMesh(mesh); 
	}
	
	~Target() {
		for (int i{}; i < models.size(); ++i) {
			UnloadModel(models[i]);
		}
    }
	
	void addShader(Shader& shader) {
		for (int i{}; i < models.size(); ++i) {
			this->models[i].materials[0].shader = shader;
		}
	}
	
	void draw() {
		if (type == TargetType::SPHERE) {
			DrawModel(models[0], position, 1.0f, PINK);
		} else if (type == TargetType::BODY) {
			DrawModel(models[0], position, 1.0f, PINK);
			DrawModel(models[1], Vector3Add(position, Vector3{0.0f, - 2 * size, 0.0f}), 1.0f, PINK);
			DrawModel(models[2], Vector3Add(position, Vector3{0.0f, - 4.5f * size, 0.0f}), 1.0f, PINK);
			DrawModel(models[1], Vector3Add(position, Vector3{0.0f, - 4.5f * size, 0.0f}), 1.0f, PINK);
		}
	}
	
	int hitscan(Vector3 camPos, Vector3 forward) {
		if (type == TargetType::SPHERE) {
			Vector3 delta = Vector3Subtract(position, camPos);
			Vector3 projection = Vector3Scale(forward, Vector3DotProduct(delta, forward) / Vector3LengthSqr(forward));
			float distance = Vector3Distance(delta, projection);
			return (distance <= size);
		} else if (type == TargetType::BODY) {
			Vector3 delta = Vector3Subtract(position, camPos);
			Vector3 projection = Vector3Scale(forward, Vector3DotProduct(delta, forward) / Vector3LengthSqr(forward));
			float distance = Vector3Distance(delta, projection);
			if (distance <= size) return HEADSHOT;
			
			delta = Vector3Subtract(Vector3Add(position, Vector3{0.0f, - 2 * size, 0.0f}), camPos);
			projection = Vector3Scale(forward, Vector3DotProduct(delta, forward) / Vector3LengthSqr(forward));
			distance = Vector3Distance(delta, projection);
			if (distance <= 2 * size) return BODYSHOT;
			
			delta = Vector3Subtract(Vector3Add(position, Vector3{0.0f, - 2.8f * size, 0.0f}), camPos);
			projection = Vector3Scale(forward, Vector3DotProduct(delta, forward) / Vector3LengthSqr(forward));
			distance = Vector3Distance(delta, projection);
			if (distance <= 2 * size) return BODYSHOT;
			
			delta = Vector3Subtract(Vector3Add(position, Vector3{0.0f, - 3.7f * size, 0.0f}), camPos);
			projection = Vector3Scale(forward, Vector3DotProduct(delta, forward) / Vector3LengthSqr(forward));
			distance = Vector3Distance(delta, projection);
			if (distance <= 2 * size) return BODYSHOT;
			
			delta = Vector3Subtract(Vector3Add(position, Vector3{0.0f, - 4.5f * size, 0.0f}), camPos);
			projection = Vector3Scale(forward, Vector3DotProduct(delta, forward) / Vector3LengthSqr(forward));
			distance = Vector3Distance(delta, projection);
			if (distance <= 2 * size) return BODYSHOT;
			
			return MISS;
		} else {
			return 0;
		}
	}	
	
	Vector3 position;
	Vector3 velocity;
	float size;
	std::vector<Model> models;
	TargetType type;
	int health;
};

#endif