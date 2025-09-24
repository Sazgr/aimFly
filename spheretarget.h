#ifndef SPHERETARGET_H
#define SPHERETARGET_H

#include "raylib.h"
#include "raymath.h"

class SphereTarget {
	public:
	SphereTarget(Vector3 position, float radius) {
		this->position = position;
		this->radius = radius;
		Mesh mesh = GenMeshSphere(radius, 32, 32);
		this->model = LoadModelFromMesh(mesh);
		//UnloadMesh(mesh); 
	}
	
	~SphereTarget() {
        UnloadModel(this->model);
    }
	
	void addShader(Shader& shader) {
		this->model.materials[0].shader = shader;
	}
	
	void draw() {
		DrawModel(this->model, position, 1.0f, PINK);
	}
	
	Vector3 position;
	float radius;
	Model model;
};

#endif