#pragma once
#ifndef LINE_H
#define LINE_H
#include<glm/glm.hpp>
using namespace glm;
class Line {
public:
	glm::vec3 start;
	glm::vec3 end;
public:
	Line()
	{
		start = vec3(0.0f);
		end = vec3(0.0f);
	}
	Line(vec3 start, vec3 end)
	{
		this->start = start;
		this->end = end;
	}
	float length()
	{
		return glm::length(end - start);
	}
	float sqLength()
	{
		float len = glm::length(end - start);
		return len*len;
	}
};
#endif // !LINE_H
#pragma once
