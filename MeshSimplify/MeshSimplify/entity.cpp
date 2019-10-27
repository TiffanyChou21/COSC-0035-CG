#include "entity.h"
#include "function.h"
#include <iostream>
using namespace std;

Edge* Vertex::connectTo(Vertex * v)
{
	for (Edge* e : edgeList)
		if (e->findAnotherVertex(this) == v)
			return e;
	return nullptr;
}

void Vertex::combineTo(Vertex * v)
{
	changeTo = v;
	v->matrix += this->matrix;
	for (Edge* e : edgeList)
	{
		//Vertex* p = e->findAnotherVertex(this);
		//if (v->connectTo(p)) {
		//	e->v1 = nullptr;
		//	continue;
		//}
		//if (e->findAnotherVertex(this) == v) {
		//	e->v1 == nullptr;
		//	continue;
		//}
		v->edgeList.push_back(e);
	}
	edgeList.clear();
	for (Face* f : faceList)
	{
		v->faceList.push_back(f);
	}
	faceList.clear();
}

Vertex* Vertex::update()
{
	if (changeTo == nullptr)
		return this;
	changeTo = changeTo->update();
	return changeTo;
}

Edge::Edge()
{
	pos = new Point();
}


Vertex* Edge::findAnotherVertex(Vertex * v)
{
	if (v == v1)
		return v2;
	if (v == v2)
		return v1;
	return nullptr;
}

bool Edge::updateVertex()
{
	v1 = v1->update();
	v2 = v2->update();
	return v1 != v2;
}


void Edge::calCost()
{
	Matrix m = v1->matrix + v2->matrix;
	double D = det(
		m.data[0][0], m.data[0][1], m.data[0][2],
		m.data[1][0], m.data[1][1], m.data[1][2],
		m.data[2][0], m.data[2][1], m.data[2][2]
		);
	if (abs(D) < 1e-10) {
		Point p = (v1->loc + v2->loc) / 2;
		pos->x = p.x;
		pos->y = p.y;
		pos->z = p.z;
	}
	else {
		pos->x = det(
			-m.data[0][3], m.data[0][1], m.data[0][2],
			-m.data[1][3], m.data[1][1], m.data[1][2],
			-m.data[2][3], m.data[2][1], m.data[2][2]
			) / D;
		pos->y = det(
			m.data[0][0], -m.data[0][3], m.data[0][2],
			m.data[1][0], -m.data[1][3], m.data[1][2],
			m.data[2][0], -m.data[2][3], m.data[2][2]
			) / D;
		pos->z = det(
			m.data[0][0], m.data[0][1], -m.data[0][3],
			m.data[1][0], m.data[1][1], -m.data[1][3],
			m.data[2][0], m.data[2][1], -m.data[2][3]
			) / D;
	}
	cost = m.data[3][3];
	double arr[3] = { pos->x, pos->y, pos->z };
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cost += arr[i] * arr[j] * m.data[i][j];
		}
		cost += arr[i] * m.data[3][i] * 2;
	}
	if (this->BorderEdge()) 
		cost += norm(v1->loc - *pos) + norm(v2->loc - *pos);
}

bool Edge::BorderEdge()
{
	return v1->edgeList.size()*2  != v1->faceList.size() 
		|| v2->edgeList.size()*2 != v2->faceList.size();

}

void Face::calMatrix()
{
	Point n = cross(v1->loc - v3->loc, v2->loc - v3->loc);
	n /= n.length();

	double temp[] = { n.x, n.y, n.z, -dot(n, v1->loc) };
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			matrix.data[i][j] = temp[i] * temp[j];
		}
	}
}

void Face::matrixAddToVertix()
{
	v1->matrix += this->matrix;
	v2->matrix += this->matrix;
	v3->matrix += this->matrix;
}

void Face::matrixSubToVertix()
{
	v1->matrix -= this->matrix;
	v2->matrix -= this->matrix;
	v3->matrix -= this->matrix;
}

bool Face::isExist(Vertex* v)
{
	return v == v1 || v == v2 || v == v3;
}

bool Face::updateVertex()
{
	v1 = v1->update();
	v2 = v2->update();
	v3 = v3->update();
	return !(v1 == v2 || v2 == v3 || v1 == v3);
}


