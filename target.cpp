#include "hitinfo.h"
#include "target.h"

HitInfo Target::hitscan(Vector3 camPos, Vector3 forward) {
	Ray ray{camPos, forward};		
	if (type == TargetType::SPHERE) {
		Matrix transform = MatrixTranslate(position.x, position.y, position.z);
		RayCollision collision = GetRayCollisionMesh(ray, models[0].meshes[0], transform);
		if (collision.hit) return HitInfo(this, HEADSHOT, collision.distance);
		
		return HitInfo(nullptr, MISS, 0);
	} else if (type == TargetType::BODY) {
		Matrix transform = MatrixTranslate(position.x, position.y, position.z);
		RayCollision collision = GetRayCollisionMesh(ray, models[0].meshes[0], transform);
		if (collision.hit) return HitInfo(this, HEADSHOT, collision.distance);
		
		transform = MatrixTranslate(position.x, position.y - 2 * size, position.z);
		collision = GetRayCollisionMesh(ray, models[1].meshes[0], transform);
		if (collision.hit) return HitInfo(this, BODYSHOT, collision.distance);
		
		transform = MatrixTranslate(position.x, position.y - 4.5f * size, position.z);
		collision = GetRayCollisionMesh(ray, models[2].meshes[0], transform);
		if (collision.hit) return HitInfo(this, BODYSHOT, collision.distance);
		
		transform = MatrixTranslate(position.x, position.y - 4.5f * size, position.z);
		collision = GetRayCollisionMesh(ray, models[1].meshes[0], transform);
		if (collision.hit) return HitInfo(this, BODYSHOT, collision.distance);
		
		return HitInfo(nullptr, MISS, 0);
	} else {
		return HitInfo(nullptr, MISS, 0);
	}
}