#include "RayClass.h"

#define MAX_DEPTH 9
#define ABSORBTION_FACTOR 0.3f

RayClass::RayClass(const vec3 _p, const vec3 _dir)
	: p(_p), dir(_dir) {}

RayClass::~RayClass() {}

vec3 RayClass::point_at(float t) { return p + t*dir; }

vec3 RayClass::getReflectDirRandom(const vec3 &hit,const vec3 &normal)
{
	vec3 p;

	std::random_device rd;
	std::default_random_engine generator(rd());
	std::uniform_real_distribution<double> dis(-100.0, 100.0);

	do {

		vec3 pRandom = vec3(dis(generator), dis(generator), dis(generator));
		p = pRandom - hit;

	} while (dot(normal, p) < 0);

	return p;
}

vec3 RayClass::color(Hit_List list,int depth) {
	HitInfo info;

	if (depth < MAX_DEPTH && list.hit(this, 0, numeric_limits<float>::max(), info))
	{
		vec3 ReflectionDir = getReflectDirRandom(info.p,info.n);
		RayClass Reflection(info.p, ReflectionDir);
		return ABSORBTION_FACTOR * Reflection.color(list,depth+1);
	}
	else {
		vec3 unit_dir = normalize(dir);
		float t = 0.5 * (unit_dir.y + 1.0);
		return (1.0f - t)* vec3(1.0, 1.0, 1.0) + t* vec3(0.5, 0.7, 1.0);
	}
}
