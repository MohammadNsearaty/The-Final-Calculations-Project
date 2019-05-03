#ifndef COLLISION_DATA_INCLUDE_H
#define COLLISION_DATA_INCLUDE_H

#pragma once


class Collision_Data {
protected:
	float Distance;
	bool isCollision;
public:
	Collision_Data();

	Collision_Data(float Dis, bool is) {
		Distance = Dis;
		isCollision = is;
	}
	bool getisCollision() { return isCollision; }

	float getDistance() { return Distance; }

	void setDistance(float dist) { Distance = dist; };

	void setIsCollision(bool IsCol) { isCollision = IsCol; };

};





#endif // !1
