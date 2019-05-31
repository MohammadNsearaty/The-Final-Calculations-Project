#pragma once

#ifndef OBB_H
#define OBB_H

#include<glm/glm.hpp>

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


};

















#endif
