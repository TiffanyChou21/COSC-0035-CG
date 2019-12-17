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
        //�������
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        float ni_over_nt;
        attenuation = vec3(1.0, 1.0, 1.0);
        vec3 refracted;
        float reflect_prob;
        float cosine;

        //���ڵ���������˵�����۴��ĸ��Ƕ�����ײ�����߶��Ǵ�����ָ����ײ�㣨�ݲ����ǰ뾶Ϊ�������������
        //���ߴ��ⲿ�۲�ʱ��dot(r_in.direction(), rec.normal)<0
        //���������ڲ�������ʱdot(r_in.direction(), rec.normal)>0
        if (dot(r_in.direction(), rec.normal) > 0)
        {
            outward_normal = -rec.normal;
            ni_over_nt = ref_idx;
            cosine = /*ref_idx **/ dot(r_in.direction(), rec.normal) / r_in.direction().length();
        }
        else
        {
            //���ⲿ�������ڲ���
            outward_normal = rec.normal;
            ni_over_nt = 1.0f / ref_idx;
            cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
        }

        if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted))
        {
            //����Ǽнǵ�cosineֵ
            reflect_prob = schlick(cosine, ref_idx);
            scattered = ray(rec.p, refracted);
        }
        else
        {
            //ȫ����
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