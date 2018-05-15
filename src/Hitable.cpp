#include "Hitable.h"

bool Hit_List::hit(const RayClass &pr, float t_min, float t_max, HitInfo &info, RayClass &scatter_ray,vec3 &atteunation)
{
	bool hit_anything = false;
	HitInfo tmp_info;
	float closest = t_max;
	for (Object* object : pObjects)
	{
		if (object->hit_object(pr, t_min, closest, tmp_info,scatter_ray,atteunation))
		{
			hit_anything = true;
			closest = tmp_info.t;
			info = tmp_info;
		}
	}

	return hit_anything;
}


