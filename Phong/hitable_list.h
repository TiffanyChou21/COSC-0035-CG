#ifndef HITABLELISTH
#define HITABLELISTH

#include "hitable.h"
#include "aabb.h"

class hitable_list : public hitable
{
public:
    hitable_list() {}
    hitable_list(hitable **l, int n) { list = l; list_size = n; }
    virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec)const
    {
        hit_record temp_rec;
        bool hit_anything = false;
        float closest_so_far = tmax;
        for (int i = 0; i < list_size; i++)
        {
            if (list[i]->hit(r, tmin, closest_so_far, temp_rec))
            {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }
        return hit_anything;
    }

    bool bounding_box(float t0, float t1, aabb& box) const
    {
        //这里可以不实现, hitable_list只是一个容器
        box = aabb(vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f));
        return true;
    }


    hitable ** list;
    int list_size;
};


#endif