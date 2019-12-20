#ifndef LAMBERTIANH
#define LAMBERTIANH

#include "material.h"
#include "vec3.h"
#include "onb.h"

#include "texture.h"

extern vec3 random_in_unit_sphere();

class lambertian : public material
{
public:
    lambertian(texture *alb) : albedo(alb)
    {
    }

    virtual float scattering_pdf(const ray &r_in, const hit_record &rec, const ray &scattered) const
    {
        float cosine = dot(rec.normal, unit_vector(scattered.direction()));
        if (cosine < 0)
            cosine = 0;
        //散射值的pdf
        return cosine / float(M_PI);
    }

    virtual bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered, float &pdf)
    {
        onb uvw;
        uvw.build_from_w(rec.normal);
        vec3 direction = uvw.local(random_cosine_direction());

        scattered = ray(rec.p, unit_vector(direction));
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        //求出该散射光的重要性pdf，固定为0.5 / float(M_PI);
        pdf = dot(uvw.w(), scattered.direction()) / float(M_PI);

        return true;
    }

    texture *albedo;
};

#endif
