
#include "Plane.h"

//#include"Shpere.h"
Plane Plane::Normalized() {
	float magnitude = normal.Length();

	return Plane(normal / magnitude, length / magnitude);

}

Collision_Data Plane::Collision_Shpere_Plane(Shapes* s) {

	float dist = fabs(normal.Dot(s->getPostion()) - length);

	float *len = s->getlength();
	float dist2 = dist - len[0];


	Vector3f pos = s->getPostion();

	if (pos.GetY() - normal.GetY() < 1e-6)
	{
		return Collision_Data(0, true);
	}
	else
	{
		return Collision_Data(fabs(pos.GetY() - normal.GetY()), false);
	}








	//	
	//	//float distanceCentertoPlane = fabs(normal.Dot( len[0]+ length));
	////	float distanceCentertoPlane = fabs((normal.GetX()* s->getPostion().GetX() +  normal.GetY() * s->getPostion().GetY()  + normal.GetZ() * s->getPostion().GetZ() )+  length);
	//	float distanceShperetoplane = (distanceCentertoPlane - len[0]);
	//	if (distanceShperetoplane < 0) {
	//		//Vector3f force2(0, 0.000001, 0);
	//		//s->applyForce(force2);
	//		return Collision_Data(distanceShperetoplane, true);
	//	}
	//	else {
	//	//	Vector3f force2(0, -0.000001, 0);
	//	//	s->applyForce(force2);
	//		return Collision_Data(distanceShperetoplane, false);
	//	}

}
