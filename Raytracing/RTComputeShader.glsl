#version 430 core

#define MAX_DEPTH 50
#define IMAGE_WIDTH 1200
#define IMAGE_HEIGHT 600

struct Ray{
	vec3 p;
	vec3 dir;
};

struct Sphere{
	vec3 center;
	float r;
	int material;
	vec3 atteunation;
};

// Hit record
struct hitInfo{
	bool isHit;
	vec3 p;
	float t;
	vec3 n;
	vec3 scatter_dir;
};

uniform Sphere sphere1,sphere2,sphere3,sphere4;

// Sampling num
uniform int sample_num;
uniform int sphere_num;

uniform float MAX_FLOAT;

// Camera properties
uniform vec3 origin;
uniform vec3 lower_left_corner;
uniform vec3 horizontal;
uniform vec3 vertical;

// Local workgroup size
layout(local_size_x=IMAGE_HEIGHT)

// Output image
layout(rgb32f, binding = 2) uniform image2D output_image;

float rand(vec2 co){
	return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
	Sphere spheres[] = {sphere1,sphere2,sphere3,sphere4};
	
	ivec2 pos = ivec2(gl_GlobalInvocationID.xy);
	
	float r = rand(pos);

	float u = (pos.x + sin(r * 3.141592)) / IMAGE_WIDTH;
	float v = (pos.y + cos(r * 3.141592)) / IMAGE_HEIGHT;
	
	vec3 color;

	for(int s = 0; s < sample_num; s++)
	{
		Ray outgoing;
		outgoing.p = origin;
		outgoing.dir = lower_left_corner + u * horizontal + v * vertical - origin;
		
		color += colorProc(outgoing,spheres,4,0);
	}

	color /= sample_num;
	
	// Write back to the output image
	imageStore(output_image, pos.yx, color);
}

vec3 colorProc(Ray ray,Sphere spheres[],int num,int depth)
{
	
	vec3 unit_dir = normalize(ray.dir);
	float t = 0.5 * (unit_dir.y + 1.0);
	vec3 env_color = (1.0f - t) * vec3(1.0,1.0,1.0) + t * vec3(0.5,0.7,1.0);	

	if(depth < MAX_DEPTH){

		float closest = MAX_FLOAT;
		hitInfo info;
		
		// Find closest intersection point
		for(int i = 0;i< num;i++){
			
			info = isIntersection(ray,spheres[i],0,closest);
			
			if(info_tmp.isHit && dot(info.scatter.dir,info.n) > 0){
				cloest = info.t;
			}
		}
		
		if(info.isHit){
			if(dot(info.scatter_dir,info.n) > 0){
				Ray scatter_ray;
				scatter_ray.p = info.p;
				scatter_ray.dir = info.scatter_dir;

				return spheres[i].atteunation * colorProc(scatter_ray, spheres, num, depth + 1)
			}
			else
				return vec3(0,0,0);
		}
		else
			return env_color;
	}
	
	return env_color;
	
}

hitInfo isIntersection(Ray incoming, Sphere sphere,float t_min, float t_max)
{
	hitInfo info;

	vec3 oc = incoming.p - sphere.center;
	float a = dot(incoming.dir,incoming.dir);
	float b = 2 * dot(oc, incoming.dir);
	float c = dot(oc,oc) - sphere.r * sphere.r;

	float discriminant = b * b - 4 * a * c;
	if(discriminant < 0)
	{
		ret.isHit = false;
		return info;
	}
	else
	{
		float tmp1 = (-b - sqrtf(discriminant)) / (2.0 * a);
		float tmp2 = (-b + sqrtf(discriminant)) / (2.0 * a);

		float tmp = min(tmp1,tmp2);
		
		if(tmp < t_max && tmp > t_min)
		{
			info.t = tmp;
			info.p = incoming.p + info.t * incoming.dir;
			info.n = (info.p - sphere.center) / sphere.r;
			
			if(sphere.material == 0)
			{
				float r = 0;
				
				// Diffuse material
				do{
					r = rand(gl_LocalInvocationID.xy + r);

					vec3 pRandom = vec3(
						r,
						sin(r * 3.141592),
						cos(r * 3.141592)
					);
					
					vec3 p = pRandom - info.p;
				}while(dot(info.n,p) < 0);
				
				info.scatter_dir = p;
			}
			else if(sphere.material == 1)
			{
				// Metal material
				info.scatter_dir = normailze(reflect(incoming.dir,info.n));
			}
		}
	}
}