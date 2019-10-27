#ifndef MESHSIPLIFY
#define MESHSIPLIFY

#include "entity.h"
#include <fstream>


class MeshSimplify
{
public:
	vector<Vertex*> vertexV;
	vector<Face*> faceV;
	vector<Edge*> edgeV;
public:
	MeshSimplify();
	~MeshSimplify();
	bool readFile(string filename);
	void writeFile(string filename);
	void simplify(double rate);
};

#endif
