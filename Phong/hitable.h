#ifndef HITABLEH
#define HITABLEH

#include "ray.h"

class material;
class aabb;
class reflection;

struct hit_record
{
    float t; //相交点直线的参数t
    vec3 p;  //根据参数t求得的p
    vec3 normal; //p点的法线
    float u, v;  //交点处的纹理坐标
    material *mat_ptr; //光线相交处的材质
};


class hitable
{
public:
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
    virtual bool bounding_box(float t0, float t1, aabb& box) const = 0;
    virtual float pdf_value(const vec3& o, const vec3& v) const { return 0.0f; }
    virtual vec3 random(const vec3& o) const { return vec3(1, 0, 0); }
};


#endif