#ifndef DIELECTRICH
#define DIELECTRICH

#include "material.h"

class dielectric : public material    //����ʣ���͸���Ĵ���
{
public:
    dielectric(float ri):ref_idx(ri) {}  //������
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
        // ������ķ��߶��Ǵ�����ָ������߽���
        if (dot(r_in.direction(), rec.normal) > 0)
        {//���ߴ������ڲ��������ʱ��cos>0
            outward_normal = -rec.normal;
            ni_over_nt = ref_idx;
            /*ref_idx*/ 
            cosine = dot(r_in.direction(), rec.normal) / r_in.direction().length();
        }
        else
        {
            //���ⲿ�������ڲ���
            outward_normal = rec.normal;
            ni_over_nt = 1.0f / ref_idx;  //�����ǿ���������1
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

        if (drand48() < reflect_prob)  //ʹ��������������ս��
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