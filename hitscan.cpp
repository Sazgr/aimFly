#include "hitscan.h"
#include <bits/stdc++.h>

bool hitscan(Vector3 camPos, Vector3 forward, Target& target) {
	Vector3 delta = Vector3Subtract(target.position, camPos);
	Vector3 projection = Vector3Scale(forward, Vector3DotProduct(delta, forward) / Vector3LengthSqr(forward));
	float distance = Vector3Distance(delta, projection);
	return (distance <= target.size);
}