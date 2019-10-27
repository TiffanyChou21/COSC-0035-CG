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
	Point loc;
	vector<Edge*> edgeList;
	vector<Face*> faceList;
	Matrix matrix;
	Vertex* changeTo;
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
	Point *pos;
	double cost;
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