#pragma once
#ifndef COLLISIONINFO_H
#define COLLISIONINFO_H
#include"glm/glm.hpp"
#include<vector>
using namespace glm;
class CollisionInfo
{
private:
	float dist;
	bool isCollision;
	vec3 collisionPoint;

public:
	CollisionInfo(float dist, bool b, vec3 point)
	{
		this->dist = dist;
		this->isCollision = b; 
		this->collisionPoint = point;
	}
	void setDist(float dist) { this->dist = dist; }
	void setIsCollision(bool b) { this->isCollision = b; }
	void setCollisionPoint(vec3 point) { this->collisionPoint = point; }

	float getDist() { return this->dist; }
	bool getIsCollision() { return this->isCollision; }
	vec3 getCollisionPoint() { return this->collisionPoint; }
};
#endif // !COLLISIONINFO_H

