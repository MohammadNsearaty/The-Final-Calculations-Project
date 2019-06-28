#include "Cube.h"
#include "Shape.h"
#include<glm/matrix.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/quaternion.hpp>
#include<freeglut.h>

using namespace glm;


Cube::Cube() {
	Shapes();
	quadric1 = gluNewQuadric();
	length[0] = 1;
	generateInteriaTensor();
	iTensorInv = glm::inverse(iTensor);
	generateOBB();
	this->setType(1);
	drawType = 1;
}
Cube::Cube(GLUquadric *quadric, float l, float m, float x, float y, float z, float c1, float c2, float c3) {	
	mass = m;
	position= vec3(x, y, z);
	color= vec3(c1, c2, c3);
	quadric1 = quadric;
	length[0] = l;
	calcEdges();
	generateInteriaTensor();
	iTensorInv = glm::inverse(iTensor);
	generateOBB();
	this->setType(1);
	drawType = 1;
}

void Cube::generateOBB()
{
	obb.center = this->position;
	obb.edges = vec3(this->length[0]/2);
}
void Cube::generateInteriaTensor()
{
	float d =  2*(length[0] * length[0]);
	float res1 = mass / 12;
	float res = (res1* d);
	
	iTensor[0][0] = iTensor[1][1] = iTensor[2][2] = res;
//	iTensorInv[0][0] = iTensorInv[1][1] = iTensorInv[2][2] = (1 / res);
}
void Cube::calcEdges()
{
	float d = length[0] / 2;
	float x = position.x, y = position.y, z = position.z;
	edges[0] = vec3(x + d, y + d, z - d);
	edges[1] = vec3(x + d, y + d, z + d);
	edges[2] = vec3(x - d, y + d, z + d);
	edges[3] = vec3(x - d, y + d, z - d);
	edges[4] = vec3(x - d, y - d, z - d);
	edges[5] = vec3(x - d, y - d, z + d);
	edges[6] = vec3(x + d, y - d, z + d);
	edges[7] = vec3(x + d, y - d, z - d);
}
void Cube::drawSolid()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BITS);
	glPushMatrix();
	{
		glTranslated(this->position.x, this->position.y, this->position.z);
		glColor3d(color.x, color.y, color.z);
		glRotated(pitch, 1, 0, 0);
		glRotated(yaw, 0, 1, 0);
		glRotated(roll, 0, 0, 1);
		glutSolidCube(this->length[0]);
	}glPopMatrix();
}
void Cube::draw_3D() {
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BITS);
	glPushMatrix();
	{
		glTranslated(this->position.x, this->position.y, this->position.z);
		glColor3d(color.x, color.y, color.z);
		glRotated(pitch, 1, 0, 0);
		glRotated(yaw, 0, 1, 0);
		glRotated(roll, 0, 0, 1);
		if (drawType == 1)
			glutSolidCube(this->length[0]);
		else
			glutWireCube(this->length[0]);
	}glPopMatrix();
	
}

//TODO:The Virtual Function you must reDefine it in The subclass 
void Cube::draw_2D(int x, int y) { }




//TODO: To draw Cube


Collision_Data Cube::Collision(Shapes* other)
{
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

void Cube::Collision2() {

}