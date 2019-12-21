#ifndef METALH
#define METALH

#include "material.h"

extern vec3 random_in_unit_sphere();
extern vec3 light_origin;

class metal : public material
{
public:
	metal(const vec3 &alb, float f) : albedo(alb)
	{
		if (f < 1)
			fuzz = f;   
		else
			fuzz = 1;   
		refRate = vec3(0, 0, 0);
	}

	/* NEW */
	metal(const vec3 &alb, float f, vec3 v) : albedo(alb), refRate(v)
	{
		if (f < 1)
			fuzz = f;//越小越光滑——完美反射
		else
			fuzz = 1;//接近漫反射粗糙
	}
	virtual bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const
	{
		vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);   //同样的散射
		scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}

	vec3 albedo;
	float fuzz;

	vec3 refRate;
	vec3 getReflectColor(const ray &r_in, const hit_record &rec) const
	{
		vec3 in = unit_vector(rec.p - light_origin);    //考虑视角的phong反射   到光源线
		vec3 h_middle = -(in + r_in.direction()) / 2;    //交点到光源线与入射光线法向量
		float mulVal = dot(unit_vector(rec.normal), h_middle);   //夹角判断
		if (mulVal < 0)                  
			return vec3(0, 0, 0);
		vec3 result = refRate * pow(mulVal, light_s);  //锐角才有反射计算颜色
		return result;
	}
	int light_s = 1;
};

#endif