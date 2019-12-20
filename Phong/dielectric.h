#ifndef DIELECTRICH
#define DIELECTRICH

#include "material.h"

class dielectric : public material    //电解质，即透明的处理
{
public:
    dielectric(float ri):ref_idx(ri) {}  //折射率
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
        // 单球体的法线都是从球心指向与光线交点
        if (dot(r_in.direction(), rec.normal) > 0)
        {//光线从球体内部向外射的时候即cos>0
            outward_normal = -rec.normal;
            ni_over_nt = ref_idx;
            /*ref_idx*/ 
            cosine = dot(r_in.direction(), rec.normal) / r_in.direction().length();
        }
        else
        {
            //从外部往球体内部射
            outward_normal = rec.normal;
            ni_over_nt = 1.0f / ref_idx;  //外面是空气所以是1
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

        if (drand48() < reflect_prob)  //使用随机数叠加最终结果
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