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

#include "jpeg.h"
#include "any.h"

#include <map>
#include <string>

using namespace std;
using namespace boost;

int main() {

	//boost::any a(5);

	//cout << "Value: " << boost::any_cast<int>(a) << '\n';
	map<string, any> m;
	m["hello"] = 5;
	m["there"] = string("how");
	m["are"] = 7.3f;

	cout << "Map: " << any_cast<int>(m["hello"]) << ", " << any_cast<string>(m["there"]) << ", " << any_cast<float>(m["are"]) << '\n';

	cin.get();

	Camera cam;
	//cam.getTransform().translate(0, 0, 0);
	//cam.flipZ();
	cam.getTransform().setPosition(Vector4f(0,0,-10));
	cam.getTransform().setTarget(Vector4f(0, 0, 0));
	
	cam.setRes(320, 240);
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
		ss << "renderOut_";
		if (i < 10) {
			ss << '0';
		}
		ss << i << ".jpg";
		buff.save(ss.str().c_str(), "jpg");
		time_t end = time(NULL);
		printf("Done %d in %d!\n", i, (end - start) );
		scene.getCamera().getTransform().orbit(0.1, 0.0);
	}
	scene.getCamera().getTransform().getWorldToObj().displayMatrix();
	cin.get();
	return 0;
};
