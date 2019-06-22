#pragma once

#ifndef OBB_H
#define OBB_H

#include<glm/glm.hpp>
#include<vector>
#include"Plane.h"
#include"Line.h"
#include"Interval.h"
using namespace glm;

class OBB
{

public:
	vec3 center;
	mat3 u;/*The Rotation Matrix */
	vec3 edges;
public:
	OBB()
	{
		center = vec3(0.0f);
		u = mat3(0.0f);
		u[0][0] = u[1][1] = u[2][2] = 1.0f;
		edges = vec3(0.0f);
	}
	std::vector<vec3> getVertices();
	std::vector<Line> getEdges();
	std::vector<Plane> getPlanes();
	bool pointInOBB(glm::vec3 point);
	vec3 ClosestPointToOBB(vec3 point);
	Interval getInterval(glm::vec3 axis);
	bool Empty()
	{
		if (edges[0] != 0 && edges[1] != 0 && edges[2] != 0)
			return false;
		return true;
	}

};

















#endif
