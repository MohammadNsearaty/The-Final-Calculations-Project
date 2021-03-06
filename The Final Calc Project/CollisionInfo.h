#pragma once
#ifndef COLLISIONINFO_H
#define COLLISIONINFO_H
#include"glm/glm.hpp"
#include<vector>
using namespace glm;
class CollisionInfo
{
private:
	float depth;
	bool isCollision;
	vec3 collisionPoint;
	vec3 normal;
public:
	std::vector<glm::vec3> points;
	CollisionInfo(){
		isCollision = false;
		depth = 0.0f;
	}
	CollisionInfo(float dist, bool b, vec3 point)
	{
		this->depth= dist;
		this->isCollision = b; 
		this->collisionPoint = point;
	}
	void setDepth(float dist) { this->depth = dist; }
	void setIsCollision(bool b) { this->isCollision = b; }
	void setCollisionPoint(vec3 point) { this->collisionPoint = point; }
	void setNormal(vec3 n) { normal = n; }

	float getDepth() { return this->depth; }
	bool getIsCollision() { return this->isCollision; }
	vec3 getCollisionPoint() { return this->collisionPoint; }
	vec3 getNormal() { return normal; }
};
#endif // !COLLISIONINFO_H

