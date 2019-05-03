#ifndef SHAPE_INCLUDE_H
#define SHAPE_INCLUDE_H

#pragma once
#include <windows.h>		// Header File For Windows
#include <gl.h>			// Header File For The OpenGL32 Library
#include <glu.h>			// Header File For The GLu32 Library
#include <glaux.h>		// Header File For The Glaux Library
#include <cmath>

#include "math3d.h"

#include "Collision_Data.h"


//Abstruct Class for shapes
class Shapes {
protected:

	Vector3f position;
	Vector3f color;
	Vector3f acc;
	Vector3f speed;
	float mass;
	float length[5];



public:
	Shapes() {
		position.Set(0, 0, 0);
		color.Set(0.0f, 0.0f, 0.0f);
		acc.Set(0, 0, 0);
		mass = 0.0f;
		speed.Set(0, 0, 0);

	}
	Shapes(float Coordinates1, float Coordinates2, float Coordinates3, float Color1, float Color2, float Color3) {
		position.Set(Coordinates1, Coordinates2, Coordinates3);
		color.Set(Color1, Color2, Color3);
	}
	void Integrate(float alpha) {
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

	Vector3f getCenter() {

		return position;
	}


};
#endif // !1

/*

float RaduisDistance = Radius + other.Radius;
float CenterDistance = (other.getVec().Length() - Center.Length());
float distance = CenterDistance - RaduisDistance;

if (CenterDistance < RaduisDistance) {

return(Collision_Data(distance, true));
}
else if (CenterDistance == RaduisDistance) {

return(Collision_Data(distance, true));

}
else {
return(Collision_Data(distance, false));
}

*/