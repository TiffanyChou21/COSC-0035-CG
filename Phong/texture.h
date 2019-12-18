#ifndef TEXTUREH
#define TEXTUREH

#include "vec3.h"
#include "perlin.h"

class texture
{
public:
    virtual vec3 value(float u, float v, const vec3& p) const = 0;
};

class constant_texture : public texture
{
public:
    constant_texture() {}
    constant_texture(vec3 c): color(c){}

    virtual vec3 value(float u, float v, const vec3& p) const
    {
        return color;
    }

    vec3 color;
};
class image_texture : public texture
{
public:
    image_texture(){}
    image_texture(unsigned char* pixels, int A, int B) : data(pixels), nx(A), ny(B)
    {

    }

    virtual vec3 value(float u, float v, const vec3 &p) const
    {
        int i = int((u)* nx);//Çó³öÏñËØË÷Òý
        int j = int((1 - v)*ny - 0.001f);
        if (i < 0) i = 0;
        if (j < 0) j = 0;
        if (i > nx - 1) i = nx - 1;
        if (j > ny - 1) j = ny - 1;
        float r = int(data[3 * i + 3 * nx*j]) / 255.0f;
        float g = int(data[3 * i + 3 * nx*j + 1]) / 255.0f;
        float b = int(data[3 * i + 3 * nx*j + 2]) / 255.0f;
        return vec3(r, g, b);
    }

    unsigned char* data;
    int nx, ny;

};
class noise_texture : public texture
{
public:
	noise_texture() {}
	noise_texture(float sc) : scale(sc) {}
	virtual vec3 value(float u, float v, const vec3& p) const
	{
		return vec3(1, 1, 1)*0.5*(1 + sin(scale*p.x() + 5 * noise.turb(scale*p)));
	}

	perlin noise;
	float scale;
};
class checker_texture : public texture
{
public:
    checker_texture(){}
    checker_texture(texture *t0, texture *t1): even(t0), odd(t1){}
 
    virtual vec3 value(float u, float v, const vec3& p) const
    {
        float sines = sin(5 * p.x())*sin(5 * p.y())*sin(5 * p.z());
        if (sines < 0)
            return odd->value(u, v, p);
        else
            return even->value(u, v, p);
    }
 
    texture* odd;
    texture* even;
};
#endif