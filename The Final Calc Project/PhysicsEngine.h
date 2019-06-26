

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
#define MAX 1e6
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
	float PentrationDepth(OBB o1, OBB o2,const vec3 axis, bool* outShouldFlip);
	CollisionInfo ObbVsOBB(OBB o1, OBB o2);
	CollisionInfo intersect(Shapes s1, Shapes s2);
	void resolvePentration(Shapes *s1, Shapes *s2, float depth,vec3 normal);
};
void PhysicsEngine::resolvePentration(Shapes *s1, Shapes *s2, float depth,vec3 normal)
{
	float totalMass = s1->getMass() + s2->getMass();
	float scale = depth / totalMass;
	vec3 correction = normal * scale;
	s1->setPosition(s1->getPostion() - correction * (1 / s1->getMass()));
	s2->setPosition(s2->getPostion() + correction * (1 / s2->getMass()));
	s1->obb.center = s1->getPostion();
	s2->obb.center = s2->getPostion();
};

CollisionInfo PhysicsEngine::intersect(Shapes s1, Shapes s2)
{
	if (s1.getType() == 0 && s2.getType() == 0)
		return this->ShereVsShpere(s1, s2);
	else if (s1.getType() == 0 && s2.getType() == 1)
		return this->ShepreAndOBB(s1, s2.getOBB());
	else if (s1.getType() == 1 && s2.getType() == 1)
		return this->ObbVsOBB(s1.getOBB(), s2.getOBB());
}
CollisionInfo PhysicsEngine::ObbVsOBB(OBB o1, OBB o2)
{
	CollisionInfo result;
	this->resetCollisionInfo(&result);
	vec3 test[15] = {//the o1 axis and the o2 axis
		o1.u[0],o1.u[1],o1.u[2],
		o2.u[0],o2.u[1],o2.u[2],
	};
	//the rest of the axis is the cross product between the o1 axis and the o2 axis
	for (int i = 0; i < 3; i++)
	{
		test[6 + i * 3 + 0] = cross(test[i], test[3]);
		test[6 + i * 3 + 1] = cross(test[i], test[4]);
		test[6 + i * 3 + 2] = cross(test[i], test[5]);
	}
	//tmp value for the direction of the collision Normal
	vec3 *hitNormal;
	bool hit = false;
	bool shouldFlip;
	//loop through the 15 axis 
	for (int i = 0; i < 15; i++)
	{
		float len = glm::dot(test[i],test[i]);
		if (len < 0.00001f)
			continue;
		//test[i] = glm::normalize(test[i]);
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
			hit = true;
		}
	}
	if (hitNormal == 0)
		return result;
	/*if (hit == false)
		return result;*/
	vec3 axis = glm::normalize(*hitNormal);
	std::vector<vec3> c1 = this->ClipEdgesToOBB(o2.getEdges(), o1);
	std::vector<vec3> c2 = this->ClipEdgesToOBB(o1.getEdges(), o2);
	if (c1.size() == 0 && c2.size() == 0)
		return result;
	result.points.reserve(c1.size() + c2.size());

	result.points.insert(result.points.end(), c1.begin(), c1.end());
	result.points.insert(result.points.end(), c2.begin(), c2.end());
	//result.setIsCollision(true);
	//result.setNormal(axis);
	//return result;
	Interval interval = o1.getInterval(axis);
	float distance = (interval.getMax() - interval.getMin()) * 0.5f - result.getDepth() * 0.5f;
	vec3 POnPlane = o1.center + axis * distance;
	for (int i = result.points.size() - 1; i >= 0; i--)
	{
		vec3 contact = result.points[i];
		result.points[i] = contact + (axis * dot(axis, POnPlane - contact));
		//delete the duplicated points
		for (int j = result.points.size() - 1; j > i; j--)
		{
			float l = glm::length(result.points[j] - result.points[i]);
			if (l*l < 0.0001f)
			{
				result.points.erase(result.points.begin() + j);
				break;
			}
		}
		
	}
	result.setIsCollision(true);
	result.setNormal(axis);
	return result;

}
float PhysicsEngine::PentrationDepth(OBB o1, OBB o2, const vec3 axis, bool* outShouldFlip)
{
	Interval i1 = o1.getInterval(axis);
	Interval i2 = o2.getInterval(axis);

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
	//result.reserve(edges.size());
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
	if (glm::abs(nAB) < 1e-6)//check  if the vectors are orthangular
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
		info->setDepth(MAX);
		info->setIsCollision(false);
		info->setNormal(vec3(0, 0, 1));
		info->points.clear();
	}
}

vec3 PhysicsEngine::J(Shapes s1, Shapes s2, CollisionInfo info)
{

	mat3 Ia = s1.getOBB().u * (s1.getITensor()) * (glm::transpose(s1.getOBB().u));
	vec3 omegaA = vec3(0.0f);
	omegaA = glm::inverse(Ia) * s1.getAngularMomentoum();

	mat3 Ib = s2.getOBB().u * (s2.getITensor())* (glm::transpose(s2.getOBB().u));
	vec3 omegaB = vec3(0.0f);
	omegaB = glm::inverse(Ib) * s2.getAngularMomentoum();
	//vec3 Pa = s1.pointTolocalAxis(info.points[0]);
	//vec3 Pb = s2.pointTolocalAxis(info.points[0]);
	vec3 ra = info.points[0] - s1.getPostion();
	vec3 rb = info.points[0] - s2.getPostion();

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

	return j*n;


}
CollisionInfo PhysicsEngine::ShereVsShpere(Shapes sp1, Shapes sp2)
{
	CollisionInfo res;
	resetCollisionInfo(&res);
	float r = sp1.getlength()[0] + sp1.getlength()[0];
	vec3 d = sp2.getPostion() - sp1.getPostion();
	float l = dot(d,d);
	if (l - r*r > 0 || l == 0.0f)
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
	Objects[0]->Integrate(0.0f);
	Objects[1]->Integrate(0.0f);

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
