#ifndef SPHERETARGET_H
#define SPHERETARGET_H

#include "raylib.h"
#include "raymath.h"

class SphereTarget {
public:
	SphereTarget(Vector3 position, float size) {
		this->position = position;
		this->size = size;
		Mesh mesh = GenMeshSphere(size, 32, 32);
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
	Vector3 velocity;
	float size;
	Model model;
};

#endif