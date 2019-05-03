#pragma once
#ifndef QUAD_H
#define QUAD_H

#include "Shape.h"



class Quad : public Shapes {
public:

	Quad();
	Quad(float H, float L, float W, float C1, float C2, float C3, float l);




	//TODO:The Virtual Function you must reDefine it in The subclass 

	void draw_2D(int x, int y);

	void draw_3D();
	float Move_Shape(float S, float T, float M);
	void Collision2();




};



#endif