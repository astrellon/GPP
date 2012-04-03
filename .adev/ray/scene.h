#pragma once

#include "camera.h"
#include "iscene_element.h"
#include "bmp.h"
#include "frame_buffer.h"
#include "irenderable.h"
#include "light.h"
#include "math.h"
#include "ray.h"
#include "ray_hit_result.h"

#include <vector>

using namespace std;

class Scene {
public:
	Scene() : ambientLight(0.2f, 0.2f, 0.2f) {}
	~Scene() {}
	
	inline Camera &getCamera() {
		return camera;
	}
	inline void setCamera(const Camera &cam) {
		camera = cam;
	}
	
	void preproc() {
		camera.getTransform().preproc();
		for (int i = 0; i < renderables.size(); i++) {
			renderables[i]->getTransform().preproc();
		}
		for (int i = 0; i < lights.size(); i++) {
			lights[i]->getTransform().preproc();
		}
	}
	
	FrameBuffer &render() {
		maxLum = 1e-36f;
		minLum = 1e36f;
		int resx = camera.getResX();
		int resy = camera.getResY();
		if (framebuf.getWidth() != resx ||
			framebuf.getHeight() != resy) {
			framebuf.setSize(resx, resy);
		}
		preproc();
		printf("Num scene elements: %d\n", renderables.size());
		printf("Num scene light: %d\n", lights.size());
		for (int y = 0; y < resy; y++) {
			for (int x = 0; x < resx; x++) {
				Ray r = camera.getRay(x, y);
				//printf("Firing ray for (%d, %d): ", x, y);
				fireRay(r);
				float lum = calcLum(r.colour);
				//printf("%f\n", lum);
				maxLum = max(maxLum, lum);
				minLum = min(minLum, lum);
				framebuf.setPixel(x, y, r.colour);
			}
		}
		return framebuf;
	}
	
	RayHitResult fireRay(Ray &r) const {
		RayHitResult result;
		//cout << "Firing " << r.getStart() << " at " << renderables.size() << " objects\n";
		float closest = 1e36;
		for (int i = 0; i < renderables.size(); i++) {
			//printf("Check obj %d ", i);
			IRenderable *c = renderables[i];
			RayHitResult hitResult;
			bool hits = c->collides(r, hitResult);
			//printf("%d\n", hits);
			//cout << r.getStart() << ',' << i << " hits: " << hits << '\n';
			if (hits) {
				if (hitResult.len < closest) {
					closest = hitResult.len;
					//closestObj = hitResult.target;
					result = hitResult;
				}
			}
		}
		if (result.target != NULL) {
			Vector4f lightColour = getLightColour(result.worldPos.add(result.normal.scale(0.01)), result.normal);
				//Vector4f lightColour = getLightColour(result.worldPos, result.normal);
				
			const Material *mat = result.target->getMaterial();
			r.colour.addTo(lightColour.multiply(mat->getDiffuse()));
		}
		return result;
	}
	
	Vector4f getLightColour(const Vector4f &pos, const Vector4f &normal) const {
		Vector4f colour = ambientLight;
		for (int i = 0; i < lights.size(); i++) {
			Light *light = lights[i];
			Vector4f toLight = light->getPosition().sub(pos).normalise();
			float dot = toLight.dot(normal);
			if (dot <= 0) {
				continue;
			}
			Ray lightRay(pos, toLight);
			
			//cout << "Light ray "<< toLight << " from " << pos << " hits: ";
			RayHitResult hit = fireRay(lightRay);
			// For now.
			//cout << (hit.target == NULL ? "nothing\n" : "sphere\n");
			if (hit.target != NULL) {
				continue;
			}
			colour.addTo(light->colour.scale(dot));
		}
		return colour;
	}
	
	template <class T>
	T *create(const string &type) {
		ISceneElement *element = NULL;
		if (type.compare("Sphere") == 0) {
			Sphere *s = new Sphere();
			renderables.push_back(s);
			element = s;
		}
		else if (type.compare("Light") == 0) {
			Light *l = new Light();
			lights.push_back(l);
			element = l;
		}
		if (element != NULL) {
			//elements.push_back(element);
			T *ret = static_cast<T*>(element);
			if (ret == NULL) {
				printf("Unable to cast created object to '%s'\n", type.c_str());
			}
			return ret;
		}
		return NULL;
	}
	
	FrameBuffer &getFrameBuffer() {
		return framebuf;
	}
	
	inline float calcLum(const Vector4f &p) const {
		return p.x * 0.7f + p.y * 0.65f + p.z *0.05f;
	}
	
	inline float getMaxLum() const {
		return maxLum;
	}
	inline float getMinLum() const {
		return minLum;
	}
protected:
	vector<IRenderable *> renderables;
	vector<Light *> lights;
	FrameBuffer framebuf;
	Camera camera;
	Vector4f ambientLight;
	
	float maxLum;
	float minLum;
};
