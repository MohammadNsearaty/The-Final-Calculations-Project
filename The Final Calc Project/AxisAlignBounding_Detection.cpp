#include "AxisAlignBounding_Data.h"
AxisAlignBounding::AxisAlignBounding(Vector3f min, Vector3f max) {

	Min_Vector = min;
	Max_Vector = max;

}

Vector3f AxisAlignBounding::GetMaxVectorAxis() {

	return Max_Vector;
}

Vector3f AxisAlignBounding::GetMinVectorAxis() {

	return Min_Vector;
}

Collision_Data AxisAlignBounding::Is_CollisionAxis(AxisAlignBounding other) {
	//Here we Get the max vector point of the down shape and mines it from the min vector point of the up shape
	Vector3f Distance1 = other.GetMinVectorAxis() - Max_Vector;
	//same of the top talk but here if we swap the shapes so the points must swap
	Vector3f Distance2 = Min_Vector - other.GetMaxVectorAxis();

	Vector3f Result = Vector3f(Distance1.Max(Distance2));

	float DistFinal = Result.Max();

	bool Is = false;

	if (DistFinal <= 0)
	{
		Is = true;
	}
	else {
		Is = false;
	}
	Collision_Data col;
	col.setDistance(DistFinal);
	col.setIsCollision(Is);
	return col;
}


