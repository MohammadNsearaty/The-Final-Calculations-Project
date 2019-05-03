#include "Cube.h"
#include "Shape.h"
Cube::Cube() {
	Shapes();
	quadric1 = gluNewQuadric();
	length[0] = 1;
}
Cube::Cube(GLUquadric *quadric, float l, float m, float x, float y, float z, float c1, float c2, float c3) {
	mass = m;
	position.Set(x, y, z);
	color.Set(c1, c2, c3);
	quadric1 = quadric;
	length[0] = l;
	calcEdges();
}

void Cube::calcEdges()
{
	float d = length[0] / 2;
	float x = position.GetX(), y = position.GetY(), z = position.GetZ();
	edges[0].Set((x + d), (y + d), (z - d));
	edges[1].Set(x + d, y + d, z + d);
	edges[2].Set(x - d, y + d, z + d);
	edges[3].Set(x - d, y + d, z - d);
	edges[4].Set(x - d, y - d, z - d);
	edges[5].Set(x - d, y - d, z + d);
	edges[6].Set(x + d, y - d, z + d);
	edges[7].Set(x + d, y - d, z - d);
}

void Cube::draw_3D() {
	glColor3d(color.GetX(), color.GetY(), color.GetZ());
	glPushMatrix();
	{

		calcEdges();
		//upper face
		glBegin(GL_QUADS);
		glVertex3f(edges[0].GetX(), edges[0].GetY(), edges[0].GetZ());
		glVertex3f(edges[1].GetX(), edges[1].GetY(), edges[1].GetZ());
		glVertex3f(edges[2].GetX(), edges[2].GetY(), edges[2].GetZ());
		glVertex3f(edges[3].GetX(), edges[3].GetY(), edges[3].GetZ());
		glEnd();
		//right face
		glBegin(GL_QUADS);
		glVertex3f(edges[0].GetX(), edges[0].GetY(), edges[0].GetZ());
		glVertex3f(edges[1].GetX(), edges[1].GetY(), edges[1].GetZ());
		glVertex3f(edges[6].GetX(), edges[6].GetY(), edges[6].GetZ());
		glVertex3f(edges[7].GetX(), edges[7].GetY(), edges[7].GetZ());
		glEnd();
		//back face
		glBegin(GL_QUADS);
		glVertex3f(edges[0].GetX(), edges[0].GetY(), edges[0].GetZ());
		glVertex3f(edges[3].GetX(), edges[3].GetY(), edges[3].GetZ());
		glVertex3f(edges[4].GetX(), edges[4].GetY(), edges[4].GetZ());
		glVertex3f(edges[7].GetX(), edges[7].GetY(), edges[7].GetZ());
		glEnd();
		//left face
		glBegin(GL_QUADS);
		glVertex3f(edges[3].GetX(), edges[3].GetY(), edges[3].GetZ());
		glVertex3f(edges[2].GetX(), edges[2].GetY(), edges[2].GetZ());
		glVertex3f(edges[5].GetX(), edges[5].GetY(), edges[5].GetZ());
		glVertex3f(edges[4].GetX(), edges[4].GetY(), edges[4].GetZ());
		glEnd();
		//bottom face
		glBegin(GL_QUADS);
		glVertex3f(edges[4].GetX(), edges[4].GetY(), edges[4].GetZ());
		glVertex3f(edges[5].GetX(), edges[5].GetY(), edges[5].GetZ());
		glVertex3f(edges[2].GetX(), edges[2].GetY(), edges[2].GetZ());
		glVertex3f(edges[3].GetX(), edges[3].GetY(), edges[3].GetZ());
		glEnd();
		//front face
		glBegin(GL_QUADS);
		glVertex3f(edges[6].GetX(), edges[6].GetY(), edges[6].GetZ());
		glVertex3f(edges[1].GetX(), edges[1].GetY(), edges[1].GetZ());
		glVertex3f(edges[2].GetX(), edges[2].GetY(), edges[2].GetZ());
		glVertex3f(edges[5].GetX(), edges[5].GetY(), edges[5].GetZ());
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

	float x1 = position.GetX(), y1 = position.GetY(), z1 = position.GetZ();
	float x2 = other[0].getPostion().GetX();
	float y2 = other[0].getPostion().GetY();
	float z2 = other[0].getPostion().GetZ();


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