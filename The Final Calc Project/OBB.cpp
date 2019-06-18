#include<vector>
#include<glm/glm.hpp>
#include"OBB.h"
using namespace glm;
std::vector<vec3> OBB::getVertices()
{
	std::vector<vec3> vertices;
	vertices.resize(8);

	glm::vec3 center = this->center;
	glm::vec3 extents = this->edges;
	glm::vec3 A[] = { u[0],u[1],u[2] };

	vertices[0] = center + A[0] * extents[0] + A[1] * extents[1] + A[2] * extents[2];
	vertices[1] = center - A[0] * extents[0] + A[1] * extents[1] + A[2] * extents[2];
	vertices[2] = center + A[0] * extents[0] - A[1] * extents[1] + A[2] * extents[2];
	vertices[3] = center + A[0] * extents[0] + A[1] * extents[1] - A[2] * extents[2];
	vertices[4] = center - A[0] * extents[0] - A[1] * extents[1] - A[2] * extents[2];
	vertices[5] = center + A[0] * extents[0] - A[1] * extents[1] - A[2] * extents[2];
	vertices[6] = center - A[0] * extents[0] + A[1] * extents[1] - A[2] * extents[2];
	vertices[7] = center - A[0] * extents[0] - A[1] * extents[1] + A[2] * extents[2];

	return vertices;
}


std::vector<Line> OBB::getEdges()
{
	std::vector<Line> edges;
	edges.resize(12);
	std::vector<vec3> vertices = this->getVertices();
	int index[][2] = { // Indices of edge-vertices 
		{ 6,1 },{ 6,3 },{ 6,4 },{ 2,7 },{ 2,5 },{ 2,0 },
		{ 0,1 },{ 0,3 },{ 7,1 },{ 7,4 },{ 4,5 },{ 5,3 } };
	for (int j = 0; j < 12; ++j)
		edges.push_back(Line(vertices[index[j][0]], vertices[index[j][1]]));
	return edges;
}



std::vector<Plane> OBB::getPlanes()
{
	glm::vec3 c = this->center;
	glm::vec3 e = this->edges;
	glm::vec3 a[] = {           // OBB Axis 
		glm::vec3(u[0][0], u[0][1], u[0][2]),
		glm::vec3(u[1][0], u[1][1], u[1][2]),
		glm::vec3(u[2][0], u[2][1], u[2][2]),
	};
	std::vector<Plane> planes;
	planes.resize(6);
	planes[0] = Plane(a[0], dot(a[0], (c + a[0] * e.x)));
	planes[1] = Plane(a[0] * -1.0f, dot(a[0], (c - a[0] * e.x)));
	planes[2] = Plane(a[1], dot(a[1], (c + a[1] * e.y)));
	planes[3] = Plane(a[1] * -1.0f, dot(a[1], (c - a[1] * e.y)));
	planes[4] = Plane(a[2], dot(a[2], (c + a[2] * e.z)));
	planes[5] = Plane(a[2] * -1.0f, dot(a[2], (c - a[2] * e.z)));
	return planes;
}


bool OBB::pointInOBB(vec3 point)
{
	vec3 dir = point - this->center;
	for (int i = 0; i < 3; i++)
	{
		vec3 axis(u[i][0], u[i][1], u[i][2]);
		float dist = dot(dir, axis);
		if (dist > edges[i])
			return false;
		if (dist < -edges[i])
			return false;
	}
	return true;
}
Interval OBB::getInterval(vec3 axis)
{
	std::vector<vec3> vertices = this->getVertices();
	Interval result;
	result.setMin(dot(axis, vertices[0]));
	result.setMax(dot(axis, vertices[0]));
	for (int i = 0; i < 8; i++)
	{
		float projection = dot(axis, vertices[i]);
		result.min = (projection < result.min) ? projection : result.min;      
		result.max = (projection > result.max) ? projection : result.max;
	/*	if (result.getMin() > projection)
			result.setMin(projection);
		if (result.getMax() < projection)
			result.setMax(projection);
			*/
	}
	return result;
}