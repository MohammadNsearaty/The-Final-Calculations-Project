#ifndef PLANE_H
#define PLANE_H
#pragma once
#include "math3d.h"
#include "Shape.h"
#include"Shpere.h"
#include<glm/glm.hpp>

class Plane {

private:
	vec3 normal;
	float length;

public:
	Plane() {
		normal = vec3(0.0f);
		length = 0.0f;
	}
	Plane(vec3 Normal, float distance)
	{
		normal = Normal;
		length = distance;
	}

	vec3 getNormal() { return normal; }

	float getLength() {
		return length;
	}

	Plane Normalized();


	Collision_Data Collision_Shpere_Plane(Shapes *shape);



};



#endif 
