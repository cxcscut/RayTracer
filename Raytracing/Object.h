#pragma once
#include "RayClass.h"
#include "Hitable.h"
#include "Material.h"

class RayClass;
class HitInfo;

class Object
{
public:

	Object(Material *pM);
	virtual ~Object();

	virtual bool hit_object(const RayClass &pr,float t_min, float t_max, HitInfo &info, RayClass &scatter_ray, vec3 &atteunation) = 0;

public:

	Material *pMaterial;
};

class Sphere : public Object
{
public:

	Sphere(const vec3 &c, float r,Material *pM);

	virtual bool hit_object(const RayClass &pr, float t_min, float t_max, HitInfo &info,RayClass &scatter_ray, vec3 &atteunation);

public:

	vec3 center;
	float radius;
};