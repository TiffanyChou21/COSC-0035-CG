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
	// 读取文件，读取成功返回true
	bool readFile(string filename);
	// 写入文件
	void writeFile(string filename);
	// 开始简化
	void simplify(double rate);
	// 用于OpenGL显示的函数
	void drawLine();
	void drawFace();
	void delUselessLine();
};

#endif
