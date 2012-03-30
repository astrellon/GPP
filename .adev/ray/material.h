#pragma once

 class Material {
 public:
 	Material() {
 		diffuse.x = 1.0f;
 		diffuse.y = 1.0f;
 		diffuse.z = 0.0f;
 	}
 	~Material() {}
 	
 	Vector4f getDiffuse() const {
 		return diffuse;
 	}
 	
 	void setDiffuse(const Vector4f &colour) {
 		diffuse = colour;
 	}
 protected:
 	Vector4f diffuse;
 };
   
