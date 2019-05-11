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
using namespace glm;

//Abstruct Class for shapes
class Shapes {
protected:

	vec3 position;
	vec3 color;
	vec3 acc;
	vec3 speed;
	vec3 angularMo;
	vec3 torque;
	mat3 rotation;//the Matrix of rotation R(x)
	mat3x3 iTensor;
	mat3 iTensorInv;
	mat3 I;
	//Quaternion orientation;//for agregating the R(x) Matrices
	float mass;
	float length[5];
	//the angles of rotaion
	float pitch;
	float yaw;
	float roll;



public:
	Shapes() {
		position = vec3(0.0f);
		color = vec3(0.0f);
		acc = vec3(0.0f);
		speed = vec3(0.0f);
		angularMo = vec3(0.0f);
		torque = vec3(0.0f);
		rotation = mat3(0.0f);
		rotation[0][0] = rotation[1][1] = rotation[2][2] = 1.0f;
		iTensor = mat3(0.0f);
		iTensorInv = mat3(0.0f);
		I = mat3(0.0f);
		mass = 0.0f;
		pitch = yaw = roll = 0.0f;
		}
	Shapes(float Coordinates1, float Coordinates2, float Coordinates3, float Color1, float Color2, float Color3) {
		position = vec3(Coordinates1, Coordinates2, Coordinates3);
		color = vec3(Color1, Color2, Color3);
		acc = vec3(0, 0, 0);
		mass = 0.0f;
		speed = vec3(0, 0, 0);
		angularMo = vec3(0, 0, 0);
		rotation[0][0] = rotation[1][1] = rotation[2][2] = 1;
		iTensor = mat3(0.0f);
		iTensorInv = mat3(0.0f);
		I = mat3(0.0f);
		mass = 0.0f;
		pitch = yaw = roll = 0.0f;
	}
	void Integrate() {
		position = position + speed;
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

	//TODO:To move the Shapes
	void applyForce(vec3 force) {
		force = force / mass;
		acc += force;
		speed += acc;
		acc = vec3(0.0f);
	}
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
	//TODO:To Collision
	virtual void Collision2() {};
};
#endif // !1
