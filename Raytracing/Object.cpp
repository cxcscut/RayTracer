#include "Object.h"

Object::Object(Material *pM) :
	pMaterial(pM)
{}
Object::~Object() {
	if (pMaterial) {
		delete pMaterial;
		pMaterial = nullptr;
	}
}


Sphere::Sphere(const vec3 &c, float r, Material *pM)
	: center(c), radius(r), Object(pM)
{}

bool Sphere::hit_object(const RayClass &pr, float t_min, float t_max, HitInfo &info, RayClass &scatter_ray,vec3 &atteunation) {
	vec3 oc = pr.p - center;
	float a = dot(pr.dir, pr.dir);
	float b = 2 * dot(oc, pr.dir);
	float c = dot(oc, oc) - radius * radius;

	float discriminant = b*b - 4 * a*c;
	if (discriminant < 0)
		return false;
	else
	{
		float tmp1 = (-b - sqrtf(discriminant)) / (2.0 * a);
		float tmp2 = (-b + sqrtf(discriminant)) / (2.0 * a);
		
		float tmp = std::fmin(tmp1, tmp2);

		if (tmp < t_max && tmp > t_min)
		{
			info.t = tmp;
			info.p = pr.point_at(info.t);
			info.n = (info.p - center) / radius;
			pMaterial->scatter(pr, info, pMaterial->albedo, scatter_ray);
			atteunation = pMaterial->albedo;
			return true;
		}
		return false;
	}
}