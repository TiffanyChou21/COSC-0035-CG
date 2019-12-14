#include "MeshSimplify.h"
#include <iostream>
#include <set>
#include <algorithm>
#include <functional>
#include <iomanip>
using namespace std;

point cross(point &a, point &b);

// 边的cost值比较
// 用于优先队列排序
bool cmpEdge(Edge* e1, Edge* e2) {
	//return e1->cost < e2->cost;
	if (e1->cost == e2->cost) {
		return e1 < e2;
	}
	else {
		return e1->cost < e2->cost;
	}
}

// 判断两边相等(判断对应点相同，而不是指针地址相同)
// 用于边的去重
bool equalEdge(Edge* e1, Edge* e2) {
	return (e1->v1 == e2->v1 && e1->v2 == e2->v2)
		|| (e1->v1 == e2->v2 && e1->v2 == e2->v1);
}

// 面排序函数
bool cmpFace(Face* f1, Face* f2) {
	return f1 < f2;
}

// 判断两面相等(判断对应点相同，而不是指针地址相同)
// 用于面的去重
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
	// 清空数据
	faceV.clear();
	edgeV.clear();
	vertexV.clear();
	fstream file(filename, ios::in);
	char line[1024];
	while (!file.eof()) 
	{
		file >> line;
		if (strcmp(line, "v") == 0) // 点
		{
			// 读取点信息
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
			// 加入点列表
			vertexV.push_back(newV);
			// 点的id
			newV->id = vertexV.size();
		}
		else if (strcmp(line, "f") == 0) // 面
		{
			Face* newF = new Face;
			// 三点id
			int id1, id2, id3;
			file >> id1 >> id2 >> id3;
			// 获取对应点
			Vertex* v1, *v2, *v3;
			v1 = vertexV[id1 - 1];
			v2 = vertexV[id2 - 1];
			v3 = vertexV[id3 - 1];
			// 面上记录邻接点
			newF->v1 = v1;
			newF->v2 = v2;
			newF->v3 = v3;
			// 点上记录邻接面
			v1->faceList.push_back(newF);
			v2->faceList.push_back(newF);
			v3->faceList.push_back(newF);
			faceV.push_back(newF);
			// 判断三边是否存在
			Edge* e1 = v1->connectTo(v2);
			Edge* e2 = v2->connectTo(v3);
			Edge* e3 = v3->connectTo(v1);
			// 如果不空，则边已经存在，无需加入重复边
			if (!e1)
			{
				e1 = new Edge; e1->v1 = v1; e1->v2 = v2;
				// 点上记录邻接边
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

			// 计算面维护的矩阵
			newF->calMatrix();
			// 将矩阵值加到点上
			newF->matrixAddToVertix();
		}
	}
	cout << "点个数:" << vertexV.size() << endl;
	cout << "面个数:" << faceV.size() << endl;
	cout << "边条数:" << edgeV.size() << endl;
	file.close();
	return true;
}

// 将简化后的模型保存到文件中
void MeshSimplify::writeFile(string filename)
{
	fstream file(filename, ios::out);
	// 点信息
	for (Vertex* v : vertexV) 
	{
		file << "v " 
			<< v->loc.x << " " 
			<< v->loc.y << " " 
			<< v->loc.z << endl;
	}
	// 面信息
	for (Face* f : faceV) 
	{
		file << "f "
			<< f->v1->id << " "
			<< f->v2->id << " "
			<< f->v3->id << endl;
	}
	file.close();
}

// 输入参数为简化比例
void MeshSimplify::simplify(double rate)
{
	int curFaceNum = faceV.size();
	int endNum = (int)curFaceNum * rate;
	int distance = curFaceNum - endNum;
	cout << "原有面数:" << curFaceNum << endl;
	cout << "目标面数:" << endNum << endl;
	cout << "当前面数:" << curFaceNum << endl;
	cout << "正在计算costֵ值..." << endl;
	// 用set集合实现去重，同时给出cmpEdge函数实现优先队列
	set<Edge*, function<bool(Edge*, Edge*)>> edgeSet(cmpEdge);
	//priority_queue<Edge*, vector<Edge*>, cmpEdgeCost> edgeQueue;
	for (Edge* e : edgeV) {
		// 计算cost值
		e->calCost();
		edgeSet.insert(e);
		//edgeQueue.push(e);
	}

	//return;
	cout << "计算成功" << endl;
	cout << "正在删除边" << endl;
	// 要删除的边数目
	int rmngNum = distance;
	//while (curFaceNum > endNum)
	while(rmngNum > 0)
	{
		// 删除优先队列第一个
		// 即删除cost最小边
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
		
		// 更新点位置(上一轮循环可能有合并)
		Vertex* v1 = e->v1;
		Vertex* v2 = e->v2;
		if (v1 != v1->update() || 
			v2 != v2->update() || 
			v1 == v2) 
			continue;

		// 将v2邻接的边&面加入到v1上，并计算合并后的点位置
		v2->combineTo(v1);
		// v1移动到合并后的点位置
		v1->loc.x = e->pos->x;
		v1->loc.y = e->pos->y;
		v1->loc.z = e->pos->z;

		// 将v1周围的边在优先队列中删除
		// 因为这些边需要重新计算cost值
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

		// 对v1周围的边排序去重
		// 排序
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
		// 去重
		unique(v1->edgeList.begin(), v1->edgeList.end(), 
			[&v1](Edge* a, Edge* b) {
			return a->findAnotherVertex(v1) == b->findAnotherVertex(v1);
		});

		// 记录修改过的边
		vector<Edge*> modifiedEdge;
		modifiedEdge.insert(modifiedEdge.end(), v1->edgeList.begin(), v1->edgeList.end());

		// 对面排序去重
		sort(v1->faceList.begin(), v1->faceList.end());
		unique(v1->faceList.begin(), v1->faceList.end());
		// 更新面
		for (vector<Face*>::iterator it = v1->faceList.begin(); it != v1->faceList.end(); it++) {
			if (!(*it)->v1)
				continue;
			(*it)->updateVertex();
		}

		for (vector<Face*>::iterator it = v1->faceList.begin(); it != v1->faceList.end(); ) {
			if (!(*it)->v1 || !(*it)->updateVertex()) { // 坏面
				if ((*it)->v1) {
					(*it)->v1 = nullptr; // 设置为坏面
					curFaceNum--;// 当前面数量减少1
					rmngNum--;
				}
				// 坏面删除
				it = v1->faceList.erase(it);
				continue;
			}
			it++;
		}

		// 再次对边排序去重
		sort(modifiedEdge.begin(), modifiedEdge.end());
		modifiedEdge.resize((unique(modifiedEdge.begin(), modifiedEdge.end()) - modifiedEdge.begin()));
		// 重新计算cost值并加入边优先队列
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

		//cout << "当前面数" << curFaceNum << endl;

		// 显示当前完成进度
		double finishRate = 100.0*(distance - rmngNum) / distance;
		cout << "[";
		int i;
		for (i = 0; i < int(finishRate)/2; i++)
			cout << ">";
		while (i++ < 50)
			cout << "=";
		cout << "][" << (int)finishRate << "%]                  \r";
	}

	// 将点结果集替换原来点集合
	vector<Vertex*> ve;
	for (Vertex* v : vertexV) {
		if (v->update() == v) {
			ve.push_back(v);
			v->id = ve.size();
		}
	}
	vertexV = move(ve);
	// 面结果集替换原来面集合
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
		// v�ķ�����
		v->normal.x = v->normal.y = v->normal.z = 0;
		for (Face* f : v->faceList) {
			v->normal.x = f->normal.x;
			v->normal.y = f->normal.y;
			v->normal.z = f->normal.z;
		}
		// v�ĵ�λ������
		v->normal /= v->normal.length();
		// ��ĵ�λ������
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
	cout << count << "条边未显示" << endl; 
}