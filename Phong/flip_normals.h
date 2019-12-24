#ifndef FLIPNORMALSH
#define FLIPNORMALSH

#include "hitable.h"

// 用于翻转面的法向量
class flip_normals : public hitable
{
public:
    flip_normals(hitable *p) : ptr(p) {}
    virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const
    {
        if (ptr->hit(r, t_min, t_max, rec))
        {
            // 命中后法向量设为负值
            rec.normal = -rec.normal;
            return true;
        }
        else
            return false;
    }

    virtual bool bounding_box(float t0, float t1, aabb &box) const
    {
        return ptr->bounding_box(t0, t1, box);
    }

    virtual float pdf_value(const vec3 &o, const vec3 &v) const
    {
        return ptr->pdf_value(o, v);
    }

    virtual vec3 random(const vec3 &o) const
    {
        return ptr->random(o);
    }

    hitable *ptr;
};

#endif