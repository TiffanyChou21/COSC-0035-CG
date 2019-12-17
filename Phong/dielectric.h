#ifndef DIELECTRICH
#define DIELECTRICH

#include "material.h"

class dielectric : public material
{
public:
    dielectric(float ri):ref_idx(ri) {}
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const 
    {
        vec3 outward_normal;
        //反射光线
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        float ni_over_nt;
        attenuation = vec3(1.0, 1.0, 1.0);
        vec3 refracted;
        float reflect_prob;
        float cosine;

        //对于单个球体来说，无论从哪个角度来碰撞，法线都是从球心指向碰撞点（暂不考虑半径为负的情况），当
        //光线从外部观察时，dot(r_in.direction(), rec.normal)<0
        //当从球体内部往外射时dot(r_in.direction(), rec.normal)>0
        if (dot(r_in.direction(), rec.normal) > 0)
        {
            outward_normal = -rec.normal;
            ni_over_nt = ref_idx;
            cosine = /*ref_idx **/ dot(r_in.direction(), rec.normal) / r_in.direction().length();
        }
        else
        {
            //从外部往球体内部射
            outward_normal = rec.normal;
            ni_over_nt = 1.0f / ref_idx;
            cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
        }

        if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted))
        {
            //入射角夹角的cosine值
            reflect_prob = schlick(cosine, ref_idx);
            scattered = ray(rec.p, refracted);
        }
        else
        {
            //全反射
            scattered = ray(rec.p, reflected);
            reflect_prob = 1.0;
        }

        if (drand48() < reflect_prob)
        {
            scattered = ray(rec.p, reflected);
        }
        else
        {
            scattered = ray(rec.p, refracted);
        }

        return true;
    }

    float ref_idx;
};


#endif