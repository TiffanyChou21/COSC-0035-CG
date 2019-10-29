#ifndef MESHSIPLIFY
#define MESHSIPLIFY

#include "entity.h"
#include <fstream>
#include <GL\glut.h>
#include <limits>


class MeshSimplify
{
public:
	float maxLen = -1;

	vector<Vertex*> vertexV;
	vector<Face*> faceV;
	vector<Edge*> edgeV;
public:
	MeshSimplify();
	~MeshSimplify();
	bool readFile(string filename);
	void writeFile(string filename);
	void simplify(double rate);
	void drawLine();
	void drawFace();
	void delUselessLine();
};

#endif
