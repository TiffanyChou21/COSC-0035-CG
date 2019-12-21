#ifndef ENTITY
#define ENTITY
#include "ray.h"
#include "vec3.h"
#include "hitable.h"
#include <vector>
using namespace std;

// 行列式
double det(
	double a11, double a12, double a13,
	double a21, double a22, double a23,
	double a31, double a32, double a33)
{
	return a11 * a22 * a33 + a12 * a23 * a31 + a13 * a21 * a32 - a13 * a22 * a31 - a12 * a21 * a33 - a11 * a32 * a23;
}

class Edge;
class Vertex;
class Face;

// 空间点
class Vertex
{
public:
	int id;
	vec3 loc;
	vector<Edge *> edgeList; // 邻接边
	vector<Face *> faceList; // 邻接面
	Vertex *changeTo;		 // 更新后的点
	vec3 normal;			 // 法向量
public:
	Edge *connectTo(Vertex *v); // 是否与零一点v相连，返回相连边
};

// 空间边
class Edge
{
public:
	// 两点
	Vertex *v1;
	Vertex *v2;
	// 边退化为一点的坐标
	vec3 pos;

public:
	Edge();
	// 根据一点找另一点
	Vertex *findAnotherVertex(Vertex *v);
};

// 三角面
class Face
{
public:
	// 三点
	Vertex *v1;
	Vertex *v2;
	Vertex *v3;
	// 法向量
	vec3 normal;

public:
	bool isExist(Vertex *v);
	// 光线与面相交判定
	bool InterTriangle(const ray &r, hit_record &rec);
};

extern vec3 light_origin;

Edge *Vertex::connectTo(Vertex *v)
{
	for (Edge *e : edgeList)
		if (e->findAnotherVertex(this) == v)
			return e;
	return nullptr;
}

Edge::Edge()
{
	pos = vec3(0, 0, 0);
}

Vertex *Edge::findAnotherVertex(Vertex *v)
{
	if (v == v1)
		return v2;
	if (v == v2)
		return v1;
	return nullptr;
}

bool Face::isExist(Vertex *v)
{
	return v == v1 || v == v2 || v == v3;
}
//光线与三角面片求交函数
bool Face::InterTriangle(const ray &r, hit_record &rec)
{
	hit_record temp_rec;
	bool hit_in = false;
	vec3 R0 = r.origin();
	vec3 Rd = r.direction();
	vec3 E1 = v1->loc - v2->loc;
	vec3 E2 = v1->loc - v3->loc;
	vec3 S = v1->loc - R0;
	float mole = det(Rd.x(), E1.x(), E2.x(), Rd.y(), E1.y(), E2.y(), Rd.z(), E1.z(), E2.z());
	temp_rec.t = det(S.x(), E1.x(), E2.x(), S.y(), E1.y(), E2.y(), S.z(), E1.z(), E2.z()) / mole;
	float beta = det(Rd.x(), S.x(), E2.x(), Rd.y(), S.y(), E2.y(), Rd.z(), S.z(), E2.z()) / mole;/*correct err*/
	float gama = det(Rd.x(), E1.x(), S.x(), Rd.y(), E1.y(), S.y(), Rd.z(), E1.z(), S.z()) / mole;/*correct err*/
	temp_rec.p = R0 + temp_rec.t * Rd;
	if (temp_rec.t > 0 && beta >= 0 && beta <= 1 && gama >= 0 && gama <= 1 && (beta + gama <= 1))
		hit_in = true;
	rec.normal = this->normal;
	rec = temp_rec;
	return hit_in;
}

#endif
