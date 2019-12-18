
#define _USE_MATH_DEFINES

#include <iostream>
#include <fstream>
using namespace std;

#include "vec3.h"
#include "ray.h"
#include "hitable_list.h"
#include "bvh_node.h"
#include "float.h"
#include "sphere.h"
#include "camera.h"
#include "drand48.h"

#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"
#include "perlin.h"
#include "diffuse_light.h"
#include "rect.h"
#include "box.h"
#include "model.h"
#include <time.h>

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include "flip_normals.h"
#include "translate.h"
#include "rotate.h"
// #include "constant_medium.h"
#include "pdf.h"
#include "hitable_pdf.h"
#include "mixture_pdf.h"

flip_normals* fp;

vec3 light_origin = vec3(0, 555, 0);

void cornell_box(hitable **scene, camera **cam, float aspect)
{
	int i = 0;
	hitable **list = new hitable *[10];
	material *red = new lambertian(new constant_texture(vec3(0.65f, 0.05f, 0.05f)));
	material *white = new lambertian(new constant_texture(vec3(0.73f, 0.73f, 0.73f)));
	material *green = new lambertian(new constant_texture(vec3(0.12f, 0.45f, 0.15f)));
	material *light = new diffuse_light(new constant_texture(vec3(15, 15, 15)));
	list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
	list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
	fp = new flip_normals(new xz_rect(213, 343, 227, 332, 554, light));
	list[i++] = fp;
	list[i++] = new flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
	list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
	list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));
	// list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18), vec3(130, 0, 65));
	// list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15), vec3(265, 0, 295));
	int xx, yy, nn;/*New*/
	unsigned char *tex_data = stbi_load("earthmap.jpg", &xx, &yy, &nn, 0);/*New*/
	//material *emat =  new lambertian(new image_texture(tex_data, xx, yy));/*New*/
	//list[i++] = new sphere(vec3(190, 190, 190), 90, emat);/*New*/
	//texture *pertext = new noise_texture(0.1);/*New*/
	//list[i++] =  new sphere(vec3(400, 210, 210),90, new lambertian( pertext));/*New*/
	//texture *checker = new checker_texture(new constant_texture(vec3(0.05, 0.05, 0.05)),new constant_texture(vec3(0.9, 0.9, 0.9)));/*New*/
	//list[i++] = new sphere(vec3(90, 90, 90),100, new lambertian(checker));

	// 所有点的三个坐标最大值设置为90
	// 根据此变量缩放obj文件
	model* obj = new model(white, 150);
	obj->readFile("./objFile/cube.obj");

	list[i++] = new rotate_y(obj, 15);

    *scene = new hitable_list(list, i);	

	vec3 lookfrom(278, 278, -800);
	vec3 lookat(278, 278, 0);
	// float dist_to_focus = 10.0f;
	// float aperture = 0.0f;
	float vfov = 40.0f;
	*cam = new camera(lookfrom, lookat, vec3(0, 1, 0), vfov, aspect);
}

vec3 random_in_unit_sphere()
{
    vec3 p;
    do 
    {
        p = 2.0f*vec3(float(drand48()), float(drand48()), float(drand48())) - vec3(1, 1, 1);
    } while (dot(p, p)>= 1.0);

    return p;
}


vec3 color(const ray& r, hitable* world, int depth)
{
    hit_record rec;
    if (world->hit(r, 0.001f, FLT_MAX, rec))
    {
        ray scattered;
        vec3 attenuation;
        vec3 emitted = rec.mat_ptr->emitted(r, rec, rec.u, rec.v, rec.p);

        float pdf_val;

        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered, pdf_val))
        {
            hitable_pdf p0(fp, rec.p);
            cosine_pdf p1(rec.normal);
            mixture_pdf p(&p0, &p1);
            // ray scattered = ray(hrec.p, p.generate(), r.time());/*New*/
            ray scattered = ray(rec.p, p.generate());/*New*/
            pdf_val = p.value(scattered.direction());

			vec3 refCol = vec3(0, 0, 0);
			// 如果是金属材质，则加上phong反射光
			metal* judge = dynamic_cast<metal*>(rec.mat_ptr);
			if (judge) {
				refCol += judge->getReflectColor(r, rec);
			}
                      
            //有反射则将当前发光体发出的光和散射的颜色预以叠加
            float mpdf = rec.mat_ptr->scattering_pdf(r, rec, scattered);
            return refCol + emitted + attenuation*mpdf*color(scattered, world, depth + 1)/ pdf_val;
        }
        else
            return emitted; //无反射则直接取发光射出的颜色

    }

    return vec3(0.0f, 1.0f, 5.0f) / 255.999f;
}



int main()
{

    ofstream out("final.ppm");
    time_t start=time(0);  

    int nx = 600;
    int ny = 600;
    int ns = 100;

    out << "P3\n" << nx << " " << ny << "\n255\n";
    
    vec3 lower_left_corner(-2.0, -1.0, -1.0);
    vec3 horizontal(4.0, 0.0, 0.0);
    vec3 vertical(0.0, 2.0, 0.0);
    vec3 origin(0.0, 0.0, 0.0);
    

    hitable *world;
    camera *cam;
    cornell_box(&world, &cam, float(nx) / float(ny));

    int allpix = nx*ny;
    int fa = 0;

	//for (int j = (ny - 1) / 2; j >= 0; j--)
    for (int j = ny - 1; j >= 0; j--)
    {
        for (int i = 0; i < nx; i++)
        {
            vec3 col(0, 0, 0);
            for (int s = 0; s < ns; s++)
            {
                float u = float(i + drand48()) / float(nx);
                float v = float(j + drand48()) / float(ny);
                ray r = cam->get_ray(u, v);

                col += color(r, world, 0);
            }
            
            col /= float(ns);


            if (col[0] > 1.0) col[0] = 1.0;
            if (col[1] > 1.0) col[1] = 1.0;
            if (col[2] > 1.0) col[2] = 1.0;

            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);
            out << ir << " " << ig << " " << ib << "\n";
            fa++;

			if (fa % 1000 == 0) {
				float finishRate = (fa*1.0f / allpix) * 100;
				cout << "[";
				int i;
				for (i = 0; i < int(finishRate) / 2; i++)
					cout << ">";
				while (i++ < 50)
					cout << "=";
				cout << "][" << (int)finishRate << "%]                  \r";
			}
        }
    }
    out.close();
    time_t end=time(0);
    cout << endl <<"Cost "<<end-start<<" s"<<endl;
}