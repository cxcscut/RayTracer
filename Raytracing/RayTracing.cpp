// RayTracing.cpp: 定义控制台应用程序的入口点。
//

#include "RayClass.h"
#include "Basic.h"
#include "RayClass.h"
#include "Object.h"
#include "Hitable.h"
#include "Camera.h"

static const GLint ImageWidth = 1200;
static const  GLint ImageHeight = 600;
static vector<float> pixels;

// 16x sampling for antialiasing
static const int sample_num = 32;
;

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_BLEND);

	glDrawPixels(ImageWidth, ImageHeight,
		GL_RGB, GL_FLOAT, &pixels.front());

	glutSwapBuffers();
}

void colorProc(int i ,int j,int start, int end, const Camera &camera, vec3 &color, const Hit_List &list)
{
	std::random_device rd;
	std::default_random_engine generator(rd());
	std::uniform_real_distribution<double> dis(-1.0, 1.0);

	for (int s = start; s < end; s++)
	{
		RayClass r = camera.getRay(
			float(i + dis(generator)) / float(ImageWidth),
			float(j + dis(generator)) / float(ImageHeight)
		);

		color += r.color(list);
	}
}

void renderProc(const Camera &camera, const Hit_List &list,int height_start, int height_end, vector<float> &subpixels)
{
	std::random_device rd;
	std::default_random_engine generator(rd());
	std::uniform_real_distribution<double> dis(-1.0, 1.0);

	for (int j = height_end - 1; j >= height_start; j--)
		for (int i = 0; i < ImageWidth; i++)
		{
			vec3 color(0, 0, 0);

			colorProc(i, j, 0, sample_num, camera, color, list);

			color /= float(sample_num);

			subpixels.push_back(color.r);
			subpixels.push_back(color.g);
			subpixels.push_back(color.b);
		}
}

void TracingWithGPU(int argc, char* argv[])
{

}

void TracingWithCPU(int argc, char* argv[])
{
	Camera camera(vec3(-2, -2, 1), vec3(0, 0, -1), vec3(0, 1, 0), 90, float(ImageWidth) / float(ImageHeight));
	Hit_List list;

	glRasterPos3f(camera.origin.x, camera.origin.y, camera.origin.z);

	Sphere *pSphere1 = new Sphere(vec3(0, 0, -1), 0.5, new Lambertian(vec3(0.8, 0.3, 0.3)));
	Sphere *pSphere2 = new Sphere(vec3(0, 100.5, -1), 100, new Lambertian(vec3(0.8, 0.8, 0.0)));
	Sphere *pSphere3 = new Sphere(vec3(1, 0, -1), 0.5, new Metal(vec3(0.8, 0.6, 0.2)));
	Sphere *pSphere4 = new Sphere(vec3(-1, 0, -1), 0.5, new Metal(vec3(0.8, 0.8, 0.8)));

	list.pObjects.push_back(pSphere1);
	list.pObjects.push_back(pSphere2);
	list.pObjects.push_back(pSphere3);
	list.pObjects.push_back(pSphere4);

	std::random_device rd;
	std::default_random_engine generator(rd());
	std::uniform_real_distribution<double> dis(-1.0, 1.0);

	std::vector<float> subpixels1, subpixels2, subpixels3, subpixels4, subpixels5, subpixels6;

	std::thread renderThread1(
		renderProc,
		std::cref(camera),
		std::cref(list),
		500, 600,
		std::ref(subpixels1)
	);

	std::thread renderThread2(
		renderProc,
		std::cref(camera),
		std::cref(list),
		400, 500,
		std::ref(subpixels2)
	);

	std::thread renderThread3(
		renderProc,
		std::cref(camera),
		std::cref(list),
		300, 400,
		std::ref(subpixels3)
	);

	std::thread renderThread4(
		renderProc,
		std::cref(camera),
		std::cref(list),
		200, 300,
		std::ref(subpixels4)
	);

	std::thread renderThread5(
		renderProc,
		std::cref(camera),
		std::cref(list),
		100, 200,
		std::ref(subpixels5)
	);

	std::thread renderThread6(
		renderProc,
		std::cref(camera),
		std::cref(list),
		0, 100,
		std::ref(subpixels6)
	);

	renderThread1.join();
	renderThread2.join();
	renderThread3.join();
	renderThread4.join();
	renderThread5.join();
	renderThread6.join();

	pixels.insert(pixels.end(), subpixels1.begin(), subpixels1.end());
	pixels.insert(pixels.end(), subpixels2.begin(), subpixels2.end());
	pixels.insert(pixels.end(), subpixels3.begin(), subpixels3.end());
	pixels.insert(pixels.end(), subpixels4.begin(), subpixels4.end());
	pixels.insert(pixels.end(), subpixels5.begin(), subpixels5.end());
	pixels.insert(pixels.end(), subpixels6.begin(), subpixels6.end());


	delete pSphere1;
	delete pSphere2;
	delete pSphere3;
	delete pSphere4;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(ImageWidth, ImageHeight);
	glutCreateWindow("Ray Tracing");
	glutDisplayFunc(&display);
	glutMainLoop();
}

int main(int argc, char* argv[])
{
	TracingWithCPU(argc, argv);
	// TracingWithGPU(argc, argv);

	return 0;
}
