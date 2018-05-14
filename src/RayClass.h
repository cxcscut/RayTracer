#pragma once
#include "Basic.h"
#include "Hitable.h"

class HitInfo;
class Hit_List;

class RayClass
{
public:

	RayClass(const vec3 _p, const vec3 _dir);

	virtual ~RayClass();

	vec3 point_at(float t);

	vec3 color(Hit_List list,int depth = 0);

	vec3 getReflectDirRandom(const vec3 &hit,const vec3 &normal);

public:

	vec3 p;
	vec3 dir;
};


