#ifndef SPHEREH
#define SPHEREH

#include "hitable.h"
#include "material.h"
#include "aabb.h"

void get_sphere_uv(const vec3& p, float& u, float& v)
{//根据交点求纹理坐标的函数,具体推导见报告，这里公式不好敲_(:з」∠)_
    float phi = atan2(p.z(), p.x());
    float theta = asin(p.y());
    u = float(1 - (phi + M_PI) / (2 * M_PI));
    v = float((theta + M_PI / 2) / M_PI);
}

class sphere : public hitable
{
public:
    sphere(){}
    sphere(vec3 cen, float r, material* mat) : center(cen), radius(r), material_ptr(mat){};
    virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const
    {

        vec3 oc = r.origin() - center;
        float a = dot(r.direction(), r.direction());
        float b = dot(oc, r.direction());
        float c = dot(oc, oc) - radius*radius;
        float discriminant = b*b - a*c;
        if (discriminant > 0)
        {
            //先看近根
            float temp = (-b - sqrt(b*b - a*c)) / a;
            if (temp < tmax && temp>tmin)
            {
                rec.t = temp;
                rec.p = r.point_at_parameter(rec.t);
                rec.normal = (rec.p - center) / radius;
                rec.mat_ptr = material_ptr;
                vec3 pNormalize = (rec.p - center) / radius;
                get_sphere_uv(pNormalize, rec.u, rec.v);
                return true;
            }
            temp = (-b + sqrt(b*b - a*c)) / a;
            if (temp<tmax && temp>tmin)
            {
                rec.t = temp;
                rec.p = r.point_at_parameter(rec.t);
                rec.normal = (rec.p - center) / radius;
                rec.mat_ptr = material_ptr;
                vec3 pNormalize = (rec.p - center) / radius;
                get_sphere_uv(pNormalize, rec.u, rec.v);
                return true;
            }
        }

        return false;
    }

    bool bounding_box(float t0, float t1, aabb& box) const
    {
        box = aabb(center - vec3(radius, radius, radius), center + vec3(radius, radius, radius));
        return true;
    }


    vec3 center;
    float radius;
    material* material_ptr;
};

#endif