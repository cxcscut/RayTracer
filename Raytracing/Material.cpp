#include "Material.h"
#include "RayClass.h"
#include "Hitable.h"

Material::Material(const vec3 &a):
	albedo(a)
{}

Lambertian::Lambertian(const vec3 &a) :
	Material(a)
{

}

vec3 Lambertian::getReflectDirRandom(const vec3 &hit, const vec3 &normal) const
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

void Lambertian::scatter(const RayClass &ray_in, const HitInfo &info, vec3 &atteunation, RayClass &scatter_ray) const
{
	vec3 ReflectionDir = getReflectDirRandom(info.p, info.n);
	scatter_ray = RayClass(info.p, ReflectionDir);
	atteunation = albedo;
}

Metal::Metal(const vec3 &a) :
	Material(a)
{}

void Metal::scatter(const RayClass &ray_in, const HitInfo &info, vec3 &atteunation, RayClass &scatter_ray) const
{
	vec3 reflected_dir = glm::normalize(glm::reflect(ray_in.dir,info.n));
	scatter_ray = RayClass(info.p,reflected_dir);
	atteunation = albedo;
}

