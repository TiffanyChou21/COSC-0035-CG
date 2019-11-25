#include <iostream>
#include <fstream>
using namespace std;
#include "float.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "random.h"
#include "math.h"

#define MAX_FLOAT 0x1.fffffep+127f
vec3 light_origin(0, 1, -1);
float light_s = 1.0;
vec3 reflect_rate(1, 1, 1);
vec3 light_color(0, 0, 1);

vec3 random_in_unit_sphere() {
    vec3 p;
    do {
        p = 2.0*vec3(random_double(), random_double(), random_double()) - vec3(1,1,1);
    } while (p.squared_length() >= 1.0);
    return p;
}
// -2,2,1
vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2*dot(v,n)*n;
}
class lambertian : public material {
    public:
        lambertian(const vec3& a) : albedo(a) {}
        virtual bool scatter(const ray& r_in, const hit_record& rec,
                             vec3& attenuation, ray& scattered) const {
            vec3 target = rec.p + rec.normal + random_in_unit_sphere();
            scattered = ray(rec.p, target-rec.p);
            attenuation = albedo;
            return true;
        }
        vec3 albedo;
};

class metal : public material {
    public:
        metal(const vec3& a) : albedo(a) {}
        virtual bool scatter(const ray& r_in, const hit_record& rec,
                             vec3& attenuation, ray& scattered) const {
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = ray(rec.p, reflected);
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }
        vec3 albedo;
};

vec3 color(const ray& r, hittable *world, int depth) {
    hit_record rec;
    if (world->hit(r, 0.001, MAX_FLOAT, rec)) {
        // return vec3(0, 0, 0);
        ray scattered;
        vec3 attenuation;
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation*color(scattered, world, depth+1);
        }
        else {
            return vec3(0,0,0);
        }
    }
    else {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5*(unit_direction.y() + 1.0);
        return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
    }
}

vec3 reflect_color(const ray& r,  hittable *world) {
    hit_record rec;
    if (world->hit(r, 0.001, MAX_FLOAT, rec)) {
        vec3 in = unit_vector(rec.p - light_origin);

        // ray check;
        // check.A = rec.p;
        // check.B = unit_vector(-in);
        // hit_record temp;
        // if(world->hit(check, 0.001, MAX_FLOAT, temp))
        //     return vec3(0, 0, 0);

        vec3 h_middle = -(in + r.direction()) / 2;
        float mulVal = dot(unit_vector(rec.normal), h_middle);
        if(mulVal < 0)
            return vec3(0, 0, 0);
        vec3 result = reflect_rate*pow(mulVal, light_s);
        return result*light_color;
    }
    return vec3(0, 0, 0);
}

int main() {
    ofstream out("8.ppm");
    int nx = 500;
    int ny = 500;
    int ns = 100;
    out << "P3\n" << nx << " " << ny << "\n255\n";

    hittable *list[4];
    list[0] = new sphere(vec3(0,0,-1), 0.5, new lambertian(vec3(0.3, 0.3, 0.3)));
    list[1] = new sphere(vec3(0,-100.5,-1), 100, new lambertian(vec3(0.3, 0.3, 0.3)));
    //list[1] = new sphere(vec3(1,0,-1), 0.5, new metal(vec3(0.8, 0.6, 0.2)));
    //list[2] = new sphere(vec3(-1,0,-1), 0.5, new metal(vec3(0.8, 0.8, 0.8)));
    hittable *world = new hittable_list(list,2);

    // camera cam(vec3(-2,2,1), vec3(0,0,-1), vec3(0,1,0), 90, float(nx)/float(ny));
    camera cam(vec3(0, 0, 0), vec3(0,0,-1), vec3(0,1,0), 90, float(nx)/float(ny));
    for (int j = ny-1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            vec3 col(0, 0, 0);
            for (int s = 0; s < ns; s++) {
                float u = float(i + random_double()) / float(nx);
                float v = float(j + random_double()) / float(ny);
                ray r = cam.get_ray(u, v);
                col += color(r, world, 0);
            }
            col /= float(ns);
            
            // reflect
            ray r = cam.get_ray(1.0*i/nx, 1.0*j/ny);
            vec3 col_temp = reflect_color(r, world);
            col += col_temp;


            col = vec3( sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

            if(col[2] > 1)
                col[2] = 1;
            if(col[1] > 1)
                col[1] = 1;
            if(col[0] > 1)
                col[0] = 1;

            int ir = int(255.99*col[0]);
            int ig = int(255.99*col[1]);
            int ib = int(255.99*col[2]);

            out << ir << " " << ig << " " << ib << "\n";        
        }
    }
    out.close();
}