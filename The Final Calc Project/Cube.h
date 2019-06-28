#pragma once
#pragma once
#ifndef CUBE_H
#define CUBE_H

#include"Shape.h"
#include<glm/gtx/quaternion.hpp>

class Cube : public Shapes {

private:
	GLUquadric *quadric1 = gluNewQuadric();
	vec3 edges[8];
public:
	Cube();
	Cube(GLUquadric *quadric, float l, float m, float x, float y, float z, float c1, float c2, float c3);
	Cube(vec3 pos, float mass, float l, vec3 color) {
		this->position = pos;
		this->mass = mass;
		this->length[0] = l;
		this->color = color;
		generateInteriaTensor();
		iTensorInv = glm::inverse(iTensor);
		generateOBB();
		this->setType(1);
	}
	//TODO:The Virtual Function you must reDefine it in The subclass 
	void draw_2D(int x, int y);
	void draw_3D();
	void drawSolid();
	void calcEdges();
	Collision_Data Collision(Shapes* other);
	void testRotate(vec3 force, vec3 point);
	void generateInteriaTensor();

	void Collision2();

	void generateOBB();


};
#endif // !CUBE_H

