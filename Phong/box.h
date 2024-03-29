#ifndef BOXH
#define BOXH

#include "hitable.h"
#include "rect.h"
#include "flip_normals.h"
#include "hitable_list.h"

// 可作为立方体
// 也可作为包围盒
class box : public hitable
{
public:
    box() {}
    box(const vec3 &p0, const vec3 &p1, material *ptr)
    {
        pmin = p0;
        pmax = p1;
        // 六个面
        hitable **list = new hitable *[6];
        list[0] = new xy_rect(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), ptr);
        list[1] = new flip_normals(new xy_rect(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), ptr));
        list[2] = new xz_rect(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), ptr);
        list[3] = new flip_normals(new xz_rect(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), ptr));
        list[4] = new yz_rect(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), ptr);
        list[5] = new flip_normals(new yz_rect(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), ptr));
        list_ptr = new hitable_list(list, 6);
    }
    virtual bool hit(const ray &r, float t0, float t1, hit_record &rec) const
    {
        return list_ptr->hit(r, t0, t1, rec);
    }
    virtual bool bounding_box(float t0, float t1, aabb &box) const
    {
        box = aabb(pmin, pmax);
        return true;
    }
    vec3 pmin, pmax;
    hitable *list_ptr;
};

#endif