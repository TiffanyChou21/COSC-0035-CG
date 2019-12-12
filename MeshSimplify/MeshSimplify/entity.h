#ifndef ENTITY
#define ENTITY
#include "Point.h"
#include "Matrix.h"
#include <vector>
using namespace std;

class Edge;
class Vertex;
class Face;

class Vertex
{
public:
	int id;
	point loc;
	vector<Edge*> edgeList;
	vector<Face*> faceList;
	Matrix matrix;
	Vertex* changeTo;
	point normal;
public:
	Edge* connectTo(Vertex* v);
	void combineTo(Vertex* v);
	Vertex* update();
};

class Edge
{
public:
	Vertex *v1;
	Vertex *v2;
	point *pos;
	double cost;
	bool drawn = true;
public:
	Edge();
	Vertex* findAnotherVertex(Vertex *v);
	bool updateVertex();
	void calCost();
	bool BorderEdge();
};

class Face
{
public:
	Vertex *v1;
	Vertex *v2;
	Vertex *v3;
	Matrix matrix;
	point normal;
public:
	void calMatrix();
	void matrixAddToVertix();
	void matrixSubToVertix();
	bool isExist(Vertex * v);
	bool updateVertex();
};


struct cmpEdgeCost
{
	bool operator()(Edge *a, Edge *b)
	{
		return a->cost > b->cost;
	}
};



#endif