#ifndef MODEL
#define MODEL

#include "hitable.h"
#include "entity.h"
#include <string>
#include <vector>
#include <fstream>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <iostream>
#include "rotate.h"
#include "translate.h"

using namespace std;

class lambertian;
class material;
class metal;
class aabb;

class model : public hitable
{
public:
	model() {}
	model(material *b, float maxt)
		: mat_ptr(b), maxDist(maxt) {};
	virtual bool hit(const ray &r, float tmin, float tmax, hit_record &rec) const;
	virtual bool bounding_box(float t0, float t1, aabb &box) const;
	bool readFile(string filename);

	material *mat_ptr;

	vector<Vertex *> vertexV;
	vector<Face *> faceV;
	vector<Edge *> edgeV;

	// ��¼���������и��������Сֵ
	// �����ʵ��Ľ�ͼ�����ź�ƽ��
	float x_max = FLT_MIN;
	float x_min = FLT_MAX;
	float y_max = FLT_MIN;
	float y_min = FLT_MAX;
	float z_max = FLT_MIN;
	float z_min = FLT_MAX;
	float maxDist;

	// ��Χ�У� ���ٴ���
	hitable* boundBox;
};

bool model::bounding_box(float t0, float t1, aabb &box) const
{
	box = aabb(vec3(x_min, y_min, z_min), vec3(x_max, y_max, z_max));
	return true;
}

bool model::hit(const ray &r, float t_min, float t_max, hit_record &rec) const
{
	// ���û�����а�Χ�У���δ����objͼ��
	if (!boundBox->hit(r, t_min, t_max, rec)) {
		return false;
	}
	// ��������������������
	rec.t = FLT_MAX;
	hit_record temp_rec;
	bool flag = false;
	for (auto f : this->faceV)
	{
		if (f->InterTriangle(r, temp_rec) && temp_rec.t < rec.t)
		{
			rec = temp_rec;
			// һ��obj��ȡʱ��������淨�������⣬�����
			// ���ǻ���ü򻯴���(�ڶ�������ҵ)���obj�ļ������ܷ�������������ת
			// �˴��ж������˷�����ȷ�ķ�����ֵ
			if (dot(r.direction(), f->normal) < 0)
				rec.normal = f->normal;
			else
				rec.normal = -(f->normal);
			rec.mat_ptr = this->mat_ptr;
			flag = true;
		}
	}
	return flag;
}

bool model::readFile(string filename)
{
	faceV.clear();
	edgeV.clear();
	vertexV.clear();
	fstream file(filename, ios::in);
	char line[1024];
	while (!file.eof())
	{
		file >> line;
		if (strcmp(line, "v") == 0)
		{
			Vertex *newV = new Vertex;
			file >> newV->loc.e[0];
			file >> newV->loc.e[1];
			file >> newV->loc.e[2];
			if (newV->loc.x() > x_max)
				x_max = newV->loc.x();
			if (newV->loc.x() < x_min)
				x_min = newV->loc.x();
			if (newV->loc.y() > y_max)
				y_max = newV->loc.y();
			if (newV->loc.y() < y_min)
				y_min = newV->loc.y();
			if (newV->loc.z() > z_max)
				z_max = newV->loc.z();
			if (newV->loc.z() < z_min)
				z_min = newV->loc.z();
			vertexV.push_back(newV);
			newV->id = vertexV.size();
		}
		else if (strcmp(line, "f") == 0)
		{
			Face *newF = new Face;
			int id1, id2, id3;
			file >> id1 >> id2 >> id3;
			Vertex *v1, *v2, *v3;
			v1 = vertexV[id1 - 1];
			v2 = vertexV[id2 - 1];
			v3 = vertexV[id3 - 1];
			newF->v1 = v1;
			newF->v2 = v2;
			newF->v3 = v3;
			v1->faceList.push_back(newF);
			v2->faceList.push_back(newF);
			v3->faceList.push_back(newF);
			faceV.push_back(newF);
			Edge *e1 = v1->connectTo(v2);
			Edge *e2 = v2->connectTo(v3);
			Edge *e3 = v3->connectTo(v1);
			if (!e1)
			{
				e1 = new Edge;
				e1->v1 = v1;
				e1->v2 = v2;
				v1->edgeList.push_back(e1);
				v2->edgeList.push_back(e1);
				edgeV.push_back(e1);
			}
			if (!e2)
			{
				e2 = new Edge;
				e2->v1 = v2;
				e2->v2 = v3;
				v2->edgeList.push_back(e2);
				v3->edgeList.push_back(e2);
				edgeV.push_back(e2);
			}
			if (!e3)
			{
				e3 = new Edge;
				e3->v1 = v3;
				e3->v2 = v1;
				v1->edgeList.push_back(e3);
				v3->edgeList.push_back(e3);
				edgeV.push_back(e3);
			}
			// �˴����������������
			// �Ѿ���
			newF->normal = cross(newF->v2->loc - newF->v1->loc, newF->v3->loc - newF->v2->loc);
		}
	}
	cout << "�������:" << vertexV.size() << endl;
	cout << "�����:" << faceV.size() << endl;
	cout << "������:" << edgeV.size() << endl;
	file.close();

	float maxVal = FLT_MIN;
	maxVal = abs(x_max) > maxVal ? abs(x_max) : maxVal;
	maxVal = abs(y_max) > maxVal ? abs(y_max) : maxVal;
	maxVal = abs(z_max) > maxVal ? abs(z_max) : maxVal;
	maxVal = abs(x_min) > maxVal ? abs(x_min) : maxVal;
	maxVal = abs(y_min) > maxVal ? abs(y_min) : maxVal;
	maxVal = abs(z_min) > maxVal ? abs(z_min) : maxVal;

	cout << "��������obj�ļ�..." << endl;

	float roate = maxDist / maxVal;

	vec3 move = vec3(278, 0, 278);
	if (y_min < 0)
		move.e[1] = -roate*y_min;

	// �Ը�����λ�ý�������ƽ��
	for (auto v : this->vertexV) {
		v->loc *= roate;
		v->loc += move;
		//cout << "(" << v->loc.x() << ", " << v->loc.y() << ", " << v->loc.z() << ")" << endl;
	}
	x_max *= roate; x_max += 278;
	x_min *= roate; x_min += 278;
	y_max *= roate; y_max += move.y();
	y_min *= roate; y_min += move.y();
	z_max *= roate; z_max += 278;
	z_min *= roate; z_min += 278;

	cout << "obj�ļ�������" << roate << "��" << endl;
	cout << "objƽ������������" << endl;
	cout << "��Χ��λ��:(" << x_min << ", " << y_min << ", " << z_min << ") --> ("
		<< x_max << ", " << y_max << ", " << z_max << ")" << endl;
	boundBox = new box(vec3(x_min, y_min, z_min), vec3(x_max, y_max, z_max), this->mat_ptr);
	cout << "��Χ�������" << endl;
	return true;
}

#endif
