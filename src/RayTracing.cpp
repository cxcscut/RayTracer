// RayTracing.cpp: 定义控制台应用程序的入口点。
//

#include "RayClass.h"
#include "Basic.h"
#include "RayClass.h"
#include "Object.h"
#include "Hitable.h"

static GLint     ImageWidth;
static GLint     ImageHeight;
static vector<float> pixels;

#include <stdio.h>
#include <stdlib.h>

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_BLEND);

	// 绘制像素
	glDrawPixels(ImageWidth, ImageHeight,
		GL_RGB, GL_FLOAT, &pixels.front());

	// 完成绘制
	glutSwapBuffers();
}

int main(int argc, char* argv[])
{
	ImageWidth = 800;
	ImageHeight = 800;

	vec3 bottom_left(-1.0,-1.0,-1.0);
	vec3 horizontal(2.0,0,0);
	vec3 vertical(0,2.0,0);
	vec3 LightOrigin(0,0,0);
	Hit_List list;

	glRasterPos3f(LightOrigin.x,LightOrigin.y,LightOrigin.z);

	Sphere *pSphere = new Sphere(vec3(0,0,-1),0.5);
	Sphere *pSphere2 = new Sphere(vec3(0,100.5,-1),100);

	list.pObjects.push_back(pSphere);
	list.pObjects.push_back(pSphere2);

	for(int j = ImageHeight-1;j>=0;j--)
		for (int i = 0; i < ImageWidth; i++)
		{
			float u = float(i) / float(ImageWidth);
			float v = float(j) / float(ImageHeight);
			RayClass r(LightOrigin,bottom_left + u * horizontal + v * vertical);
			vec3 color = r.color(list);

			pixels.push_back(color.r);
			pixels.push_back(color.g);
			pixels.push_back(color.b);
		}

	delete pSphere;
	delete pSphere2;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(ImageWidth, ImageHeight);
	glutCreateWindow("Ray Tracing");
	glutDisplayFunc(&display);
	glutMainLoop();

	return 0;
}


