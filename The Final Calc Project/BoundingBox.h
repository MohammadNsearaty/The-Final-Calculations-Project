#pragma once
#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H
#include <glm/glm.hpp>
#include "Shape.h"
#include<freeglut.h>
class BoundingBox {
//private:
//	glm::vec3 minX, maxX;
//	glm::vec3 minY, maxY;
//	glm::vec3 minZ, maxZ;
//public:
//	glm::vec3 min, max;
//	BoundingBox()
//	{
//		min = max = vec3(0.0f);
//	}
//	BoundingBox(vec3 x1, vec3 x2, vec3 y1, vec3 y2, vec3 z1, vec3 z2)
//	{
//		minX = x1; maxX = x2;
//		minY = y1; maxY = y2;
//		minZ = z1; maxZ = z2;
//		min = vec3(x1, y1, z1);
//		max = vec3(x2, y2, z2);
//	}
//	BoundingBox(vec3 min, vec3 max)
//	{
//		this->min = min;
//		this->max = max;
//	}
//	bool contain(Shapes s)
//	{
//		std::vector<glm::vec3> vertices = s.getOBB().getVertices();
//		for (int i = 00; i < vertices.size(); i++)
//		{
//			vec3 vertex = vertices[i];
//			if (!(vertex.x < max.x && vertex.y < max.y && vertex.z < max.z))
//				return false;
//			if (!(vertex.x > min.x && vertex.y > min.y && vertex.z > min.z))
//				return false;
//		}
//		return true;
//	}
//	void draw3D()
//	{/*
//		vec3 dim = max - min;
//		vec3 center = dim / 2.0f;
//		vec3 tmp = vec3(max.x, 0, 0);
//		vec3 edge = tmp - center;
//		float le = glm::length(edge);
//
//		glPushMatrix();
//		{
//			glColor3d(1,0,0.4);
//			glutWireCube(le);
//		}
//		glPopMatrix();
//	}
//	*/
//	}
};


#endif
