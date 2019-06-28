#pragma once
#ifndef RAY_H
#define RAY_H
#include<glm/glm.hpp>
using namespace glm;
class Ray
{
public:
	vec3 orgin;
	vec3 dir;

	inline Ray() : dir(0.0f, 0.0f, 1.0f) {}
	inline Ray(const vec3& o, const vec3& d) : orgin(o), dir(glm::normalize(d)) 
	{ NormalizeDirection(); }
	inline void NormalizeDirection() 
	{ normalize(dir); }

	Ray FromPoints(const vec3& from, const vec3& to) 
	{ return Ray(from, normalize(to - from)); }
};


#endif
