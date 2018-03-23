#include "RayClass.h"

RayClass::RayClass(const vec3 _p, const vec3 _dir)
	: p(_p), dir(_dir) {}

RayClass::~RayClass() {}

vec3 RayClass::point_at(float t) { return p + t*dir; }

vec3 RayClass::color(Hit_List list) {
	HitInfo info;
	if (list.hit(this, 0, numeric_limits<float>::max(), info))
	{
		return 0.5f * (info.n + vec3(1, 1, 1));
	}
	else {
		vec3 unit_dir = normalize(dir);
		float t = 0.5 * (unit_dir.y + 1.0);
		return (1.0f - t)* vec3(1.0, 1.0, 1.0) + t* vec3(0.5, 0.7, 1.0);
	}
}
