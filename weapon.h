#ifndef WEAPON_H
#define WEAPON_H

#include "raylib.h"
#include "raymath.h"

#include <string>

class Weapon {
public:
	Weapon(std::string objPath, std::string diffusePath, std::string normalPath, Shader& shader) {
		this->model = LoadModel(objPath.c_str());
		this->diffuseMap = LoadTexture(diffusePath.c_str()); 
		this->normalMap = LoadTexture(normalPath.c_str());

		this->model.materials[0].shader = shader;
		this->model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = diffuseMap;
		this->model.materials[0].maps[MATERIAL_MAP_NORMAL].texture = normalMap;
		
		GenTextureMipmaps(&this->model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture);
		GenTextureMipmaps(&this->model.materials[0].maps[MATERIAL_MAP_NORMAL].texture);

		SetTextureFilter(this->model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture, TEXTURE_FILTER_TRILINEAR);
		SetTextureFilter(this->model.materials[0].maps[MATERIAL_MAP_NORMAL].texture, TEXTURE_FILTER_TRILINEAR);
	}
	
	~Weapon() {
		UnloadTexture(diffuseMap);
		UnloadTexture(normalMap);
		UnloadModel(model);
	}
	
	void draw(Vector3 position, float size) {
		DrawModel(this->model, position, 0.1f, WHITE);
	}
	
	Model model;
	Texture2D normalMap;
	Texture2D diffuseMap;
};

#endif