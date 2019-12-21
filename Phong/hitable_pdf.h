#ifndef HITABLE_PDFH
#define HITABLE_PDFH

#include "vec3.h"
#include "pdf.h"
#include "hitable.h"

class hitable_pdf : public pdf   //物体pdf
{
public:
    hitable_pdf(hitable *p, const vec3 &origin) : ptr(p), o(origin) {}
    virtual float value(const vec3 &direction) const
    {
        return ptr->pdf_value(o, direction);   //找出方向概率分布
    }

    virtual vec3 generate() const
    {
        return ptr->random(o);   //随机生成一个符合概率分布的向量
    }

    vec3 o;
    hitable *ptr;
};

#endif