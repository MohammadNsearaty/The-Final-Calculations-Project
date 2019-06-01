#include "Cube.h"
#include "Shape.h"
#include<glm/matrix.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/quaternion.hpp>

using namespace glm;


Cube::Cube() {
	Shapes();
	quadric1 = gluNewQuadric();
	length[0] = 1;
	generateInteriaTensor();
	iTensorInv = glm::inverse(iTensor);
	generateOBB();
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

void Cube::draw_3D() {
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BITS);
	glColor3d(color.x, color.y, color.z);
	glPushMatrix();
	{

		calcEdges();
		//upper face
		glColor3d(color.x,color.y,color.z);
		glBegin(GL_QUADS);
		glVertex3f(edges[0].x, edges[0].y, edges[0].z);
		glVertex3f(edges[1].x, edges[1].y, edges[1].z);
		glVertex3f(edges[2].x, edges[2].y, edges[2].z);
		glVertex3f(edges[3].x, edges[3].y, edges[3].z);
		glEnd();
		//right face
		//glColor3d(0, 1, 0);
		glBegin(GL_QUADS);
		glVertex3f(edges[0].x, edges[0].y, edges[0].z);
		glVertex3f(edges[1].x, edges[1].y, edges[1].z);
		glVertex3f(edges[6].x, edges[6].y, edges[6].z);
		glVertex3f(edges[7].x, edges[7].y, edges[7].z);
		glEnd();
		//back face
		//glColor3d(color.x, color.y, color.z);
		glBegin(GL_QUADS);
		glVertex3f(edges[0].x, edges[0].y, edges[0].z);
		glVertex3f(edges[3].x, edges[3].y, edges[3].z);
		glVertex3f(edges[4].x, edges[4].y, edges[4].z);
		glVertex3f(edges[7].x, edges[7].y, edges[7].z);
		glEnd();
		//left face
		//glColor3d(0, 0, 1);
		glBegin(GL_QUADS);
		glVertex3f(edges[3].x, edges[3].y, edges[3].z);
		glVertex3f(edges[2].x, edges[2].y, edges[2].z);
		glVertex3f(edges[5].x, edges[5].y, edges[5].z);
		glVertex3f(edges[4].x, edges[4].y, edges[4].z);
		glEnd();
		//bottom face
		//glColor3d(0.35,0.7,0.9);

		glBegin(GL_QUADS);
		glVertex3f(edges[4].x, edges[4].y, edges[4].z);
		glVertex3f(edges[5].x, edges[5].y, edges[5].z);
		glVertex3f(edges[2].x, edges[2].y, edges[2].z);
		glVertex3f(edges[3].x, edges[3].y, edges[3].z);
		glEnd();
		//glColor3d(0.7, 0.1, 0.4);
		//front face
		glBegin(GL_QUADS);
		glVertex3f(edges[6].x, edges[6].y, edges[6].z);
		glVertex3f(edges[1].x, edges[1].y, edges[1].z);
		glVertex3f(edges[2].x, edges[2].y, edges[2].z);
		glVertex3f(edges[5].x, edges[5].y, edges[5].z);
		glEnd();


	}
	glPopMatrix();
	glColor3d(1, 1, 1);

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