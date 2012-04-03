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
#include <time.h>

using namespace std;

int main() {
	Camera cam;
	//cam.getTransform().translate(0, 0, 0);
	//cam.flipZ();
	cam.getTransform().setTarget(Vector4f(0, 0, 0));
	cam.getTransform().setPosition(Vector4f(0,0,-10));
	cam.setRes(32, 24);
	cam.setOrtho(true);
	cam.setFov(7.0f);
	//cam.getTransform().orbit(.5, 0);
	Scene scene;
	scene.setCamera(cam);
	
	Sphere *sph = scene.create<Sphere>("Sphere");
	sph->size = 3.0f;
	sph->getTransform().translate(0, 0, 0);
	Material mat;
	mat.setDiffuse(Vector4f(250, 200, 70));
	sph->setMaterial(&mat);
	
	sph = scene.create<Sphere>("Sphere");
	sph->size = 1.0f;
	sph->getTransform().translate(-1, 0, 2);
	Material mat2;
	mat2.setDiffuse(Vector4f(200, 250, 70));
	sph->setMaterial(&mat2);
	
	Light *li = scene.create<Light>("Light");
	li->getTransform().translate(1, 5, 0);
	
	//li = scene.create<Light>("Light");
	//li->getTransform().translate(-3, -3, 0);
	for (int i = 0; i < 1; i++) {
		FrameBuffer &buff = scene.render();
		stringstream ss;
		time_t start = time(NULL);
		ss << "/sdcard/renderOut_" << i << ".bmp";
		buff.save(ss.str().c_str(), "bmp");
		time_t end = time(NULL);
		printf("Done %d in %d!\n", i, (end - start) );
		scene.getCamera().getTransform().orbit(0.1, 0.0);
	}
	scene.getCamera().getTransform().getWorldToObj().displayMatrix();
	return 0;
};
