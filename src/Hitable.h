#pragma once
#include "Basic.h"
#include "RayClass.h"
#include "Object.h"

class RayClass;
class Object;

using HitInfo = struct HitInfo {
	float t;
	vec3 p;
	vec3 n;
};

class Hit_List
{
public:

	bool hit(RayClass *pr, float t_min, float t_max, HitInfo &info);

public:
	vector<Object*> pObjects;
};