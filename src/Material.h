#include "basic.h"

#ifndef MATERIAL_H
#define MATERIAL_H

class RayClass;
class HitInfo;

class Material {

public:
	
	Material(const vec3 & a);

	virtual void scatter(const RayClass &ray_in, const HitInfo &info, vec3 &attenuation, RayClass &scatter_ray) const = 0;

public:

	vec3 albedo;
};

class Lambertian : public Material
{

public:

	Lambertian(const vec3& a);

	virtual void scatter(const RayClass &ray_in, const HitInfo &info, vec3 &attenuation, RayClass &scatter_ray) const;

	vec3 getReflectDirRandom(const vec3 &hit, const vec3 &normal) const;

};

class Metal : public Material
{

public:

	Metal(const vec3 &a);

	virtual void scatter(const RayClass &ray_in, const HitInfo &info, vec3 &attenuation, RayClass &scatter_ray) const;

};

#endif // MATERIAL_H
