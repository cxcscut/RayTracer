
#include "Camera.h"

Camera::Camera(const vec3 pos, const vec3 &lookat, const vec3 &up,
	float fov, float aspect)
{
	vec3 u, w;
	float theta = glm::radians(fov);
	float half_height = tanf(theta/2);
	float half_width = aspect * half_height;

	w = glm::normalize(pos - lookat);
	u = glm::normalize(glm::cross(up,w));

	origin = pos;
	lower_left_corner = origin - w - half_width * u - half_height * up;
	horizontal = 2 * half_width * u;
	vertical = 2 * half_height * up;
}

RayClass Camera::getRay(float u, float v) const
{
	return RayClass(origin, lower_left_corner + u * horizontal + v * vertical - origin);
}