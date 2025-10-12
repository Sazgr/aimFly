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
		hitType = MISS;
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
			DrawModel(models[0], position, 1.0f, SKYBLUE);
		} else if (type == TargetType::BODY) {
			DrawModel(models[0], position, 1.0f, hitType == HEADSHOT ? YELLOW : SKYBLUE);
			DrawModel(models[1], Vector3Add(position, Vector3{0.0f, - 2 * size, 0.0f}), 1.0f, hitType == BODYSHOT ? YELLOW : SKYBLUE);
			DrawModel(models[2], Vector3Add(position, Vector3{0.0f, - 4.5f * size, 0.0f}), 1.0f, hitType == BODYSHOT ? YELLOW : SKYBLUE);
			DrawModel(models[1], Vector3Add(position, Vector3{0.0f, - 4.5f * size, 0.0f}), 1.0f, hitType == BODYSHOT ? YELLOW : SKYBLUE);
		}
	}
	
	int hitscan(Vector3 camPos, Vector3 forward) {
		Ray ray{camPos, forward};		
		if (type == TargetType::SPHERE) {
			Matrix transform = MatrixTranslate(position.x, position.y, position.z);
			RayCollision collision = GetRayCollisionMesh(ray, models[0].meshes[0], transform);
			if (collision.hit) return HEADSHOT;
			
			return MISS;
		} else if (type == TargetType::BODY) {
			Matrix transform = MatrixTranslate(position.x, position.y, position.z);
			RayCollision collision = GetRayCollisionMesh(ray, models[0].meshes[0], transform);
			if (collision.hit) return HEADSHOT;
			
			transform = MatrixTranslate(position.x, position.y - 2 * size, position.z);
			collision = GetRayCollisionMesh(ray, models[1].meshes[0], transform);
			if (collision.hit) return BODYSHOT;
			
			transform = MatrixTranslate(position.x, position.y - 4.5f * size, position.z);
			collision = GetRayCollisionMesh(ray, models[2].meshes[0], transform);
			if (collision.hit) return BODYSHOT;
			
			transform = MatrixTranslate(position.x, position.y - 4.5f * size, position.z);
			collision = GetRayCollisionMesh(ray, models[1].meshes[0], transform);
			if (collision.hit) return BODYSHOT;
			
			return MISS;
		} else {
			return 0;
		}
	}	
	
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