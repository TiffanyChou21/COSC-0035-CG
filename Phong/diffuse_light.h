#ifndef DIFFUSE_LIGHTH
#define DIFFUSE_LIGHTH

#include "material.h"
#include "texture.h"

class diffuse_light : public material     //自发光体类
{
public:
    diffuse_light(texture *a) : emit(a) {}
    virtual bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const
    {
        return false;     //不会反射散射只会反向变成从这里发出的光↓emitted
    }

    virtual vec3 emitted(const ray &r_in, const hit_record &rec, float u, float v, const vec3 &p) const
    {
        if (dot(rec.normal, r_in.direction()) < 0.0f)
            return emit->value(u, v, p);   //小于90°就收到光源发光影响
        else
            return vec3(0, 0, 0);      //否则不会计如color计算
    }

    texture *emit;
};

#endif
