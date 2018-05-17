#include "basic.h"
#include "RayClass.h"

#ifndef CAMERA_H
#define CAMERA_H

class Camera {

public :

	Camera(const vec3 pos, const vec3 &lookat, const vec3 &up,
		float fov, float aspect);

	RayClass getRay(float u, float v) const;

public:

	glm::vec3 origin;
	glm::vec3 lower_left_corner;
	glm::vec3 horizontal;
	glm::vec3 vertical;
};

#endif // CAMERA_H
