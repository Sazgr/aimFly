#include "hitinfo.h"

bool operator<(HitInfo a, HitInfo b) {
	return a.distance < b.distance;
}