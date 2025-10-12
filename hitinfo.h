#ifndef HITINFO_H
#define HITINFO_H

#include "target.h"

class Target;

struct HitInfo {
	HitInfo(Target* target, int hitType, float distance) {
		this->target = target;
		this->hitType = hitType;
		this->distance = distance;
	}
	
	Target* target;
	int hitType;
	float distance;
};

#endif