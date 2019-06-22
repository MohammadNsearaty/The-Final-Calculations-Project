#pragma once
#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H
#include <glm/glm.hpp>
using namespace glm;
class BoundingBox {
private:
	glm::vec3 minX, maxX;
	glm::vec3 minY, maxY;
	glm::vec3 minZ, maxZ;
public:
	glm::vec3 min, max;
	BoundingBox()
	{
		min = max = vec3(0.0f);
	}
	BoundingBox(vec3 x1, vec3 x2, vec3 y1, vec3 y2, vec3 z1, vec3 z2)
	{
		minX = x1; maxX = x2;
		minY = y1; maxY = y2;
		minZ = z1; maxZ = z2;
		min = vec3(x1, y1, z1);
		max = vec3(x2, y2, z2);
	}
	BoundingBox(vec3 min, vec3 max)
	{
		this->min = min;
		this->max = max;
	}
	bool contain(Shapes s) {};

};


#endif
