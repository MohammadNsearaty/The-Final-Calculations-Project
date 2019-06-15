

#ifndef PhysicsEngine_H
#define PhysicsEngine_H
#pragma once
#include <vector>
#include "Shape.h"
#include "Plane.h"
#include "CollisionInfo.h"
#include"Interval.h"
using namespace std;

#define EPSILON 1e-6
#define colEpsilon 0.9f
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
	CollisionInfo ShepreAndOBB(Shapes s, OBB obb);

	vec3 J(Shapes s1, Shapes s2, CollisionInfo info);
	void resetCollisionInfo(CollisionInfo *info);
	CollisionInfo ShereVsShpere(Shapes s1, Shapes s2);

	//OBB Vs OBB auxillary methods
	bool ClipToPlane(Plane plane, Line line, vec3* outPoint);
	std::vector<vec3> ClipEdgesToOBB(std::vector<Line> edges, OBB obb);
	float PentrationDepth(OBB o1, OBB o2, vec3 axis, bool* outShouldFlip);
	CollisionInfo ObbVsOBB(OBB o1, OBB o2);

};
CollisionInfo PhysicsEngine::ObbVsOBB(OBB o1, OBB o2)
{
	CollisionInfo result;
	this->resetCollisionInfo(&result);
	vec3 test[15] = {//the o1 axis and the o2 axis
		vec3(o1.u[0][0] , o1.u[0][1] , o1.u[0][2]),
		vec3(o1.u[1][0] , o1.u[1][1] , o1.u[1][2]),
		vec3(o1.u[2][0] , o1.u[2][1] , o1.u[2][2]),
		vec3(o2.u[0][0] , o2.u[0][1] , o2.u[0][2]),
		vec3(o2.u[1][0] , o2.u[1][1] , o2.u[1][2]),
		vec3(o2.u[2][0] , o2.u[2][1] , o2.u[2][2]),
	};
	//the rest of the axis is the cross product between the o1 axis and the o2 axis
	for (int i = 0; i < 3; i++)
	{
		test[6 + i * 3 + 0] = cross(test[i], test[0]);
		test[6 + i * 3 + 1] = cross(test[i], test[1]);
		test[6 + i * 3 + 2] = cross(test[i], test[2]);
	}
	//tmp value for the direction of the collision Normal
	vec3* hitNormal;
	bool shouldFlip;
	//loop through the 15 axis 
	for (int i = 0; i < 15; i++)
	{
		float len = glm::length(test[i]);
		if (len*len < 0.001f)
			continue;

		float depth = this->PentrationDepth(o1, o2, test[i], &shouldFlip);

		if (depth <= 0.0f)
			return result;
		else if (depth < result.getDepth())
		{
			if (shouldFlip)
			{
				test[i] = test[i] * -1.0f;
			}
			result.setDepth(depth);
			hitNormal = &test[i];
		}
	}
	if (hitNormal == 0)
		return result;
	vec3 axis = glm::normalize(*hitNormal);
	std::vector<vec3> c1 = this->ClipEdgesToOBB(o2.getEdges(), o1);
	std::vector<vec3> c2 = this->ClipEdgesToOBB(o1.getEdges(), o2);
	result.points.reserve(c1.size() + c2.size());

	result.points.insert(result.points.end(), c1.begin(), c1.end());
	result.points.insert(result.points.end(), c2.begin(), c2.end());
	Interval interval = o1.getInterval(axis);
	float distance = (interval.getMax() - interval.getMin()) * 0.5f - result.getDepth() * 0.5f;
	vec3 POnPlane = o1.center + axis * distance;
	for (int j = result.points.size() - 1; j >= 0; j--)
	{
		vec3 contact = result.points[j];
		result.points[j] = contact + (axis * dot(axis, POnPlane - contact));
	}
	result.setIsCollision(true);
	result.setNormal(axis);
	return result;

}
float PhysicsEngine::PentrationDepth(OBB o1, OBB o2, vec3 axis, bool* outShouldFlip)
{
	Interval i1 = o1.getInterval(glm::normalize(axis));
	Interval i2 = o2.getInterval(glm::normalize(axis));

	if (!((i2.getMin() <= i1.getMax()) && (i1.getMin() <= i2.getMax()))) {
		return 0.0f; // No penerattion
	}
	float len1 = i1.getMax() - i1.getMin();
	float len2 = i2.getMax() - i2.getMin();

	float min, max;
	if (i1.getMin() > i2.getMin())
		min = i2.getMin();
	else
		min = i1.getMin();
	if (i1.getMax() > i2.getMax())
		max = i1.getMax();
	else
		max = i2.getMax();
	float length = max - min;
	if (outShouldFlip != 0)
	{
		*outShouldFlip = (i2.getMin() < i1.getMin());
	}
	return (len1 + len2) - length;
}

std::vector<vec3> PhysicsEngine::ClipEdgesToOBB(std::vector<Line> edges, OBB obb)
{
	std::vector<vec3> result;
	result.reserve(edges.size());
	vec3 intersection;
	std::vector<Plane> planes = obb.getPlanes();
	for (int i = 0; i < planes.size(); i++)
		for (int j = 0; j < edges.size(); j++)
			if (this->ClipToPlane(planes[i], edges[j], &intersection))
				if (obb.pointInOBB(intersection))
					result.push_back(intersection);
	return result;
}

//check if plane and edge intersects
bool PhysicsEngine::ClipToPlane(Plane plane, Line line, vec3* outPoint)
{
	vec3 ab = line.end - line.start;
	float nAB = dot(plane.getNormal(), ab);
	if (nAB - 0 < 1e-6)//check  if the vectors are orthangular
		return false;
	float nA = dot(plane.getNormal(), line.start);
	float t = (plane.getDistance() - nA) / nAB;
	if (t >= 0.0f && t <= 1.0f)
	{
		if (outPoint != 0)
			*outPoint = line.start + ab * t;
		return true;
	}
	return false;
}
void PhysicsEngine::resetCollisionInfo(CollisionInfo *info)
{
	if (info != 0)
	{
		info->setDepth(0.0f);
		info->setIsCollision(false);
		info->setNormal(vec3(0, 0, 1));
		info->points.clear();
	}
}

vec3 PhysicsEngine::J(Shapes s1, Shapes s2, CollisionInfo info)
{

	mat3 Ia = s1.getOBB().u * (glm::inverse(s1.getITensor())) * (glm::transpose(s1.getOBB().u));
	vec3 omegaA = vec3(0.0f);
	omegaA = glm::inverse(Ia) * s1.getAngularMomentoum();

	mat3 Ib = s2.getOBB().u * (glm::inverse(s2.getITensor()))* (glm::transpose(s2.getOBB().u));
	vec3 omegaB = vec3(0.0f);
	omegaB = glm::inverse(Ib) * s2.getAngularMomentoum();
	vec3 Pa = s1.pointTolocalAxis(info.points[0]);
	vec3 Pb = s2.pointTolocalAxis(info.points[0]);
	vec3 ra = Pa - s1.getPostion();
	vec3 rb = Pb - s2.getPostion();

	vec3 dPa = s1.getSpeed() + glm::cross(omegaA, ra);
	vec3 dPb = s2.getSpeed() + glm::cross(omegaB, rb);

	vec3 n = info.getNormal();

	vec3 Vrel = n * (dPa - dPb);



	float ma = 1 / s1.getMass();
	float mb = 1 / s2.getMass();

	vec3 upTerm = (-1.0f)*(1.0f + colEpsilon) * Vrel;

	vec3 cr1 = glm::cross(ra, n);
	vec3 c1 = n *(Ia * cr1);
	vec3 c2 = n * (Ib* glm::cross(rb, n));


	vec3 cross1 = glm::cross(c1, ra);
	vec3 cross2 = glm::cross(c2, rb);
	vec3 downTerm = ma + mb + cross1 + cross2;


	vec3 j = upTerm / downTerm;

	return j;


}
CollisionInfo PhysicsEngine::ShereVsShpere(Shapes sp1, Shapes sp2)
{
	CollisionInfo res;
	resetCollisionInfo(&res);
	float r = sp1.getlength()[0] + sp1.getlength()[0];
	vec3 d = sp2.getPostion() - sp1.getPostion();
	float l = length(d);
	l = l*l;
	if (l - r*r > 0 || glm::length(d) == 0.0f)
		return res;


	res.setIsCollision(true);
	float dist = glm::abs(length(d) - r) * 0.5f;
	if (dist < 1e-3)
		dist = 0.0f;
	res.setDepth(dist);
	d = normalize(d);
	res.setNormal(d);
	float dpt = sp1.getlength()[0] - dist;
	vec3 point = sp1.getPostion() + d * dpt;
	res.points.push_back(point);

	return res;
}
CollisionInfo PhysicsEngine::ShepreAndOBB(Shapes s, OBB obb)
{
	vec3 resPoint = this->ClosestPointToOBB(s.getPostion(), obb);
	//float dist = this->SqDistPointToOBB(s.getPostion(), obb);
	float dist = glm::dot(resPoint - s.getPostion(), resPoint - s.getPostion());
	float sqRaduis = s.getlength()[0] * s.getlength()[0];
	if (dist - sqRaduis < 1e-6)
	{
		CollisionInfo result(s.getlength()[0], true, resPoint);
		vec3 n = glm::normalize(resPoint - s.getPostion());
		result.setNormal(n);
		result.points.push_back(resPoint);
		return result;
	}
	CollisionInfo wrongRes(-1, false, vec3(0.0f));
	return wrongRes;
}

vec3 PhysicsEngine::ClosestPointToPlane(vec3 point, Plane plane)
{
	plane.Normalized();
	float t = (glm::dot(plane.getNormal(), point) - plane.getDistance());
	return point - t * plane.getNormal();
}

float PhysicsEngine::distPointToPlane(vec3 point, Plane plane)
{
	plane.Normalized();
	return (glm::dot(plane.getNormal(), point) - plane.getDistance());
}
vec3 PhysicsEngine::ClosestPointToOBB(vec3 point, OBB obb)
{
	vec3 d = point - obb.center;
	vec3 res = obb.center;

	for (int i = 0; i < 3; i++)
	{
		/*float dist = glm::dot(d, obb.u[i]);
		if (dist > obb.edges[i])
		dist = obb.edges[i];
		if (dist < -obb.edges[i])
		dist = -obb.edges[i];
		res += dist * obb.u[i];
		*/

		float dist = glm::clamp(glm::dot(d, obb.u[i]), -obb.edges[i], obb.edges[i]);
		res += dist * obb.u[i];
	}
	return res;
}

float PhysicsEngine::SqDistPointToOBB(vec3 point, OBB obb)
{
	vec3 p = ClosestPointToOBB(point, obb);
	return glm::dot(p - point, p - point);
}
int PhysicsEngine::TestOBB(OBB &a, OBB &b)
{
	float ra, rb;
	mat3 R, AbsR;
	// Compute rotation matrix expressing b in a’s coordinate frame
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			R[i][j] = glm::dot(a.u[i], b.u[j]);


	// Compute translation vector t 
	vec3 t = b.center - a.center; // Bring translation into a’s coordinate frame
	t = vec3(glm::dot(t, a.u[0]), glm::dot(t, a.u[1]), glm::dot(t, a.u[2]));
	// Compute common subexpressions. Add in an epsilon term to // counteract arithmetic errors when two edges are parallel and // their cross product is (near) null (see text for details)
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			AbsR[i][j] = glm::abs(R[i][j]) + 1e-6;
	// Test axes L = A0, L = A1, L = A2 
	for (int i = 0; i < 3; i++)
	{
		ra = a.edges[i];
		rb = b.edges[0] * AbsR[i][0] + b.edges[1] * AbsR[i][1] + b.edges[2] * AbsR[i][2];
		if (glm::abs(t[i]) > ra + rb) return 0;
	}
	// Test axes L = B0, L = B1, L = B2
	for (int i = 0; i < 3; i++)
	{
		ra = a.edges[0] * AbsR[0][i] + a.edges[1] * AbsR[1][i] + a.edges[2] * AbsR[2][i]; rb = b.edges[i];
		if (glm::abs(t[0] * R[0][i] + t[1] * R[1][i] + t[2] * R[2][i]) > ra + rb) return 0;
	}
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
