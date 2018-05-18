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
static const int sample_num = 100;

// Compute shader source code 
static const std::string compute_shader_path = "RTComputeShader.glsl";

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
	glewExperimental = true;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed in initializing glew" << std::endl;
		return;
	}

	// Create compute shader
	GLuint ComputeShaderID = glCreateShader(GL_COMPUTE_SHADER);

	std::string ComputeShaderSource;
	std::ifstream ComputeShaderStream(compute_shader_path, std::ios::in);
	
	// Read source code
	if (ComputeShaderStream.is_open())
	{
		std::stringstream sstr;
		sstr << ComputeShaderStream.rdbuf();
		ComputeShaderSource = sstr.str();
		ComputeShaderStream.close();
	}
	else
	{
		std::cout << "unable to open compute shader source code file" << std::endl;
		return;
	}

	// Compile compute shader
	char const *ComputeShaderSourcePointer = ComputeShaderSource.c_str();
	glShaderSource(ComputeShaderID, 1, &ComputeShaderSourcePointer, nullptr);
	glCompileShader(ComputeShaderID);

	GLint res = 0;
	int CompilationStatusInfoLength, LinkingStatusInfoLength;

	// Check the compilation status
	glGetShaderiv(ComputeShaderID, GL_COMPILE_STATUS, &res);
	glGetShaderiv(ComputeShaderID, GL_INFO_LOG_LENGTH, &CompilationStatusInfoLength);

	if (CompilationStatusInfoLength > 0) {
		std::vector<char> CompilationErrorMsg(CompilationStatusInfoLength + 1);
		glGetShaderInfoLog(ComputeShaderID, CompilationStatusInfoLength, nullptr,&CompilationErrorMsg.front());
		std::cout << &CompilationErrorMsg.front() << std::endl;
	}

	// Link program
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, ComputeShaderID);
	glLinkProgram(ProgramID);

	// Check linking status
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &res);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH , &LinkingStatusInfoLength);

	if (LinkingStatusInfoLength > 0)
	{
		std::vector<char> LinkingErrorMsg(CompilationStatusInfoLength + 1);
		glGetShaderInfoLog(ComputeShaderID, CompilationStatusInfoLength, nullptr, &LinkingErrorMsg.front());
		std::cout << &LinkingErrorMsg.front() << std::endl;
	}

	glUseProgram(ProgramID);

	// Create texture for output image
	GLuint _OutTexture;
	glGenTextures(1, &_OutTexture);
	glBindTexture(GL_TEXTURE_2D, _OutTexture);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ImageWidth, ImageHeight, 0 ,GL_RGB, GL_UNSIGNED_BYTE, &pixels.front());

	glBindImageTexture(0, _OutTexture, 0, GL_FALSE, 0 ,GL_WRITE_ONLY, GL_RGB);

	// Dispatch workload 
	glDispatchCompute(1, ImageWidth, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(ImageWidth, ImageHeight);
	glutCreateWindow("Ray Tracing");
	glutDisplayFunc(&display);
	glutMainLoop();

}

void TracingWithCPU(int argc, char* argv[])
{
	Camera camera(vec3(-2.5, -0.7, -3), vec3(0, 0, -1), vec3(0, 1, 0), 90, float(ImageWidth) / float(ImageHeight));
	Hit_List list;

	glRasterPos3f(camera.origin.x, camera.origin.y, camera.origin.z);

	Sphere *pSphere1 = new Sphere(vec3(0, 0, -1), 0.5, new Lambertian(vec3(0.8, 0.3, 0.3)));
	Sphere *pSphere2 = new Sphere(vec3(0, 100.5, -1), 100, new Lambertian(vec3(0.8, 0.8, 0.0)));
	Sphere *pSphere3 = new Sphere(vec3(1, 0, -1), 0.5, new Metal(vec3(0.8, 0.6, 0.2)));
	Sphere *pSphere4 = new Sphere(vec3(-1, 0, -1), 0.5, new Metal(vec3(0.8, 0.8, 0.8)));
	Sphere *pSphere5 = new Sphere(vec3(1, 0, -2), 0.5, new Metal(vec3(0.5, 0.5, 0.3)));
	Sphere *pSphere6 = new Sphere(vec3(1, -0.5, -3.5), 1, new Metal(vec3(0.4, 0.6, 0.7)));

	list.pObjects.push_back(pSphere1);
	list.pObjects.push_back(pSphere2);
	list.pObjects.push_back(pSphere3);
	list.pObjects.push_back(pSphere4);
	list.pObjects.push_back(pSphere5);
	list.pObjects.push_back(pSphere6);

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
	delete pSphere5;
	delete pSphere6;

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
