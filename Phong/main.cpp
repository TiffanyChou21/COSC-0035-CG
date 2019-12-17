#include <fstream>
#include "float.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "random.h"
#include "math.h"
#include "model.h"
#include <limits.h>
#include "rectangle.h"
#include <time.h>

#define MAX_FLOAT 0x1.fffffep+127f
vec3 light_origin(0, 1, 0);
float light_s = 1.0;
vec3 reflect_rate(1, 1, 1);
vec3 light_color(1, 1, 1);

float leftWallX = -1;
float rightWallX = 1;

vec3 random_in_unit_sphere() {
	vec3 p;
	do {
		p = 2.0*vec3(random_double(), random_double(), random_double()) - vec3(1, 1, 1);
	} while (p.squared_length() >= 1.0);
	return p;
}
// -2,2,1
vec3 reflect(const vec3& v, const vec3& n) {
	return v - 2 * dot(v, n)*n;
}
class lambertian {
public:
	lambertian(const vec3& a) : albedo(a) {}
	bool scatter(const ray& r_in, const hit_record& rec,
		vec3& attenuation, ray& scattered) const {
		vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		scattered = ray(rec.p, target - rec.p);
		attenuation = albedo;
		return true;
	}
	vec3 albedo;
};

class metal {
public:
	metal(const vec3& a) : refRate(a) {}
	vec3 getReflectColor(const ray& r_in, const hit_record& rec) const {
		vec3 in = unit_vector(rec.p - light_origin);
		vec3 h_middle = -(in + r_in.direction()) / 2;
		float mulVal = dot(unit_vector(rec.normal), h_middle);
		if (mulVal < 0)
			return vec3(0, 0, 0);
		vec3 result = refRate*pow(mulVal, light_s);
		return result*light_color;
	}
	//vec3 getReflectColor(const ray& r_in, hittable *world, const hit_record& rec) const {
	//	hit_record newRec;
	//	if (world->hit(r_in, 0.001, FLT_MAX, newRec)) {
	//		if (rec.p.x() == -1) {
	//			vec3 in = unit_vector(newRec.p - light_origin);
	//			vec3 h_middle = -(in + r_in.direction()) / 2;
	//			float mulVal = dot(unit_vector(newRec.normal), h_middle);
	//			if (mulVal < 0)
	//				return vec3(0, 0, 0);
	//			vec3 result = refRate*pow(mulVal, light_s);
	//			return result*vec3(0, 1, 0);
	//		}
	//		if (rec.p.x() == 1) {
	//			vec3 in = unit_vector(newRec.p - light_origin);
	//			vec3 h_middle = -(in + r_in.direction()) / 2;
	//			float mulVal = dot(unit_vector(newRec.normal), h_middle);
	//			if (mulVal < 0)
	//				return vec3(0, 0, 0);
	//			vec3 result = refRate*pow(mulVal, light_s);
	//			return result*vec3(1, 0, 0);
	//		}
	//	}
	//	vec3 in = unit_vector(rec.p - light_origin);
	//	vec3 h_middle = -(in + r_in.direction()) / 2;
	//	float mulVal = dot(unit_vector(rec.normal), h_middle);
	//	if (mulVal < 0)
	//		return vec3(0, 0, 0);
	//	vec3 result = refRate*pow(mulVal, light_s);
	//	return result*light_color;
	//}

	vec3 refRate;
};

vec3 color(const ray& r, hittable *world, int depth) {
	hit_record rec;
	if (world->hit(r, 0.001, FLT_MAX, rec)) {

		if (depth > 0 && rec.p.x() == leftWallX)
			return vec3(0, 1, 0);
		if (depth > 0 && rec.p.x() == rightWallX)
			return vec3(1, 0, 0);

		ray scattered;
		vec3 attenuation;
		if (depth < 5) {
			rec.lam_ptr->scatter(r, rec, attenuation, scattered);
			vec3 refColor = rec.met_ptr->getReflectColor(r, rec);
			//vec3 refColor = rec.met_ptr->getReflectColor(scattered, world, rec);
			return attenuation*color(scattered, world, depth + 1) + refColor;
		}
		else {
			return vec3(0, 0, 0);
		}
	}
	else {
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5*(unit_direction.y() + 1.0);
		return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
	}
}

int main() {
	time_t start = time(0);
	ofstream out("8.ppm");
	int nx = 300;
	int ny = 300;
	int ns = 5;
	out << "P3\n" << nx << " " << ny << "\n255\n";

	lambertian *red = new lambertian(vec3(0.65, 0.05, 0.05));
	lambertian *light = new lambertian(vec3(15, 15, 15));
	lambertian *green = new lambertian(vec3(0.05, 0.65, 0.05));
	lambertian *white = new lambertian(vec3(0.73, 0.73, 0.73));
	//    lambertian *green=new lambertian(vec3(0.12, 0.45, 0.15));
	hittable *list[7];
	list[0] = new sphere(
		vec3(0, 0.5, 0), 0.75,
		new lambertian(vec3(1, 1, 1)),
		new metal(vec3(0.0, 0.0, 0.0))
		);
	list[1] = new sphere(
		vec3(0, -0.5, 0), 0.1,
		new lambertian(vec3(0.8, 0.8, 0.8)),
		new metal(vec3(0.2, 0.2, 0.2))
		);
	list[2] = new yz_rect(-1, 1, -1, 1, 1, red);
	list[3] = new flip_normals(new yz_rect(-1, 1, -1, 1, -1, green));
	list[4] = new xz_rect(-1, 1, -1, 1, 1, white);
	list[5] = new flip_normals(new xz_rect(-1, 1, -1, 1, -1, white));
	list[6] = new flip_normals(new xy_rect(-1, 1, -1, 1, -1, white));

	// 此处需设置好光源位置，左边墙的x坐标，右边墙的x坐标
	light_origin = vec3(0, 0, 1);
	leftWallX = -1;
	rightWallX = 1;

	//obj world

	//    model* m = new model(
	//                    new lambertian(vec3(0.1, 0.1, 0.1)),
	//                    new metal(vec3(0.7, 0.7, 0.7))
	//                );
	//    m->readFile("objFile/cube1.obj");
	//    list[0]=new yz_rect(m->y_min,2*m->y_max,2*m->z_min,10*m->z_max,3*m->x_max,red);
	//    list[1]=new flip_normals(new yz_rect(m->y_min,2*m->y_max,2*m->z_min,10*m->z_max,3*m->x_min,green));
	//    list[2]=new flip_normals(new xy_rect(m->x_min>0?-3*m->x_min:3*m->x_min,3*m->x_max,m->y_min,2*m->y_max,2*m->z_min,white));
	//    list[3]=new flip_normals(new xz_rect(3*m->x_min,3*m->x_max,2*m->z_min,10*m->z_max,m->y_min,white));
	//    list[4]=new xz_rect(3*m->x_min,3*m->x_max,2*m->z_min,10*m->z_max,2*m->y_max,white);
	//    list[5] = m;

	//obj world end
	hittable *world = new hittable_list(list, 6);
	camera cam(vec3(0, 0, 2), vec3(0, 0.01, 0), vec3(0, 0, 2), 90, float(nx) / float(ny));


	//        vec3 cam_origin=vec3(3*(m->x_max + m->x_min) / 2,2*(m->y_max + m->y_min) / 2,6*m->z_max);
	//    camera cam(
	//vec3(1.5 * m->x_min, 1.5*m->y_min, 1.5*m->z_min),
	//vec3(1.5 * m->x_max, 1.5*m->y_max, 1.5*m->z_max),
	//               vec3(0.4,0.5,0.1),
	//               cam_origin,

	//			vec3((m->x_max + m->x_min) / 2, (m->y_max + m->y_min) / 2, (m->z_max + m->z_min) / 2),
	//               vec3(3*(m->x_max + m->x_min) / 2,3*(m->y_max + m->y_min) / 2,6*m->z_max),
	//			90,
	//			float(nx)/float(ny)
	//		);

	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++) {
				float u = float(i + random_double()) / float(nx);
				float v = float(j + random_double()) / float(ny);
				ray r = cam.get_ray(u, v);
				col += color(r, world, 0);
			}
			col /= float(ns);

			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);

			out << ir << " " << ig << " " << ib << "\n";
		}
	}
	out.close();
	time_t end = time(0);
	cout << "Cost " << end - start << " s" << endl;
}
