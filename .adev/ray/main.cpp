#include <stdio.h>
#include <iostream>
#include "bmp.h"
#include "vector.h"
#include "matrix.h"
#include "ray.h"
#include "sphere.h"
#include "camera.h"
#include "scene.h"
#include "frame_buffer.h"

using namespace std;

int main() {
	Camera cam;
	cam.getTransform().translate(0, 0 , -10);
	//cam.flipZ();
	cam.setRes(640, 480);
	cam.setOrtho(true);
	cam.setAperture(5.0f);
	
	Scene scene;
	scene.setCamera(cam);
	Sphere *sph = scene.create<Sphere>("Sphere");
	sph->size = 2.0f;
	sph->getTransform().translate(0.0, 0, 5);
	Material mat;
	mat.setDiffuse(Vector4f(250, 200, 70));
	sph->setMaterial(&mat);
	
	Light *li = scene.create<Light>("Light");
	li->getTransform().translate(0,5,-10);
	
	FrameBuffer &buff = scene.render();
	buff.save("/sdcard/renderOut.bmp", "bmp");
	printf("Done!\n");
	return 0;
};
