#ifndef PDFH
#define PDFH

#include "vec3.h"
#include "onb.h"
#include <math.h>

class pdf //概率分布基类
{
public:
    virtual float value(const vec3 &direction) const = 0; //计算概率密度
    virtual vec3 generate() const = 0;                    //根据概率密度随机生成一个符合pdf的向量
};

class cosine_pdf : public pdf //余弦分布
{
public:
    cosine_pdf(const vec3 &w) { uvw.build_from_w(w); }
    virtual float value(const vec3 &direction) const
    {
        float cosine = dot(unit_vector(direction), uvw.w()); //方向和法线的标准正交基向量
        if (cosine > 0)                                      //只有成锐角的才是合适的方向
            return cosine / float(M_PI);
        else
            return 0;
    }

    virtual vec3 generate() const
    {
        return uvw.local(random_cosine_direction()); //将随机生成的余弦分布向量转到onb的标准正交基坐标系上
    }

    onb uvw;
};

#endif