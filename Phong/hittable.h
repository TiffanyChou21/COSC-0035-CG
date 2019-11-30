#pragma once
#ifndef HITTABLEH
#define HITTABLEH

#include "ray.h"

class lambertian ;
class metal;

struct hit_record {
    float t;
    float u;   //uv用来存交到平面rect上的xy、xz、yz交点
    float v;
    vec3 p;
    vec3 normal;
    lambertian *lam_ptr;
    metal *met_ptr;
};

class hittable  {
    public:
        virtual bool hit(
            const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};

class flip_normals : public hittable {
//翻转法向量
    public:
        flip_normals(hittable *p) : ptr(p) {}

        virtual bool hit(
            const ray& r, float t_min, float t_max, hit_record& rec) const {

            if (ptr->hit(r, t_min, t_max, rec)) {
                rec.normal = -rec.normal;
                return true;
            }
            else
                return false;
        }
        hittable *ptr;
};
#endif
