#ifndef TRANSLATEH
#define TRANSLATEH

#include "hitable.h"

// 实现对模型在坐标系的平移
class translate : public hitable
{
public:
    translate(hitable *p, const vec3 &displacement) : ptr(p), offset(displacement) {}
    //本质上是对光线进行反向的平移,毕竟光比较好移动
    virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const
    {
        ray moved_r(r.origin() - offset, r.direction());
        if (ptr->hit(moved_r, t_min, t_max, rec))
        {
            rec.p += offset;
            return true;
        }
        else
            return false;
    }
    //根据移动的offset对aabb进行同等平移,避免出现因为aabb造成的不合常理渲染
    virtual bool bounding_box(float t0, float t1, aabb &box) const
    {
        if (ptr->bounding_box(t0, t1, box))
        {
            box = aabb(box.min() + offset, box.max() + offset);
            return true;
        }
        else
            return false;
    }

    hitable *ptr;
    vec3 offset;
};

#endif
