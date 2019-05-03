#ifndef PLANE_H
#define PLANE_H
#pragma once
#include "math3d.h"
#include "Shape.h"
#include"Shpere.h"


class Plane {

private:
	Vector3f normal;
	float length;

public:
	Plane() {
		normal = 0.0f;
		length = 0.0f;
	}
	Plane(Vector3f Normal, float distance)
	{
		normal = Normal;
		length = distance;
	}

	Vector3f getNormal() { return normal; }

	float getLength() {
		return length;
	}

	Plane Normalized();


	Collision_Data Collision_Shpere_Plane(Shapes *shape);



};



#endif 
