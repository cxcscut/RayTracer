#include "Object.h"

Object::Object() {}
Object::~Object() {}


Sphere::Sphere(const vec3 &c, float r)
	: center(c), radius(r) {}

bool Sphere::hit_object(RayClass *pr, float t_min, float t_max, HitInfo &info) {
	vec3 oc = pr->p - center;
	float a = dot(pr->dir, pr->dir);
	float b = 2 * dot(oc, pr->dir);
	float c = dot(oc, oc) - radius * radius;

	float discriminant = b*b - 4 * a*c;
	if (discriminant < 0)
		return false;
	else
	{
		float tmp = (-b - sqrtf(discriminant)) / (2.0 * a);
		if (tmp < t_max && tmp > t_min)
		{
			info.t = tmp;
			info.p = pr->point_at(info.t);
			info.n = (info.p - center) / radius;
			return true;
		}
		tmp = (-b + sqrtf(discriminant)) / (2.0 * a);
		if (tmp < t_max && tmp > t_min) {
			info.t = tmp;
			info.p = pr->point_at(info.t);
			info.n = (info.p - center) / radius;
			return true;
		}

		return false;
	}
}