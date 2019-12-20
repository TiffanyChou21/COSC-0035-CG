#ifndef PERLINH
#define PERLINH

#include "vec3.h"
#include "drand48.h" 


inline float perlin_interp(vec3 c[2][2][2], float u, float v, float w)//进行xyz三个方向的线性插值，主要是中间过渡值，使其自然
{
    float uu = u*u*(3 - 2 * u);
    float vv = v*v*(3 - 2 * v);
    float ww = w*w*(3 - 2 * w);

    float accum = 0;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            for (int k = 0; k < 2; k++)
            {
                vec3 weight_v(u - i, v - j, w - k);
                accum += (i*uu + (1 - i)*(1 - uu)) * (j*vv + (1 - j)*(1 - vv)) * (k*ww + (1 - k)*(1 - ww))*dot(c[i][j][k], weight_v);
            }
                
    return accum;
}

// 实现柏林噪声用于纹理
class perlin
{
public:
    float noise(const vec3& p) const    //随机生成噪声值
    {
        float u = p.x() - floor(p.x());
        float v = p.y() - floor(p.y());
        float w = p.z() - floor(p.z());

        int i = int(floor(p.x()));
        int j = int(floor(p.y()));
        int k = int(floor(p.z()));

        vec3 c[2][2][2];
        for (int di = 0; di < 2; di++)
            for (int dj = 0; dj < 2; dj++)
                for (int dk = 0; dk < 2; dk++)
                    c[di][dj][dk] = ranvec[perm_x[(i + di) & 255] ^ perm_y[(j + dj) & 255] ^ perm_z[(k + dk) & 255]];

        return perlin_interp(c, u, v, w);

    }

    float turb(const vec3& p, int depth = 7)const
    {//增强多重采样的效果，使明更明，暗更暗
        float accum = 0.0f;
        vec3 temp_p = p;
        float weight = 1.0f;
        for (int i = 0; i < depth; i++)
        {
            accum += weight * noise(temp_p);
            weight *= 0.5f;
            temp_p *= 2;
        }

        return fabs(accum);
    }

    static vec3* ranvec;
    static int* perm_x;
    static int* perm_y;
    static int* perm_z;
};


static vec3* perlin_generate()  //生成最终柏林噪声
{
    vec3* p = new vec3[256];
    for (int i = 0; i < 256; ++i)
    {//随机单位向量增强各个方向插值效果，多次
        p[i] = unit_vector(vec3(-1 + 2*drand48(), -1 + 2*drand48(), -1 + 2*drand48()));
    }
    return p;
}

void permute(int *p, int n)
{//随机排列生成的256个0-1
    for (int i = n - 1; i > 0; i--)
    {
        int target = int(drand48() * (i + 1));
        int tmp = p[i];
        p[i] = p[target];
        p[target] = tmp;
    }
    return;
}

static int* perlin_generate_perm()
{//重排色块生成
    int *p = new int[256];
    for (int i = 0; i < 256; i++)
    {
        p[i] = i;
    }

    permute(p, 256);
    return p;
}

//随机产生256个[0, 1]中间的数
vec3 *perlin::ranvec = perlin_generate();
int *perlin::perm_x = perlin_generate_perm();
int *perlin::perm_y = perlin_generate_perm();
int *perlin::perm_z = perlin_generate_perm();
#endif