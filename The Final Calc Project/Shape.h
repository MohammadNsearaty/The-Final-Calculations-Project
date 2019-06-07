#ifndef SHAPE_INCLUDE_H
#define SHAPE_INCLUDE_H

#pragma once
#include <windows.h>		// Header File For Windows
#include <gl.h>			// Header File For The OpenGL32 Library
#include <glu.h>			// Header File For The GLu32 Library
#include <glaux.h>		// Header File For The Glaux Library
#include <cmath>
#include<iostream>
#include "math3d.h"
#include<glm/glm.hpp>
#include "Collision_Data.h"
#include<glm/gtx/quaternion.hpp>
#include"OBB.h"
using namespace glm;

//Abstruct Class for shapes
class Shapes {
protected:
	vec3 position;
	vec3 color;
	vec3 acc;
	vec3 speed;
	vec3 angularMo;
	//mat3 rotation;//the Matrix of rotation R(x)
	//constant
	mat3x3 iTensor;
	mat3 iTensorInv;
	//Quaternion orientation;//for agregating the R(x) Matrices
	float mass;
	float length[5];
	//the angles of rotaion
	float pitch;
	float yaw;
	float roll;
	//The Oriented Bounding Box
	OBB obb;
	//my variable
	int type;
	//try this 
	glm::fquat quat;

public:
	Shapes() {
		position = vec3(0.0f);
		color = vec3(0.0f);
		acc = vec3(0.0f);
		speed = vec3(0.0f);
		angularMo = vec3(0.0f);
		obb = OBB();
		//rotation = mat3(0.0f);
		//rotation[0][0] = rotation[1][1] = rotation[2][2] = 1.0f;
		iTensor = mat3(0.0f);
		iTensorInv = mat3(0.0f);
		mass = 0.0f;
		pitch = yaw = roll = 0.0f;
		quat = fquat(0, vec3(0, 1, 0));
	//	obb.u = mat3(0.0f);
		//obb.u[0][0] = obb.u[1][1] = obb.u[2][2] = 1.0f;
	}
	Shapes(float Coordinates1, float Coordinates2, float Coordinates3, float Color1, float Color2, float Color3) {
		position = vec3(Coordinates1, Coordinates2, Coordinates3);
		color = vec3(Color1, Color2, Color3);
		acc = vec3(0, 0, 0);
		mass = 0.0f;
		speed = vec3(0, 0, 0);
		angularMo = vec3(0, 0, 0);
		quat = fquat(0, vec3(0, 1, 0));
		//rotation[0][0] = rotation[1][1] = rotation[2][2] = 1;
		iTensor = mat3(0.0f);
		iTensorInv = mat3(0.0f);
		mass = 0.0f;
		pitch = yaw = roll = 0.0f;
	}
	//generate the OBB for each body
	virtual void generateOBB() {};
	//
	OBB getOBB() { return obb; }
	mat3 getITensor() { return iTensor; }
	vec3 getAngularMomentoum() { return angularMo; }
	virtual void generateInteriaTensor() {};

	//TODO:To move the Shapes
	void applyForce(vec3 force,vec3 point) {
		force = force / mass;
		acc += force;
		speed += acc;
		vec3 res = pointTolocalAxis(point);
		vec3 torque = glm::cross(res, force);
		angularMo += torque;
		acc = vec3(0.0f);
	}
	void setType(int t)
	{
		this->type = t;
	}
	int getType()
	{
		return this->type;
	}
	//void simulateRotation(vec3 point, vec3 force) {	}
	void Integrate() {

		//Linear Moves
		position = position + speed;
		obb.center = position;
		//Angular Moves
		mat3 I = obb.u * iTensor * (glm::transpose(obb.u));
		vec3 omega = vec3(0.0f);
		omega = glm::inverse(I) * angularMo;

		//glm::fquat qOmega = fquat(0, omega);

		//quat += (1.0f / 2.0f) * (qOmega * quat);
		mat3 mat = star(omega);
		obb.u += mat * obb.u;

		//obb.u = glm::toMat3(quat);
		pitch = glm::degrees(atan2(obb.u[1][2], obb.u[2][2]));
		yaw = glm::degrees(atan2(-obb.u[2][0], sqrt((obb.u[1][2] * obb.u[1][2]) + (obb.u[2][2] * obb.u[2][2]))));
		roll = glm::degrees(atan2(obb.u[1][0], obb.u[0][0]));

	}
	vec3 getPostion() {
		return position;
	}
	vec3 getSpeed() {
		return speed;
	}
	float getMass()
	{
		return mass;
	}
	vec3 getAcc()
	{
		return acc;
	}
	void setPosition(vec3 pos)
	{
		position = pos;
	}
	void setSpeed(vec3 sp)
	{
		speed = sp;
	}
	void setAcc(vec3 ac)
	{
		acc = ac;
	}
	void setMass(float m)
	{
		mass = m;
	}
	float* getlength()
	{
		return length;
	}
	void reverseSpeed(int x, int y, int z)
	{
		speed.x*=x;
		speed.y *= y;
		speed.z *= speed.z;
	}

	float getPitch() { return pitch; }
	float getYaw() { return yaw; }
	float getRoll() { return roll; }

	void setPitch(float p) { pitch = p; }
	void setYaw(float y) { yaw = y; }
	void setRoll(float r) { roll = r; }
	//virtual void generateInteriaTensor();
	virtual Collision_Data Collision(Shapes *other) { return Collision_Data(1, true); };
	//TODO:To draw 2D shapes
	virtual void draw_2D(int x, int y) {};

	//TODO:To draw 3D shapes
	virtual void draw_3D() {};

	
	mat3 star(vec3 v)
	{
		mat3 matrix = mat3(0.0f);
		matrix[0][1] = -v.z;
		matrix[0][2] = v.y;
		matrix[1][0] = v.z;
		matrix[1][2] = -v.x;
		matrix[2][0] = -v.y;
		matrix[2][1] = v.x;
		return matrix;
	}
	vec3 pointTolocalAxis(vec3 point)
	{
		vec3 res = glm::inverse(obb.u) * point;
		return res;
	}
   void toEulerAngle(fquat q)
	{
		// roll (x-axis rotation)
		double sinr_cosp = +2.0 * (q.w * q.x + q.y * q.z);
		double cosr_cosp = +1.0 - 2.0 * (q.x * q.x + q.y * q.y);
		roll = atan2(sinr_cosp, cosr_cosp);

		// pitch (y-axis rotation)
		double sinp = +2.0 * (q.w * q.y - q.z * q.x);
		if (fabs(sinp) >= 1)
			pitch = copysign(3.14f / 2, sinp); // use 90 degrees if out of range
		else
			pitch = asin(sinp);

		// yaw (z-axis rotation)
		double siny_cosp = +2.0 * (q.w * q.z + q.x * q.y);
		double cosy_cosp = +1.0 - 2.0 * (q.y * q.y + q.z * q.z);
		yaw = atan2(siny_cosp, cosy_cosp);
	}
	//TODO:To Collision
	virtual void Collision2() {};
};
#endif // !1
