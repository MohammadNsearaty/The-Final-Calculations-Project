#pragma once
#ifndef AxisAlignBounding_Data_INCLUDE_H
#define AxisAlignBounding_Data_INCLUDE_H

#pragma once

#include"math3d.h"
#include "Collision_Data.h"

class AxisAlignBounding {

private:
	Vector3f Min_Vector;
	Vector3f Max_Vector;

public:
	AxisAlignBounding(Vector3f min, Vector3f max);
	Vector3f GetMinVectorAxis();
	Vector3f GetMaxVectorAxis();

	Collision_Data Is_CollisionAxis(AxisAlignBounding other);


};

#endif 