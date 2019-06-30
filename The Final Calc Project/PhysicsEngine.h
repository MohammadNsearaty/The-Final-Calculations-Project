

#ifndef PhysicsEngine_H
#define PhysicsEngine_H
#pragma once
#include <vector>
#include "Shape.h"
#include "Plane.h"
#include "CollisionInfo.h"
#include"Interval.h"
#include"Ray.h"
using namespace std;

#define EPSILON 1e-6
#define MAX 1e6
class PhysicsEngine {
public:
	vector<Shapes*> Objects;
	vector<Plane> planes;
	vector<Cube> cubeList;
	vector<Shpere> shperList;
	float colEpsilon = 0.99f;
public:
	PhysicsEngine();
	void addCube(Cube c)
	{
		cubeList.push_back(c);
	}
	void addShper(Shpere sp)
	{
		shperList.push_back(sp);
	}
	void drawCubes()
	{
		for (int i = 0; i < cubeList.size(); i++)
			cubeList[i].draw_3D();
	}
	void drawShperes()
	{
		for (int i = 0; i < shperList.size(); i++)
			shperList[i].draw_3D();
	}
	int getLength() { return Objects.size(); }
	Shapes* getElement(int i)
	{
		return Objects[i];
	}
	void addPlane(Plane plane);
	Plane getPlane(int i);
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
	float rayCast(Shpere shpere, Ray ray);
	float rayCast(Cube cube, Ray ray);
	CollisionInfo ShpereVsPlane(Shpere s, Plane p);
	CollisionInfo CubeVsPlane(Cube b, Plane p);
	void checkIntersect();
	void Integrate(float dur);
	void drawObjects();
	void applyJ(vec3 J, vec3 point,int index,int dir,int type);

};
void PhysicsEngine::applyJ(vec3 J,vec3 point, int index1,int dir , int type)
{
	if (type == 0)//Sphere
	{
		//linear
		if (dir == 0)
			shperList[index1].setSpeed(shperList[index1].getSpeed() + (J / shperList[index1].getMass()));
		else
			shperList[index1].setSpeed(shperList[index1].getSpeed() - (J / shperList[index1].getMass()));

		//angular
		vec3 r = point - shperList[index1].getPostion();
		vec3 torq = glm::cross(r, J);
		if (dir == 0)
			shperList[index1].omega += inverse(shperList[index1].getITensor()) * torq;
		else
			shperList[index1].omega -= inverse(shperList[index1].getITensor()) * torq;

	}
	else if (type == 1)
	{
		//linear
		if(dir == 0)
		    cubeList[index1].setSpeed(cubeList[index1].getSpeed() + (J / cubeList[index1].getMass()));
		else
			cubeList[index1].setSpeed(cubeList[index1].getSpeed() - (J / cubeList[index1].getMass()));

		//angular
		vec3 r = point - cubeList[index1].getPostion();
		vec3 torq = glm::cross(r, J);
		if (dir == 0)
			cubeList[index1].angularMo += torq;
		else
			cubeList[index1].angularMo -= torq;
	}
}
void  PhysicsEngine::addPlane(Plane p)
{
	this->planes.push_back(p);
}
float PhysicsEngine::rayCast(Shpere shpere, Ray ray)
{
	vec3 e = shpere.getPostion() - ray.orgin;
	float r = shpere.getlength()[0] * shpere.getlength()[0];
	float len = dot(e, e);
	float a = dot(e, ray.dir);

	float bsq = len - (a*a);
	float f = glm::sqrt(r - bsq);

	if (r - (len - (a*a)) < 0.0f)
		return -1.f;
	else if (len < r)
		return a + f;
	return a - f;
}
float PhysicsEngine::rayCast(Cube cube, Ray ray)
{
	vec3 X = cube.obb.u[0];
	vec3 Y = cube.obb.u[1];
	vec3 Z = cube.obb.u[2];
	vec3 p = cube.obb.center - ray.orgin;
	vec3 f = vec3(dot(X, ray.dir), dot(Y, ray.dir), dot(Z, ray.dir));
	vec3 e = vec3(dot(X, p), dot(Y, p), dot(Z, p));
	float t[6] = { 0, 0, 0, 0, 0, 0 };
	for (int i = 0;i < 3; i++)
	{
		if (f[i] <= 0)
		{//if the ray is parallel to the slab and the ray orgin not insde the slap
			if (-e[i] - cube.obb.edges[i] > 0 || -e[i] + cube.obb.edges[i] < 0)
				return -1;
			f[i] = 0.00001f;
		}
		t[i * 2 + 0] = (e[i] + cube.obb.edges[i]) / f[i];
		t[i * 2 + 1] = (e[i] - cube.obb.edges[i]) / f[i];
		float tmin = glm::max(max(min(t[0], t[1]), min(t[2], t[3])), min(t[4], t[5]));
		float tmax = glm::min(min(max(t[0], t[1]), max(t[2], t[3])), max(t[4], t[5]));
		if (tmax < 0)//the cube is behind the orgin of the ray
			return -1.0f;
		if (tmin > tmax)//no intersect
			return -1.0f;
		if (tmin < 0.0f)//the ray orgin is in the cube
			return tmax;
		return tmin;
	}
}
void PhysicsEngine::resolvePentration(Shapes *s1, Shapes *s2, float depth,vec3 normal)
{
	float totalMass = s1->getMass() + s2->getMass();
	float scale = depth / totalMass ;
	vec3 correction = normal * scale ;
	s1->setPosition(s1->getPostion() + correction * (1 / s1->getMass()));
	s2->setPosition(s2->getPostion() - correction * (1 / s2->getMass()));
	s1->obb.center = s1->getPostion();
	s2->obb.center = s2->getPostion();
};
void PhysicsEngine::checkIntersect()
{
	int num = 1;
	for (int i = 0; i < cubeList.size(); i++)
	{
		for (int j = i+1; j < cubeList.size(); j++)
		{
			CollisionInfo res = ObbVsOBB(cubeList[i].getOBB(), cubeList[j].obb);
			if (res.getIsCollision())
			{
				this->resolvePentration(&cubeList[i], &cubeList[j], res.getDepth(), res.getNormal());

				vector<vec3> allJ;
				for (int p = 0; p < res.points.size(); p++)
				{
					CollisionInfo tmp = CollisionInfo(res);
					vec3 point = vec3(res.points[p]);
					tmp.points.clear();
					tmp.points.push_back(res.points[p]);
					vec3 J = this->J(cubeList[i], cubeList[j], tmp);
					allJ.push_back(J);
					//this->applyJ(J, tmp.points[0], i, 0, 1);
					//this->applyJ(J, tmp.points[0], j, 1, 1);
				}
				for (int p = 0; p < allJ.size(); p++)
				{
					this->applyJ(allJ[p], res.points[p], i, 0, 1);
					this->applyJ(allJ[p], res.points[p], j, 1, 1);
				}
				
			}
		}
		for (int k = 0; k < shperList.size(); k++)
		{
			CollisionInfo res = this->ShepreAndOBB(shperList[k], cubeList[i].obb);
			if (res.getIsCollision())
			{
				vec3 J = this->J(cubeList[i], shperList[k], res);
				for (int z = 0; z < num; z++)
				{
					this->applyJ(J, res.points[0], i, 0, 1);
					this->applyJ(J, res.points[0], k, 1, 0);
				}
				this->resolvePentration(&cubeList[i], &shperList[k], res.getDepth(), res.getNormal());

			}
		}
	}
	for (int i = 0; i < shperList.size(); i++)
	{
		for (int j = i+1; j < shperList.size(); j++)
		{
			CollisionInfo res = this->ShereVsShpere(shperList[i], shperList[j]);
			if (res.getIsCollision())
			{
				this->resolvePentration(&shperList[i], &shperList[j], res.getDepth(), res.getNormal());
				vec3 J = this->J(shperList[i], shperList[j], res);
				this->applyJ(J, res.points[0], i, 0, 0);
				this->applyJ(J, res.points[0], j, 1, 0);
			}
		}
	}
}
void PhysicsEngine::Integrate(float dur)
{
	for (int i = 0; i < cubeList.size(); i++)
		cubeList[i].Integrate(dur);
	for (int i = 0; i < shperList.size(); i++)
		shperList[i].Integrate(dur);
}
void PhysicsEngine::drawObjects()
{
	for (int i = 0; i < cubeList.size(); i++)
		cubeList[i].draw_3D();
	for (int i = 0; i < shperList.size(); i++)
		shperList[i].draw_3D();
}
CollisionInfo PhysicsEngine::intersect(Shapes s1, Shapes s2)
{
	if (s1.getType() == 0 && s2.getType() == 0)
		return this->ShereVsShpere(s1, s2);
	else if (s1.getType() == 0 && s2.getType() == 1)
		return this->ShepreAndOBB(s1, s2.getOBB());
	else if (s1.getType() == 1 && s2.getType() == 1)
		return this->ObbVsOBB(s1.getOBB(), s2.getOBB());
}
CollisionInfo PhysicsEngine::ShpereVsPlane(Shpere s, Plane p)
{
	vec3 point = this->ClosestPointToPlane(s.getPostion(), p);
	vec3 vect = s.getPostion() - point;
	float distSq = dot(vect, vect);
	float raduisSq = s.getlength()[0] * s.getlength()[0];
	if (distSq - raduisSq < 1e-6)
	{
		CollisionInfo res;
		res.points.push_back(point);
		res.setNormal(glm::normalize(p.getNormal()));
		float len = glm::length(vect);
		res.setDepth(s.getlength()[0] - len * 0.5f);
		res.setIsCollision(true);
		return res;
	}
	CollisionInfo res;
	this->resetCollisionInfo(&res);
	return res;
}
CollisionInfo PhysicsEngine::CubeVsPlane(Cube b, Plane p)
{
	OBB obb = b.obb;
	//project the cube vertices onto the plane normal
	float interv = obb.edges[0] * abs(dot(p.getNormal(), obb.u[0])) +
		obb.edges[1] * abs(dot(p.getNormal(), obb.u[1])) +
		obb.edges[2] * abs(dot(p.getNormal(), obb.u[2]));
	float d = dot(p.getNormal(), obb.center);
	float dist = d - p.getDistance();
	if (abs(dist) <= interv)
	{
		CollisionInfo data;
		std::vector<vec3> points;
		std::vector<Line> edges = b.obb.getEdges();
		for (int i = 0; i < 8; i++)
		{
			vec3 resP;
			bool res = this->ClipToPlane(p, edges[i], &resP);
			if (res)
				points.push_back(resP);
		}
		data.setIsCollision(true);
		data.setNormal(glm::normalize(p.getNormal()));
		data.points = points;
		data.setDepth(0.0f);
		return data;
	}
	CollisionInfo res;
	this->resetCollisionInfo(&res);
	return res;
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
	result.setIsCollision(true);
	result.setNormal(axis);
	return result;
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
		*outShouldFlip = (i1.getMin() < i2.getMin());
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

	mat3 Ia = s1.getOBB().u * (glm::inverse(s1.getITensor())) * (glm::transpose(s1.getOBB().u));
	mat3 Ib = s2.getOBB().u * (glm::inverse(s2.getITensor()))* (glm::transpose(s2.getOBB().u));
	vec3 omegaA = Ia * s1.angularMo;
	vec3 omegaB = Ib * s2.angularMo;
	vec3 ra = info.points[0] - s1.getPostion();
	vec3 rb = info.points[0] - s2.getPostion();


	vec3 dPa = s1.getSpeed() + glm::cross(s1.omega, ra);
	vec3 dPb = s2.getSpeed() + glm::cross(s2.omega, rb);

	vec3 n = normalize(info.getNormal());

	float Vrel = glm::dot(n,  (dPa - dPb));



	float ma = 1 / s1.getMass();
	float mb = 1 / s2.getMass();

	float upTerm = (-1.0f)*(1.0f + colEpsilon) * Vrel;

	vec3 c1 = (Ia* glm::cross(ra, n));
	vec3 c2 = (Ib* glm::cross(rb, n));


	vec3 cross1 = glm::cross(c1, ra);
	vec3 cross2 = glm::cross(c2, rb);
	float downTerm = ma + mb + dot(n,cross1 + cross2);


	float j = upTerm / downTerm;

	return j*n;
}
CollisionInfo PhysicsEngine::ShereVsShpere(Shapes sp1, Shapes sp2)
{
	CollisionInfo res;
	resetCollisionInfo(&res);
	float r = sp1.getlength()[0] + sp2.getlength()[0];
	vec3 d = sp2.getPostion() - sp1.getPostion();
	float l = dot(d,d);
	if (l - r*r > 0 || l == 0)
		return res;


	res.setIsCollision(true);
	float dist = glm::abs(length(d) - r) * 0.5f;
	//if (dist < 1e-3)
		//dist = 0.0f;
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
	CollisionInfo result;
	this->resetCollisionInfo(&result);
	vec3 resPoint = this->ClosestPointToOBB(s.getPostion(), obb);
	float dist = glm::dot(resPoint - s.getPostion(), resPoint - s.getPostion());
	float sqRaduis = s.getlength()[0] * s.getlength()[0];
	if (dist > sqRaduis)
	{
		return result;
	}
	vec3 normal;
	if (dist < 1e-6)
	{
		float msq = dot(resPoint - obb.center, resPoint - obb.center);
		if (msq <= 1e-6)
			return result;
		normal = normalize(resPoint - obb.center);
	}
	else
		normal = normalize(s.getPostion() - resPoint);
	vec3 point = s.getPostion() - normal*s.getlength()[0];
	float ds = dot(resPoint - point, resPoint - point);
	result.setIsCollision(true);
	result.points.push_back(resPoint);
	result.setNormal(normal);
	float depth = glm::length(resPoint - point);
	result.setDepth(depth);
	return result;
		/*vec3 n = glm::normalize(resPoint - s.getPostion());
		result.setNormal(n);
		result.points.push_back(resPoint);
		result.setIsCollision(true);
		float depth = (s.getlength()[0] - glm::length(resPoint - s.getPostion())) / 2.0f;
		result.setDepth(depth);
		return result;
	}
	CollisionInfo wrongRes(-1, false, vec3(0.0f));
	return wrongRes;*/
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
PhysicsEngine::PhysicsEngine() {}
#endif // !PhysicsEngine_H
