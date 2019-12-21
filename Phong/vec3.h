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
    vec3() {}
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

    inline const vec3 &operator+() const { return *this; }
    inline vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    inline float operator[](int i) const { return e[i]; }
    inline float &operator[](int i) { return e[i]; }

    inline vec3 &operator+=(const vec3 &v2)
    {
        e[0] += v2.e[0];
        e[1] += v2.e[1];
        e[2] += v2.e[2];
        return *this;
    }

    inline vec3 &operator-=(const vec3 &v2)
    {
        e[0] -= v2.e[0];
        e[1] -= v2.e[1];
        e[2] -= v2.e[2];
        return *this;
    }

    inline vec3 &operator*=(const vec3 &v2)
    {
        e[0] *= v2.e[0];
        e[1] *= v2.e[1];
        e[2] *= v2.e[2];
        return *this;
    }

    inline vec3 &operator/=(const vec3 &v2)
    {
        e[0] /= v2.e[0];
        e[1] /= v2.e[1];
        e[2] /= v2.e[2];
        return *this;
    }

    inline vec3 &operator*=(const float t)
    {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    inline vec3 &operator/=(const float t)
    {
        e[0] /= t;
        e[1] /= t;
        e[2] /= t;
        return *this;
    }

    inline float length() const { return std::sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]); }   //向量模长
    inline float squared_length() const { return (e[0] * e[0] + e[1] * e[1] + e[2] * e[2]); }

    inline void make_unit_vector()  //向量标准化
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

inline float dot(const vec3 &v1, const vec3 &v2)   //点乘
{
    return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

inline vec3 cross(const vec3 &v1, const vec3 &v2)  //叉乘
{
    return vec3(
        (v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1]),
        (-(v1.e[0] * v2.e[2] - v1.e[2] * v2.e[0])),
        (v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]));
}

inline vec3 unit_vector(vec3 v)   //该方向单位向量
{
    return v / v.length();
}

vec3 reflect(const vec3 &v, const vec3 &n) //不考虑视角的反射光线计算
{
    return v - 2 * dot(v, n) * n;
}

bool refract(const vec3 &v, const vec3 &n, float ni_over_nt, vec3 &refracted)
{                                                                        //折射计算Snell定律    ni_over_nt是下方介质相当于上方介质的折射率
    vec3 uv = unit_vector(v);                                            //入射光线单位化
    float dt = dot(uv, n);                                               //计算与法线的cosθ
    float discriminant = 1.0f - ni_over_nt * ni_over_nt * (1 - dt * dt); //根据公式推导出来的下方介质的cosθ'
    if (discriminant > 0)
    {
        refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant); //转换回折射光线
        return true;
    }
    return false;
}

float schlick(float cosine, float ref_idx) //Schlick简单方法，用以估计反射和折射的比例
{
    float r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

inline vec3 random_cosine_direction() //随机生成余弦分布中的一个方向向量
{
    float r1 = drand48();
    float r2 = drand48();
    float z = sqrt(1 - r2);              //单位向量
    float phi = 2.0f * float(M_PI) * r1; //生成φ
    float x = cos(phi) * 2 * sqrt(r2);   //根据公式计算
    float y = sin(phi) * 2 * sqrt(r2);
    return vec3(x, y, z);
}

#endif