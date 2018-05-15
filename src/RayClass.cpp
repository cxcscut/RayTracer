#include "RayClass.h"

#define MAX_DEPTH 50

RayClass::RayClass() {}

RayClass::RayClass(const vec3 _p, const vec3 _dir)
	: p(_p), dir(_dir) {}

RayClass::~RayClass() {}

vec3 RayClass::point_at(float t) const { return p + t*dir; }

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
	RayClass scatter;
	vec3 atteunation;

	if (depth < MAX_DEPTH && list.hit(*this, 0, numeric_limits<float>::max(), info, scatter, atteunation))
	{
		if (dot(scatter.dir, info.n) > 0)
			return atteunation * scatter.color(list, depth + 1);
		else
			return vec3(0,0,0);
	}
	else {
		vec3 unit_dir = normalize(dir);
		float t = 0.5 * (unit_dir.y + 1.0);
		return (1.0f - t)* vec3(1.0, 1.0, 1.0) + t* vec3(0.5, 0.7, 1.0);
	}
}
