#pragma once

#ifndef PLANE_H
#define PLANE_H
//#include "math3d.h"
//#include "Shape.h"
//#include"Shpere.h"
#include<glm/glm.hpp>
using namespace glm;
class Plane {

private:
	vec3 normal;
	float distance;

public:
	Plane() {
		normal = vec3(0.0f);
		distance = 0.0f;
	}
	Plane(vec3 Normal, float distance)
	{
		normal = Normal;
		this->distance = distance;
	}

	vec3 getNormal() { return normal; }

	float getDistance() {
		return this->distance;
	}

	Plane Normalized(){
		float magnitude = glm::length(normal);
		return Plane(normal / magnitude, distance / magnitude);
	}


	//Collision_Data Collision_Shpere_Plane(Shapes *shape);



};



#endif 
