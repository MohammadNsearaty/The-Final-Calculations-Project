#include"Shpere.h"
#include<freeglut.h>

Shpere::Shpere() {
	length[0] = 4;
	Longitudes = 20;
	Latitudes = 20;
	quadric1 = gluNewQuadric();
	generateOBB();
	generateInteriaTensor();
	this->setType(0);
}
Shpere::Shpere(GLUquadric *quadric, float r, float m, float x, float y, float z, float c1, float c2, float c3) {
	Longitudes = 32;
	Latitudes = 32;
	mass = m;
	position = vec3(x, y, z);
	color = vec3(c1, c2, c3);
	length[0] = r;
	quadric1 = quadric;
	generateOBB();
	generateInteriaTensor();
	this->setType(0);
}

void Shpere::generateOBB()
{
	obb.center = this->position;
	obb.edges = vec3(length[0]);
	
}
void Shpere::generateInteriaTensor()
{
	float res = (2.0f / 5.0f) * this->mass * (this->length[0]/2) * (this->length[0]/2);
	this->iTensor[0][0] = this->iTensor[1][1] = this->iTensor[2][2] = res;
}
void Shpere::setRadius(float R)
{
	length[0] = R;
}

void Shpere::draw_2D(int x, int y) { }

void Shpere::draw_3D() {
	glPushMatrix();
	{
		glColor3d(color.x, color.y, color.z);
		glTranslatef(position.x, position.y, position.z);

		glRotated(pitch, 1, 0, 0);
		glRotated(yaw, 0, 1, 0);
		glRotated(roll, 0, 0, 1);
		if (drawType == 1)
			glutSolidSphere(length[0], 32, 32);
		else
			glutWireSphere(length[0], 32, 32);
		glColor3d(1, 1, 1);

	}
	glPopMatrix();
}



Collision_Data Shpere::Collision(Shapes* other) {

	float l1 = length[0] / 2;
	float l2 = other[0].getlength()[0];
	float l = l1 + l2;

	float x1 = position.x, y1 = position.y, z1 = position.z;
	float x2 = other[0].getPostion().x;
	float y2 = other[0].getPostion().y;
	float z2 = other[0].getPostion().z;


	float dist = sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1) + (z2 - z1)*(z2 - z1));


	if (dist < l)
	{
		return Collision_Data(dist, true);
	}
	else
	{
		return Collision_Data(dist, false);
	}

}
