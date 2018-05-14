#include "basic.h"
#include "RayClass.h"

#ifndef CAMERA_H
#define CAMERA_H

class Camera {

public :

	Camera();

	RayClass getRay(float u, float v);

public:

	glm::vec3 origin;
	glm::vec3 lower_left_corner;
	glm::vec3 horizontal;
	glm::vec3 vertical;
};

#endif // CAMERA_H
