#include "hitinfo.h"
#include "worldobject.h"

WorldObject::WorldObject(Vector3 pos, float width, float height, float length, bool interactable = true) {
	position = pos;
	Mesh mesh = GenMeshCube(width, height, length);
	model = LoadModelFromMesh(mesh);
	this->interactable = interactable;
}

WorldObject::~WorldObject() {
	UnloadModel(model);
}

void WorldObject::addShader(Shader& shader) {
	this->model.materials[0].shader = shader;
}

void WorldObject::draw() {
	DrawModel(model, position, 1.0f, GRAY);
}

HitInfo WorldObject::hitscan(Vector3 camPos, Vector3 forward) {
	if (!interactable) return HitInfo(nullptr, MISS, 0);
	
	Ray ray{camPos, forward};		
	
	Matrix transform = MatrixTranslate(position.x, position.y, position.z);
	RayCollision collision = GetRayCollisionMesh(ray, model.meshes[0], transform);
	
	return HitInfo(nullptr, collision.hit ? HIT : MISS, collision.distance);
}