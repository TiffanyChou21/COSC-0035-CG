#ifndef PDFH
#define PDFH

#include "vec3.h"
#include "onb.h"
#include <math.h>

class pdf
{
public:
    virtual float value(const vec3& direction) const = 0;
    virtual vec3 generate() const = 0;
};


class cosine_pdf : public pdf
{
public:
    cosine_pdf(const vec3& w) { uvw.build_from_w(w); }
    virtual float value(const vec3& direction) const
    {
        float cosine = dot(unit_vector(direction), uvw.w());
        if (cosine > 0)
            return cosine / float(M_PI);
        else
            return 0;
    }

    virtual vec3 generate() const
    {
        return uvw.local(random_cosine_direction());
    }
    
    onb uvw;
};


#endif