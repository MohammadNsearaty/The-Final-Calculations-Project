

#ifndef PhysicsEngine_H
#define PhysicsEngine_H
#pragma once
#include <vector>
#include "Shape.h"
#include "Plane.h"
using namespace std;

#define EPSILON 1e-6
class PhysicsEngine {
private:
	vector<Shapes*> Objects;
	vector<Plane> planes;

public:
	PhysicsEngine();

	int getLength() { return Objects.size(); }

	Shapes* getElement(int i)
	{
		return Objects[i];
	}
	void addPlane(Plane plane);
	Plane getPlane(int i);
	int GetNumVector();

	Shapes* GetPhysicsObject(unsigned int pos);

	void AddObject(Shapes *obj);

	void Simulate(float alpha);

	void HandlerCollision();

	int TestOBB(OBB &a, OBB &b);

	vec3 ClosestPointToPlane(vec3 point, Plane plane);
	float distPointToPlane(vec3 point, Plane plane);
	vec3 ClosestPointToOBB(vec3 point, OBB obb);
	float SqDistPointToOBB(vec3 point, OBB obb);




};

vec3 PhysicsEngine::ClosestPointToPlane(vec3 point, Plane plane)
{
	plane.Normalized();
	float t = (glm::dot(plane.getNormal(), point) - plane.getLength());
	return point - t * plane.getNormal();
}

float PhysicsEngine::distPointToPlane(vec3 point, Plane plane)
{
	plane.Normalized();
	return (glm::dot(plane.getNormal(), point) - plane.getLength());
}
vec3 PhysicsEngine::ClosestPointToOBB(vec3 point, OBB obb)
{
	vec3 d = point - obb.center;
	vec3 res = obb.center;

	for (int i = 0; i < 3; i++)
	{
		float dist = glm::clamp(glm::dot(d,obb.u[i]),-obb.edges[i],obb.edges[i]);
		res += dist * obb.u[i];
	}
	return res;
}

float PhysicsEngine::SqDistPointToOBB(vec3 point,OBB obb)
{
	vec3 p = ClosestPointToOBB(point, obb);
	return glm::dot(p - point, p - point);
}
int PhysicsEngine::TestOBB(OBB &a, OBB &b)
{
	float ra, rb;
	mat3 R, AbsR;
	// Compute rotation matrix expressing b in a’s coordinate frame
	for (int i = 0; i < 3;i++) 
		for (int j = 0; j < 3;j++)
			R[i][j] = glm::dot(a.u[i], b.u[j]);


	// Compute translation vector t 
    	vec3 t = b.center - a.center; // Bring translation into a’s coordinate frame
		t = vec3(glm::dot(t, a.u[0]), glm::dot(t, a.u[1]), glm::dot(t, a.u[2]));
	// Compute common subexpressions. Add in an epsilon term to // counteract arithmetic errors when two edges are parallel and // their cross product is (near) null (see text for details)
		for (int i = 0; i < 3;i++)
			for (int j = 0; j < 3;j++)
				AbsR[i][j] = glm::abs(R[i][j]) + 1e-6;
	// Test axes L = A0, L = A1, L = A2 
		for (int i = 0; i < 3;i++) 
		{ ra = a.edges[i];
		rb = b.edges[0] * AbsR[i][0] + b.edges[1] * AbsR[i][1] + b.edges[2] * AbsR[i][2];
		if (glm::abs(t[i]) > ra + rb) return 0; }
	// Test axes L = B0, L = B1, L = B2
		for (int i = 0; i < 3;i++) 
		{ ra = a.edges[0] * AbsR[0][i] + a.edges[1] * AbsR[1][i] + a.edges[2] * AbsR[2][i]; rb = b.edges[i];
		if (glm::abs(t[0] * R[0][i] + t[1] * R[1][i] + t[2] * R[2][i]) > ra + rb) return 0; }
	// Test axis L = A0 x B0
		ra = a.edges[1] * AbsR[2][0] + a.edges[2] * AbsR[1][0]; rb = b.edges[1] * AbsR[0][2] + b.edges[2] * AbsR[0][1]; 
		if (glm::abs(t[2] * R[1][0] - t[1] * R[2][0]) > ra + rb) return 0;
	// Test axis L = A0 x B1
		ra = a.edges[1] * AbsR[2][1] + a.edges[2] * AbsR[1][1]; rb = b.edges[0] * AbsR[0][2] + b.edges[2] * AbsR[0][0];
		if (glm::abs(t[2] * R[1][1] - t[1] * R[2][1]) > ra + rb) return 0;
	// Test axis L = A0 x B2 
		ra = a.edges[1] * AbsR[2][2] + a.edges[2] * AbsR[1][2]; rb = b.edges[0] * AbsR[0][1] + b.edges[1] * AbsR[0][0];
		if (glm::abs(t[2] * R[1][2] - t[1] * R[2][2]) > ra + rb) return 0;
	// Test axis L = A1 x B0
		ra = a.edges[0] * AbsR[2][0] + a.edges[2] * AbsR[0][0];
		rb = b.edges[1] * AbsR[1][2] + b.edges[2] * AbsR[1][1];
		if (glm::abs(t[0] * R[2][0] - t[2] * R[0][0]) > ra + rb) return 0;
	// Test axis L = A1 x B1
		ra = a.edges[0] * AbsR[2][1] + a.edges[2] * AbsR[0][1];
		rb = b.edges[0] * AbsR[1][2] + b.edges[2] * AbsR[1][0];
		if (glm::abs(t[0] * R[2][1] - t[2] * R[0][1]) > ra + rb) return 0;
	// Test axis L = A1 x B2
		ra = a.edges[0] * AbsR[2][2] + a.edges[2] * AbsR[0][2];
		rb = b.edges[0] * AbsR[1][1] + b.edges[1] * AbsR[1][0]; 
		if (glm::abs(t[0] * R[2][2] - t[2] * R[0][2]) > ra + rb) return 0;
	// Test axis L = A2 x B0
		ra = a.edges[0] * AbsR[1][0] + a.edges[1] * AbsR[0][0];
		rb = b.edges[1] * AbsR[2][2] + b.edges[2] * AbsR[2][1];
		if (glm::abs(t[1] * R[0][0] - t[0] * R[1][0]) > ra + rb) return 0;
	// Test axis L = A2 x B1 
		ra = a.edges[0] * AbsR[1][1] + a.edges[1] * AbsR[0][1];
		rb = b.edges[0] * AbsR[2][2] + b.edges[2] * AbsR[2][0]; 
		if (glm::abs(t[1] * R[0][1] - t[0] * R[1][1]) > ra + rb) return 0;
	// Test axis L = A2 x B2
		ra = a.edges[0] * AbsR[1][2] + a.edges[1] * AbsR[0][2]; 
		rb = b.edges[0] * AbsR[2][1] + b.edges[1] * AbsR[2][0]; 
		if (glm::abs(t[1] * R[0][2] - t[0] * R[1][2]) > ra + rb) return 0;
	// Since no separating axis was found, the OBBs must be intersecting
		return 1;
}


Plane PhysicsEngine::getPlane(int i)
{
	return planes[i];
}
void PhysicsEngine::addPlane(Plane plane)
{
	planes.push_back(plane);
}
PhysicsEngine::PhysicsEngine() {

}


int PhysicsEngine::GetNumVector() {

	return (unsigned int)Objects.size();
}

Shapes* PhysicsEngine::GetPhysicsObject(unsigned int pos) {
	return Objects[pos];
}

void PhysicsEngine::AddObject(Shapes* obj) {
	Objects.push_back(obj);
}

void PhysicsEngine::Simulate(float alpha) {

	//for (unsigned int i = 0; i < Objects.size(); i++) {
	Objects[0]->Integrate();
	Objects[1]->Integrate();

	//	}
}

void PhysicsEngine::HandlerCollision() {

	//	for (unsigned int i = 0; i < Objects.size(); i++) {

	//		for (unsigned int j = i+1; j < Objects.size(); j++) {

	Collision_Data data = Objects[0]->Collision(Objects[1]);
	if (data.getisCollision()) {
		Objects[0]->reverseSpeed(0.005, -0.005, 0.005);
		Objects[1]->reverseSpeed(-0.005, 0.005, 0.005);
	}
	//	}


	//	}

}


#endif // !PhysicsEngine_H
