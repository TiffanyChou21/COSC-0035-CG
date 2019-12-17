#ifndef VEC3H
#define VEC3H

#include <math.h>
#include <stdlib.h>
#include <iostream>
#include "drand48.h"

// #define M_PI 3.1415926

class vec3
{
public:
    vec3(){}
    vec3(float e0, float e1, float e2)
    {
        e[0] = e0;
        e[1] = e1;
        e[2] = e2;
    }

    vec3(double e0, double e1, double e2)
    {
        e[0] = float(e0);
        e[1] = float(e1);
        e[2] = float(e2);
    }

    vec3(int e0, int e1, int e2)
    {
        e[0] = float(e0);
        e[1] = float(e1);
        e[2] = float(e2);
    }

    inline float x() const { return e[0]; }
    inline float y() const { return e[1]; }
    inline float z() const { return e[2]; }

    inline float r() const { return e[0]; }
    inline float g() const { return e[1]; }
    inline float b() const { return e[2]; }

    inline const vec3& operator+() const { return *this; }
    inline vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    inline float operator[](int i) const { return e[i]; }
    inline float& operator[](int i) { return e[i]; }

    inline vec3& operator+=(const vec3 &v2)
    {
        e[0] += v2.e[0];
        e[1] += v2.e[1];
        e[2] += v2.e[2];
        return *this;
    }

    inline vec3& operator-=(const vec3 &v2)
    {
        e[0] -= v2.e[0];
        e[1] -= v2.e[1];
        e[2] -= v2.e[2];
        return *this;
    }

    inline vec3& operator*=(const vec3 &v2)
    {
        e[0] *= v2.e[0];
        e[1] *= v2.e[1];
        e[2] *= v2.e[2];
        return *this;
    }

    inline vec3& operator/=(const vec3 &v2)
    {
        e[0] /= v2.e[0];
        e[1] /= v2.e[1];
        e[2] /= v2.e[2];
        return *this;
    }

    inline vec3& operator*=(const float t)
    {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    inline vec3& operator/=(const float t)
    {
        e[0] /= t;
        e[1] /= t;
        e[2] /= t;
        return *this;
    }

    inline float length() const { return std::sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);}
    inline float squared_length() const { return (e[0] * e[0] + e[1] * e[1] + e[2] * e[2]); }

    inline void make_unit_vector()
    {
        float len = length();
        e[0] /= len;
        e[1] /= len;
        e[2] /= len;
    }

    float e[3];        
};

inline vec3 operator+(const vec3 &v1, const vec3 &v2)
{
    return vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline vec3 operator-(const vec3 &v1, const vec3 &v2)
{
    return vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline vec3 operator*(const vec3 &v1, const vec3 &v2)
{
    return vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

inline vec3 operator*(const vec3 &v1, float t)
{
    return vec3(v1.e[0] * t, v1.e[1] * t, v1.e[2] * t);
}

inline vec3 operator*(float t, const vec3 &v1)
{
    return vec3(v1.e[0] * t, v1.e[1] * t, v1.e[2] * t);
}

inline vec3 operator/(const vec3 &v1, const vec3 &v2)
{
    return vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}

inline vec3 operator/(const vec3 &v1, float t)
{
    return vec3(v1.e[0] / t, v1.e[1] / t, v1.e[2] / t);
}

inline float dot(const vec3 &v1, const vec3 &v2)
{
    return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

inline vec3 cross(const vec3 &v1, const vec3 &v2)
{
    return vec3( 
        (v1.e[1]*v2.e[2]-v1.e[2]*v2.e[1]),
        (-(v1.e[0]*v2.e[2] - v1.e[2]*v2.e[0])),
        (v1.e[0]*v2.e[1] - v1.e[1]*v2.e[0])
    );
}

vec3 reflect(const vec3 &v, const vec3& n)
{
    return v - 2 * dot(v, n)*n;
}

inline vec3 unit_vector(vec3 v)
{
    return v / v.length();
}

bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted)
{
    vec3 uv = unit_vector(v);
    float dt = dot(uv, n);
    float discriminant = 1.0f - ni_over_nt*ni_over_nt*(1 - dt*dt);
    if (discriminant > 0)
    {
        refracted = ni_over_nt * (uv - n*dt) - n*sqrt(discriminant);
        return true;
    }

    //È«·´Éä
    return false;
}

float schlick(float cosine, float ref_idx)
{
    float r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

vec3 random_in_unit_disk()
{
    vec3 p;
    do
    {
        p = 2.0f*vec3(drand48(), drand48(), 0.0f) - vec3(1, 1, 0);
    } while (dot(p, p) >= 1.0);
    return p;
}

inline vec3 random_cosine_direction()
{
    float r1 = drand48();
    float r2 = drand48();
    float z = sqrt(1 - r2);
    float phi = 2.0f * float(M_PI)*r1;
    float x = cos(phi) * 2 * sqrt(r2);
    float y = sin(phi) * 2 * sqrt(r2);
    return vec3(x, y, z);
}

#endif