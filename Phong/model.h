#ifndef MODEL
#define MODEL

#include "hittable.h"
#include "entity.h"
#include <string>
#include <vector>
#include <fstream>
#include <string.h>
#include <math.h>
#include <limits.h>

using namespace std;

class lambertian;
class metal;
class model : public hittable {
public:
	model() {}
	model(lambertian *a, metal *b)
		: lam_ptr(a), met_ptr(b) {};
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
    bool readFile(string filename);
	lambertian *lam_ptr; /* NEW */
	metal *met_ptr;

	vector<Vertex*> vertexV;
	vector<Face*> faceV;
	vector<Edge*> edgeV;

	float x_max = FLT_MIN;
	float x_min = FLT_MAX;
	float y_max = FLT_MIN;
	float y_min = FLT_MAX;
	float z_max = FLT_MIN;
	float z_min = FLT_MAX;
};
bool model::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	rec.t = FLT_MAX;
	hit_record temp_rec;
	bool flag = false;
	for (auto f : this->faceV) {
		if (f->InterTriangle(r, temp_rec) && temp_rec.t < rec.t) {
			rec = temp_rec;
			rec.normal = f->normal;
			rec.lam_ptr = this->lam_ptr;
			rec.met_ptr = this->met_ptr;
			flag = true;
		}
	}
	return flag;
}

bool model::readFile(string filename) {
	faceV.clear();
	edgeV.clear();
	vertexV.clear();
	fstream file(filename, ios::in);
	char line[1024];
	while (!file.eof())
	{
		file >> line;
		if (strcmp(line, "v") == 0) // 椤剁偣
		{
			// 璇诲叆椤剁偣
			Vertex* newV = new Vertex;
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
			// 鍔犲叆椤剁偣鍒楄〃
			vertexV.push_back(newV);
			// 璁板綍id
			newV->id = vertexV.size();
		}
		else if (strcmp(line, "f") == 0) // 闈?
		{
			Face* newF = new Face;
			// 璇诲叆涓変釜椤剁偣id
			int id1, id2, id3;
			file >> id1 >> id2 >> id3;
			// 鑾峰彇涓変釜椤剁偣
			Vertex* v1, *v2, *v3;
			v1 = vertexV[id1 - 1];
			v2 = vertexV[id2 - 1];
			v3 = vertexV[id3 - 1];
			// 闈笂璁板綍涓変釜椤剁偣
			newF->v1 = v1;
			newF->v2 = v2;
			newF->v3 = v3;
			// 璁板綍椤剁偣閾炬帴鐨勯潰
			v1->faceList.push_back(newF);
			v2->faceList.push_back(newF);
			v3->faceList.push_back(newF);
			faceV.push_back(newF);
			// 涓夋潯杈?
			Edge* e1 = v1->connectTo(v2);
			Edge* e2 = v2->connectTo(v3);
			//Edge* e3 = v1->connectTo(v2);
			Edge* e3 = v3->connectTo(v1);
			if (!e1)
			{
				// 杈硅缃《鐐?
				e1 = new Edge; e1->v1 = v1; e1->v2 = v2;
				// 璁板綍椤剁偣鐩歌繛鐨勮竟
				v1->edgeList.push_back(e1);
				v2->edgeList.push_back(e1);
				// 鍔犲叆杈瑰垪琛?
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
			newF->normal = cross(newF->v2->loc - newF->v1->loc, newF->v3->loc - newF->v2->loc);
		}
	}
	cout << "顶点个数:" << vertexV.size() << endl;
	cout << "面个数:" << faceV.size() << endl;
	cout << "边条数:" << edgeV.size() << endl;
	file.close();
	return true;
}

#endif
