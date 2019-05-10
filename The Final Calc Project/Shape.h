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

#include "Collision_Data.h"


//Abstruct Class for shapes
class Shapes {
protected:

	Vector3f position;
	Vector3f color;
	Vector3f acc;
	Vector3f speed;
	Vector3f omega;
	Vector3f torque;
	Matrix3f rotation;//the Matrix of rotation R(x)
	Matrix3f iTensor;
	Matrix3f iTensorInv;
	Matrix3f Iinv;
	//Quaternion orientation;//for agregating the R(x) Matrices
	float mass;
	float length[5];
	//the angles of rotaion
	float pitch;
	float yaw;
	float roll;



public:
	Shapes() {
		position.Set(0, 0, 0);
		color.Set(0.0f, 0.0f, 0.0f);
		acc.Set(0, 0, 0);
		mass = 0.0f;
		speed.Set(0, 0, 0);
		omega.Set(0, 0, 0);
		iTensor.Set(3, 3, 0);
		rotation.Set(3, 3, 0);
		rotation[0][0] = rotation[1][1] = rotation[2][2] = 1;
		Iinv.Set(3, 3, 0);
		iTensorInv = iTensor.Inverse();
	}
	Shapes(float Coordinates1, float Coordinates2, float Coordinates3, float Color1, float Color2, float Color3) {
		position.Set(Coordinates1, Coordinates2, Coordinates3);
		color.Set(Color1, Color2, Color3);
		acc.Set(0, 0, 0);
		mass = 0.0f;
		speed.Set(0, 0, 0);
		omega.Set(0, 0, 0);
		iTensor.Set(3, 3, 0);
		rotation[0][0] = rotation[1][1] = rotation[2][2] = 1;
		iTensorInv = iTensor.Inverse();
		Iinv.Set(3, 3, 0);
	}
	void Integrate() {
		position.Set(position.GetX() + speed.GetX(), position.GetY() + speed.GetY(), position.GetZ() + speed.GetZ());
	}
	Vector3f getPostion() {
		return position;
	}
	Vector3f getSpeed() {
		return speed;
	}
	float getMass()
	{
		return mass;
	}
	Vector3f getAcc()
	{
		return acc;
	}
	void setPosition(Vector3f pos)
	{
		position.Set(pos.GetX(), pos.GetY(), pos.GetZ());
	}
	void setSpeed(Vector3f sp)
	{
		speed.Set(sp.GetX(), sp.GetY(), sp.GetZ());
	}
	void setAcc(Vector3f ac)
	{
		acc.Set(ac.GetX(), ac.GetY(), ac.GetZ());
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
		speed.SetX(speed.GetX()*x);
		speed.SetY(speed.GetY() * y);
		speed.SetZ(speed.GetZ() * z);
	}
	//virtual void generateInteriaTensor();
	virtual Collision_Data Collision(Shapes *other) { return Collision_Data(1, true); };
	//TODO:To draw 2D shapes
	virtual void draw_2D(int x, int y) {};

	//TODO:To draw 3D shapes
	virtual void draw_3D() {};

	//TODO:To move the Shapes
	void applyForce(Vector3f force) {

		force.Set(force.GetX() / mass, force.GetY() / mass, force.GetZ() / mass);
		acc.Set(acc.GetX() + force.GetX(), acc.GetY() + force.GetY(), acc.GetZ() + force.GetZ());
		speed.Set(speed.GetX() + acc.GetX(), speed.GetY() + acc.GetY(), speed.GetZ() + acc.GetZ());
		position.Set(position.GetX() + speed.GetX(), position.GetY() + speed.GetY(), position.GetZ() + speed.GetZ());
		acc.Set(0, 0, 0);
	}

	//TODO:To Collision
	virtual void Collision2() {};
};
#endif // !1
