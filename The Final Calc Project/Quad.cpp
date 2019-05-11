#include "Quad.h"

Quad::Quad() {
	length[0] = 1;
	length[1] = 1;
	length[2] = 1;
}

Quad::Quad(float H, float L, float W, float c1, float c2, float c3, float l) {

	position = vec3(H, L, W);
	color = vec3(c1, c2, c3);
	length[0] = 1;
	length[1] = 1;
	length[2] = 1;
}


void Quad::draw_2D(int x, int y) {
	glColor3d(color.x, color.y, color.z);
	glBegin(GL_QUADS);
	{
		glVertex2d(x, y);
		glVertex2d(-x, y);
		glVertex2d(-x, y + 2);
		glVertex2d(x, y + 2);
	}glEnd();

}

void Quad::draw_3D() {


}

float Quad::Move_Shape(float S, float T, float M) {

	//A += S / T;

	return acc.x;
}

void Quad::Collision2() {};