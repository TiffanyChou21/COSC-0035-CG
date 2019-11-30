#ifndef AABBH
#define AABBH
#include "ray.h"
#include "hittable.h"

class aabb {
    public:
        aabb() {}
        aabb(const vec3& a, const vec3& b) { _min = a; _max = b;}//对角线两点
        vec3 min() const {return _min; }
        vec3 max() const {return _max; }

        bool hit(const ray& r, float tmin, float tmax) const {
            for (int a = 0; a < 3; a++) {
                float t0 = std::min((_min[a] - r.origin()[a]) / r.direction()[a],
                                (_max[a] - r.origin()[a]) / r.direction()[a]);
                float t1 = std::max((_min[a] - r.origin()[a]) / r.direction()[a],
                                (_max[a] - r.origin()[a]) / r.direction()[a]);
                tmin = std::max(t0, tmin);
                tmax = std::min(t1, tmax);
                if (tmax <= tmin)//slab重叠
                    return false;
            }
            return true;
        }
        vec3 _min;
        vec3 _max;
};
aabb surrounding_box(aabb box0, aabb box1) {
    vec3 small( std::min(box0.min().x(), box1.min().x()),
                std::min(box0.min().y(), box1.min().y()),
                std::min(box0.min().z(), box1.min().z()));
    vec3 big  ( std::max(box0.max().x(), box1.max().x()),
                std::max(box0.max().y(), box1.max().y()),
                std::max(box0.max().z(), box1.max().z()));
    return aabb(small,big);
}

#endif

