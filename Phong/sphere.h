#ifndef SPHEREH
#define SPHEREH

#include "hittable.h"
class lambertian;
class metal;
class sphere: public hittable  {
    public:
        sphere() {}
        sphere(vec3 cen, float r, lambertian *a, metal *b)
            : center(cen), radius(r), lam_ptr(a), met_ptr(b)  {};
        virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
            vec3 center;
        float radius;
        lambertian *lam_ptr; /* NEW */
        metal *met_ptr;
};
bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius*radius;
    float discriminant = b*b - a*c;
    if (discriminant > 0) {
        float temp = (-b - sqrt(discriminant))/a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.lam_ptr = lam_ptr; /* NEW */
            rec.met_ptr = met_ptr;
            return true;
        }
        temp = (-b + sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.lam_ptr = lam_ptr; /* NEW */
            rec.met_ptr = met_ptr;
            return true;
        }
    }
    return false;
}


#endif
