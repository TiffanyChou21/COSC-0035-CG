#ifndef CAMERAH
#define CAMERAH

#include "ray.h"
#include "drand48.h"

class camera
{
public:
    camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect)
    {   //camera的位置,看向的位置,镜头朝向的角度,视角范围,图片长宽比
        float theta = float(vfov * M_PI / 180); //视角范围θ
        float half_height = tan(theta / 2);
        float half_width = aspect * half_height;
        origin = lookfrom;
        w = unit_vector(lookfrom - lookat); //求出相机局部坐标正交基uvw
        u = unit_vector(cross(vup, w));
        v = cross(w, u);
        lower_left_corner = origin - half_width * u - half_height * v - w; //视平面
        horizontal = 2 * half_width * u;
        vertical = 2 * half_height * v;
    }

    ray get_ray(float s, float t) //相当于反向光线找路径计算颜色
    {
        return ray(origin,
                   lower_left_corner + s * horizontal + t * vertical - origin);
    }

    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
};

#endif