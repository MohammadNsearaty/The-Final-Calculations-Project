
#include "Plane.h"

//#include"Shpere.h"
Plane Plane::Normalized() {
	float magnitude = glm::length(normal);

	return Plane(normal / magnitude, length / magnitude);

}

Collision_Data Plane::Collision_Shpere_Plane(Shapes* s) {

	float dist = fabs(glm::dot(normal, (s->getPostion()) - length));          
	float *len = s->getlength();
	float dist2 = dist - len[0];


	vec3 pos = s->getPostion();

	if (pos.y - normal.y < 1e-6)
	{
		return Collision_Data(0, true);
	}
	else
	{
		return Collision_Data(fabs(pos.y - normal.y), false);
	}








	//	
	//	//float distanceCentertoPlane = fabs(normal.Dot( len[0]+ length));
	////	float distanceCentertoPlane = fabs((normal.GetX()* s->getPostion().GetX() +  normal.GetY() * s->getPostion().GetY()  + normal.GetZ() * s->getPostion().GetZ() )+  length);
	//	float distanceShperetoplane = (distanceCentertoPlane - len[0]);
	//	if (distanceShperetoplane < 0) {
	//		//vec3 force2(0, 0.000001, 0);
	//		//s->applyForce(force2);
	//		return Collision_Data(distanceShperetoplane, true);
	//	}
	//	else {
	//	//	vec3 force2(0, -0.000001, 0);
	//	//	s->applyForce(force2);
	//		return Collision_Data(distanceShperetoplane, false);
	//	}

}
