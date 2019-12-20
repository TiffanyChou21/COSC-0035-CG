#ifndef MATERIALH
#define MATERIALH

#include "ray.h"
#include "vec3.h"
#include "hitable.h"

// 材质基类， 虚基类
// 所有材质需实现此类
class material
{
public:
    virtual bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered, float &pdf)
    {
        return false;
    };
    virtual float scattering_pdf(const ray &r_in, const hit_record &rec, const ray &scattered) const
    {
        return 0.0f;
    }
    virtual vec3 emitted(const ray &r_in, const hit_record &rec, float u, float v, const vec3 &p) const
    {
        return vec3(0, 0, 0);
    }
};

#endif