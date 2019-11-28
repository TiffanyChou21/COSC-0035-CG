#ifndef ENTITY
#define ENTITY
#include "ray.h"
#include "vec3.h"
#include "hittable.h"
#include <vector>
using namespace std;

class Edge;
class Vertex;
class Face;

class Vertex
{
public:
	int id;
	vec3 loc;
	vector<Edge*> edgeList;
	vector<Face*> faceList;
	Vertex* changeTo;
	vec3 normal;
public:
	Edge* connectTo(Vertex* v);
};

class Edge
{
public:
	Vertex *v1;
	Vertex *v2;
	vec3 pos;
public:
	Edge();
	Vertex* findAnotherVertex(Vertex *v);
};

class Face
{
public:
	Vertex *v1;
	Vertex *v2;
	Vertex *v3;
	vec3 normal;
public:
	bool isExist(Vertex * v);
	bool InterTriangle(const ray& r, hit_record& rec);
};

#endif