#pragma once
#ifndef SHPERE_H
#define SHPERE_H

#include"Shape.h"


class Shpere : public Shapes {

private:
	GLUquadric *quadric1;
	int Longitudes;
	int Latitudes;

public:
	Shpere();
	Shpere(GLUquadric *quadric, float r, float m, float x, float y, float z, float c1, float c2, float c3);

	void Draw_Shpere();

	
	//TODO:The Virtual Function you must reDefine it in The subclass 

	float getRadius() {
		return length[0];
	}
	void setRadius(float R);

	void draw_2D(int x, int y);



	void draw_3D();


	Collision_Data Collision(Shapes* other);
	void Collision2() {
	}

	void generateOBB();




};
#endif // !SHPERE_H

