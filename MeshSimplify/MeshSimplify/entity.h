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
	// 第id个点
	int id; 
	// 点在空间中位置
	point loc;
	// 邻接边
	vector<Edge*> edgeList;
	// 邻接面
	vector<Face*> faceList;
	// 维护的矩阵
	Matrix matrix;
	// 与另一个点合并后的位置
	Vertex* changeTo;
	// 法向量
	point normal;
public:
	// 将该点与参数点连接成边并返回边
	Edge* connectTo(Vertex* v);
	// 将点邻接面&边加入到v，并删除自身
	void combineTo(Vertex* v);
	// 更新合并后点的位置
	Vertex* update();
};

class Edge
{
public:
	// 端点1
	Vertex *v1;
	// 端点2
	Vertex *v2;
	// 删除后合成一点的位置
	point *pos;
	// cost值
	double cost;
	// 是否要被画出，附加功能显示上用到
	bool drawn = true;
public:
	Edge();
	// 根据一个端点找另一个端点
	Vertex* findAnotherVertex(Vertex *v);
	// 更新端点
	bool updateVertex();
	// 计算cost值
	void calCost();
	// 是否是边界边
	bool BorderEdge();
};

class Face
{
public:
	// 端点1
	Vertex *v1;
	// 端点2
	Vertex *v2;
	// 端点3
	Vertex *v3;
	// 维护的矩阵
	Matrix matrix;
	// 法向量
	point normal;
public:
	// 计算矩阵
	void calMatrix();
	// 将矩阵加到周围点上
	void matrixAddToVertix();
	// 将矩阵减到周围点上
	void matrixSubToVertix();
	// 点v是否在面上，用于面的去重
	bool isExist(Vertex * v);
	// 更新周围点信息
	bool updateVertex();
};

// 边cost比较函数，用于优先队列
struct cmpEdgeCost
{
	bool operator()(Edge *a, Edge *b)
	{
		return a->cost > b->cost;
	}
};



#endif