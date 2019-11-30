#include "entity.h"
#include "function.h"
#include <iostream>
using namespace std;

extern vec3 light_origin;

Edge* Vertex::connectTo(Vertex * v)
{
	for (Edge* e : edgeList)
		if (e->findAnotherVertex(this) == v)
			return e;
	return nullptr;
}


Edge::Edge()
{
	pos = vec3(0,0,0);
}


Vertex* Edge::findAnotherVertex(Vertex * v)
{
	if (v == v1)
		return v2;
	if (v == v2)
		return v1;
	return nullptr;
}


bool Face::isExist(Vertex* v)
{
	return v == v1 || v == v2 || v == v3;
}

bool Face::InterTriangle(const ray& r, hit_record& rec)
{
	hit_record temp_rec;
    bool hit_in = false;
	vec3 R0=r.origin();
	vec3 Rd=r.direction();
	vec3 E1=v1->loc-v2->loc;
	vec3 E2=v1->loc-v3->loc;
	vec3 S=v1->loc-R0;
	float mole =det(Rd.x(),E1.x(),E2.x(),Rd.y(),E1.y(),E2.y(),Rd.z(),E1.z(),E2.z());
	temp_rec.t=det(S.x(),E1.x(),E2.x(),S.y(),E1.y(),E2.y(),S.z(),E1.z(),E2.z())/mole;
	temp_rec.p.e[1]=det(Rd.x(),S.x(),E2.x(),Rd.y(),S.y(),E2.y(),Rd.z(),S.z(),E2.z())/mole;
	temp_rec.p.e[2]=det(Rd.x(),E1.x(),S.x(),Rd.y(),E1.y(),S.y(),Rd.z(),E1.z(),S.z())/mole;
	temp_rec.p.e[0]=1-temp_rec.p.e[1]-temp_rec.p.e[2];
	if(temp_rec.t>0&&temp_rec.p.y()>=0&&temp_rec.p.y()<=1&&temp_rec.p.z()>=0&&temp_rec.p.z()<=1&&(temp_rec.p.y()+temp_rec.p.z()<=1))
		hit_in=true;
	rec.normal = this->normal;
	rec = temp_rec;
    return hit_in;
}
