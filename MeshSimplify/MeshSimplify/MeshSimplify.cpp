#include "MeshSimplify.h"
#include <iostream>
#include <set>
#include <algorithm>
#include <functional>
#include <iomanip>
using namespace std;

point cross(point &a, point &b);

bool cmpEdge(Edge* e1, Edge* e2) {
	//return e1->cost < e2->cost;
	if (e1->cost == e2->cost) {
		return e1 < e2;
	}
	else {
		return e1->cost < e2->cost;
	}
}

bool equalEdge(Edge* e1, Edge* e2) {
	return (e1->v1 == e2->v1 && e1->v2 == e2->v2)
		|| (e1->v1 == e2->v2 && e1->v2 == e2->v1);
}

bool cmpFace(Face* f1, Face* f2) {
	return f1 < f2;
}

bool equalFace(Face* f1, Face* f2) {
	return (f1->v1 == f2->v1 && f1->v2 == f2->v2 && f1->v3 == f2->v3)
		|| (f1->v1 == f2->v1 && f1->v2 == f2->v3 && f1->v3 == f2->v2)
		|| (f1->v1 == f2->v2 && f1->v2 == f2->v1 && f1->v3 == f2->v3)
		|| (f1->v1 == f2->v2 && f1->v2 == f2->v3 && f1->v3 == f2->v1)
		|| (f1->v1 == f2->v3 && f1->v2 == f2->v1 && f1->v3 == f2->v2)
		|| (f1->v1 == f2->v3 && f1->v2 == f2->v2 && f1->v3 == f2->v1);
}

MeshSimplify::MeshSimplify()
{
}


MeshSimplify::~MeshSimplify()
{
}

bool MeshSimplify::readFile(string filename)
{
	faceV.clear();
	edgeV.clear();
	vertexV.clear();
	fstream file(filename, ios::in);
	char line[1024];
	while (!file.eof()) 
	{
		file >> line;
		if (strcmp(line, "v") == 0) // 顶点
		{
			// 读入顶点
			Vertex* newV = new Vertex;
			file >> newV->loc.x;
			file >> newV->loc.y;
			file >> newV->loc.z;
			if (abs(newV->loc.x) > maxLen)
				maxLen = newV->loc.x;
			if (abs(newV->loc.y) > maxLen)
				maxLen = newV->loc.y;
			if (abs(newV->loc.z) > maxLen)
				maxLen = newV->loc.z;
			// 加入顶点列表
			vertexV.push_back(newV);
			// 记录id
			newV->id = vertexV.size();
		}
		else if (strcmp(line, "f") == 0) // 面
		{
			Face* newF = new Face;
			// 读入三个顶点id
			int id1, id2, id3;
			file >> id1 >> id2 >> id3;
			// 获取三个顶点
			Vertex* v1, *v2, *v3;
			v1 = vertexV[id1 - 1];
			v2 = vertexV[id2 - 1];
			v3 = vertexV[id3 - 1];
			// 面上记录三个顶点
			newF->v1 = v1;
			newF->v2 = v2;
			newF->v3 = v3;
			// 记录顶点链接的面
			v1->faceList.push_back(newF);
			v2->faceList.push_back(newF);
			v3->faceList.push_back(newF);
			faceV.push_back(newF);
			// 三条边
			Edge* e1 = v1->connectTo(v2);
			Edge* e2 = v2->connectTo(v3);
			Edge* e3 = v1->connectTo(v2);
			if (!e1)
			{
				// 边设置顶点
				e1 = new Edge; e1->v1 = v1; e1->v2 = v2;
				// 记录顶点相连的边
				v1->edgeList.push_back(e1);
				v2->edgeList.push_back(e1);
				// 加入边列表
				edgeV.push_back(e1);
			}
			if (!e2)
			{
				e2 = new Edge; e2->v1 = v2; e2->v2 = v3;
				v2->edgeList.push_back(e2);
				v3->edgeList.push_back(e2);
				edgeV.push_back(e2);
			}
			if (!e3)
			{
				e3 = new Edge; e3->v1 = v3; e3->v2 = v1;
				v1->edgeList.push_back(e3);
				v3->edgeList.push_back(e3);
				edgeV.push_back(e3);
			}

			newF->calMatrix();
			newF->matrixAddToVertix();
		}
	}
	cout << "顶点个数:" << vertexV.size() << endl;
	cout << "面个数:" << faceV.size() << endl;
	cout << "边条数:" << edgeV.size() << endl;
	file.close();
	return true;
}

void MeshSimplify::writeFile(string filename)
{
	fstream file(filename, ios::out);
	for (Vertex* v : vertexV) 
	{
		file << "v " 
			<< v->loc.x << " " 
			<< v->loc.y << " " 
			<< v->loc.z << endl;
	}
	for (Face* f : faceV) 
	{
		file << "f "
			<< f->v1->id << " "
			<< f->v2->id << " "
			<< f->v3->id << endl;
	}
	file.close();
}

void MeshSimplify::simplify(double rate)
{
	int curFaceNum = faceV.size();
	int endNum = (int)curFaceNum * rate;
	int distance = curFaceNum - endNum;
	cout << "原有面数：" << curFaceNum << endl;
	cout << "目标面数:" << endNum << endl;
	cout << "当前面数：" << curFaceNum << endl;
	cout << "正在计算cost值..." << endl;
	set<Edge*, function<bool(Edge*, Edge*)>> edgeSet(cmpEdge);
	//priority_queue<Edge*, vector<Edge*>, cmpEdgeCost> edgeQueue;
	for (Edge* e : edgeV) {
		e->calCost();

		edgeSet.insert(e);
		//edgeQueue.push(e);
	}

	//return;
	cout << "计算成功" << endl;
	cout << "正在删除边" << endl;
	int rmngNum = distance;
	//while (curFaceNum > endNum)
	while(rmngNum > 0)
	{
		Edge* e = *(edgeSet.begin());
		edgeSet.erase(e);

		//for (int i = 10; i >= 0; i--)
		//{
		//	cout << e->cost << endl;
		//	Edge* e = *(edgeSet.begin());
		//	cout << e->v1->id << endl;
		//	edgeSet.erase(e);
		//}
		//return;

		//Edge* e = edgeQueue.top();
		//edgeQueue.pop();
		Vertex* v1 = e->v1;
		Vertex* v2 = e->v2;
		if (v1 != v1->update() || 
			v2 != v2->update() || 
			v1 == v2) 
			continue;

		v2->combineTo(v1);
		v1->loc.x = e->pos->x;
		v1->loc.y = e->pos->y;
		v1->loc.z = e->pos->z;

		for (vector<Edge*>::iterator it = v1->edgeList.begin(); it != v1->edgeList.end();) {
			Edge* temp = *it;
			edgeSet.erase(temp);
			if (!(*it)->updateVertex()) {
				it = v1->edgeList.erase(it);
				continue;
			}
			it++;
		}

		//for (vector<Edge*>::iterator it = v1->edgeList.begin(); it != v1->edgeList.end();) {
		//	if (!(*it)->updateVertex())
		//		it = v1->edgeList.erase(it);
		//	it++;
		//}

		//sort(v1->edgeList.begin(),v1->edgeList.end(), cmpEdge);
		//v1->edgeList.erase(
		//		unique(v1->edgeList.begin(), v1->edgeList.end(), equalEdge),
		//		v1->edgeList.end()
		//	);
		sort(v1->edgeList.begin(), v1->edgeList.end(),
			[&v1](Edge* a, Edge* b) {
			Vertex* ta = a->findAnotherVertex(v1);
			Vertex* tb = b->findAnotherVertex(v1);
			if (ta == tb) {
				return a < b;
			}
			else {
				return ta < tb;
			}
		});
		unique(v1->edgeList.begin(), v1->edgeList.end(), 
			[&v1](Edge* a, Edge* b) {
			return a->findAnotherVertex(v1) == b->findAnotherVertex(v1);
		});

		vector<Edge*> modifiedEdge;
		//if (!recalculate) {
		modifiedEdge.insert(modifiedEdge.end(), v1->edgeList.begin(), v1->edgeList.end());
		//}

		sort(v1->faceList.begin(), v1->faceList.end());
		unique(v1->faceList.begin(), v1->faceList.end());
		for (vector<Face*>::iterator it = v1->faceList.begin(); it != v1->faceList.end(); it++) {
			if (!(*it)->v1)
				continue;
			(*it)->updateVertex();
		}

		for (vector<Face*>::iterator it = v1->faceList.begin(); it != v1->faceList.end(); ) {
			if (!(*it)->v1 || !(*it)->updateVertex()) { //无效面
				if ((*it)->v1) {
					(*it)->v1 = nullptr;//标记面失效
					curFaceNum--;//面数减少计数
					rmngNum--;
				}
				it = v1->faceList.erase(it);
				continue;
			}
			it++;
		}

		sort(modifiedEdge.begin(), modifiedEdge.end());
		modifiedEdge.resize((unique(modifiedEdge.begin(), modifiedEdge.end()) - modifiedEdge.begin()));
		for (Edge* x : modifiedEdge) {
			edgeSet.erase(x);
			x->updateVertex();
			x->calCost();
			edgeSet.insert(x);
		}
		//for (auto it = v1->faceList.begin(); it != v1->faceList.end();) {
		//	if ((*it)->v1 == nullptr)
		//	{
		//		v1->faceList.erase(it);
		//		curFaceNum--;
		//		continue;
		//	}
		//	it++;
		//}

		//sort(v1->faceList.begin(), v1->faceList.end(), cmpFace);
		//v1->faceList.erase(
		//	unique(v1->faceList.begin(), v1->faceList.end(), equalFace),
		//	v1->faceList.end()
		//	);

		//cout << "当前面数：" << curFaceNum << endl;
		double finishRate = 100.0*(distance - rmngNum) / distance;
		cout << "[";
		int i;
		for (i = 0; i < int(finishRate)/2; i++)
			cout << ">";
		while (i++ < 50)
			cout << "=";
		cout << "][" << (int)finishRate << "%]                  \r";
	}
	vector<Vertex*> ve;
	for (Vertex* v : vertexV) {
		if (v->update() == v) {
			ve.push_back(v);
			v->id = ve.size();
		}
	}
	vertexV = move(ve);
	vector<Face*> fa;
	for (Face* f : faceV) {
		if (f->v1 != nullptr && f->updateVertex()) {
			fa.push_back(f);
		}
	}
	faceV = move(fa);
	cout << endl;
}

void MeshSimplify::drawLine() {
	glLineWidth(1.0);
	glColor3f(0.5, 0.5, 0.5);
	glBegin(GL_LINES);
	for (vector<Edge*>::iterator it = edgeV.begin(); it != edgeV.end(); it++)
	{
		Edge* edge = *it;
		if (!edge->drawn)
			continue;
		Vertex* v1 = edge->v1;
		Vertex* v2 = edge->v2;
		glVertex3f(v1->loc.x , v1->loc.y, v1->loc.z);
		glVertex3f(v2->loc.x, v2->loc.y, v2->loc.z);
	}
	glEnd();
}

void MeshSimplify::drawFace() {
	

	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_TRIANGLES);
	for (vector<Face*>::iterator it = faceV.begin(); it != faceV.end(); it++)
	{
		Face* face = *it;
		Vertex* v1 = face->v1;
		Vertex* v2 = face->v2;
		Vertex* v3 = face->v3;
		glVertex3f(v1->loc.x, v1->loc.y , v1->loc.z );
		glVertex3f(v2->loc.x , v2->loc.y , v2->loc.z );
		glVertex3f(v3->loc.x , v3->loc.y , v3->loc.z );
		glNormal3f(face->normal.x, face->normal.y, face->normal.z);
	}
	glEnd();
}

void MeshSimplify::delUselessLine() {
	int count = 0;
	for (Vertex* v : vertexV) {
		// v的法向量
		v->normal.x = v->normal.y = v->normal.z = 0;
		for (Face* f : v->faceList) {
			v->normal.x = f->normal.x;
			v->normal.y = f->normal.y;
			v->normal.z = f->normal.z;
		}
		// v的单位法向量
		v->normal /= v->normal.length();
		// 面的单位法向量
		Face* face = *faceV.begin();
		point faceNormal = face->normal;
		if (abs(faceNormal.x - v->normal.x) < 0.2
			&& abs(faceNormal.y - v->normal.y) < 0.2
			&& abs(faceNormal.z - v->normal.z) < 0.2
			)
		{
			for (Edge* e : v->edgeList)
			{
				e->drawn = false;
				count++;
			}
		}
	}
	cout << count << "条线未显示" << endl; 
}