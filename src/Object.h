#pragma once
#include "RayClass.h"
#include "Hitable.h"
class RayClass;
class HitInfo;

class Object
{
public:

	Object();
	virtual ~Object();

	virtual bool hit_object(RayClass *pr,float t_min, float t_max, HitInfo &info) = 0;
};

class Sphere : public Object
{
public:

	Sphere(const vec3 &c, float r);

	virtual bool hit_object(RayClass *pr, float t_min, float t_max, HitInfo &info);

public:

	vec3 center;
	float radius;
};