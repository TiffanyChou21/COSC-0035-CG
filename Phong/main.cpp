
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

// ���������
// �������½�Ϊ����ԭ��(0, 0, 0)
// ���߷���Ϊz��������
// ��������Ϊx��������
// ��������Ϊy��������
// �������Ϊ�̶�555
void cornell_box(hitable **scene, camera **cam, float aspect)
{
	int i = 0;
	hitable **list = new hitable *[10];
	material *red = new lambertian(new constant_texture(vec3(0.65f, 0.05f, 0.05f)));
	material *white = new lambertian(new constant_texture(vec3(0.73f, 0.73f, 0.73f)));
	material *green = new lambertian(new constant_texture(vec3(0.12f, 0.45f, 0.15f)));
	material *light = new diffuse_light(new constant_texture(vec3(15, 15, 15)));
	// �����ɫǽ�� flip_normals���ڷ�ת������
	list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
	// �����ɫǽ
	list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
	// �컨���Դ
	fp = new flip_normals(new xz_rect(213, 343, 227, 332, 554, light));
	list[i++] = fp;
	// �컨��
	list[i++] = new flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
	// ����
	list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
	// ����
	list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));
	// ���������壬
	// list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18), vec3(130, 0, 65));
	// list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15), vec3(265, 0, 295));
	int xx, yy, nn;/*New*/
	// ����
	unsigned char *tex_data = stbi_load("earthmap.jpg", &xx, &yy, &nn, 0);/*New*/
	//material *emat =  new lambertian(new image_texture(tex_data, xx, yy));/*New*/
	// ��
	//list[i++] = new sphere(vec3(190, 190, 190), 90, emat);/*New*/
	//texture *pertext = new noise_texture(0.1);/*New*/
	//list[i++] =  new sphere(vec3(400, 210, 210),90, new lambertian( pertext));/*New*/
	//texture *checker = new checker_texture(new constant_texture(vec3(0.05, 0.05, 0.05)),new constant_texture(vec3(0.9, 0.9, 0.9)));/*New*/
	//list[i++] = new sphere(vec3(90, 90, 90),100, new lambertian(checker));

	// ���е�������������ֵ����Ϊ90
	// ���ݴ˱�������obj�ļ�
	model* obj = new model(white, 150);
	// �˴�����obj�ļ�·��
	// obj��ȡʱ�Ѿ��ʵ������ź�ƽ�ƣ�����Ӧ����������
	obj->readFile("./objFile/Arma.obj");
	// �����Ҫ���ʵ���ת
	// ����Ϊ��z����ʱ��
	list[i++] = new rotate_y(obj, 20);
	//list[i++] = new translate(new rotate_y(obj, 20), vec3(0, 0, 100));

    *scene = new hitable_list(list, i);	

	// �Ӹ�λ�ÿ�
	vec3 lookfrom(278, 278, -800);
	// ���λ�ÿ�
	vec3 lookat(278, 278, 0);
	// float dist_to_focus = 10.0f;
	// float aperture = 0.0f;
	float vfov = 40.0f;
	*cam = new camera(lookfrom, lookat, vec3(0, 1, 0), vfov, aspect);
}

// ������ɵ�λ����������������
vec3 random_in_unit_sphere()
{
    vec3 p;
    do 
    {
        p = 2.0f*vec3(float(drand48()), float(drand48()), float(drand48())) - vec3(1, 1, 1);
    } while (dot(p, p)>= 1.0);

    return p;
}

// ���ݹ���(����)r��ȡ��ɫ
vec3 color(const ray& r, hitable* world, int depth)
{
    hit_record rec;
	// �����䵽����������ʱ����¼���е�rec��Ϣ
	// rec�������ཻ��λ���Ѿ�׷�ٲ���t���ཻλ�÷����������ʣ��������Ϣ
    if (world->hit(r, 0.001f, FLT_MAX, rec))
    {
        ray scattered;
        vec3 attenuation;
		
		// ֻ�й�Դ���и�ֵ������Ϊvec3(0, 0, 0)
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
			// ����ǽ������ʣ������phong�����
			metal* judge = dynamic_cast<metal*>(rec.mat_ptr);
			if (judge) {
				refCol += judge->getReflectColor(r, rec);
			}
                      
            //�з����򽫵�ǰ�����巢���Ĺ��ɢ�����ɫԤ�Ե���
            float mpdf = rec.mat_ptr->scattering_pdf(r, rec, scattered);
            return refCol + emitted + attenuation*mpdf*color(scattered, world, depth + 1)/ pdf_val;
        }
        else
            return emitted; //�޷�����ֱ��ȡ�����������ɫ

    }

    return vec3(0.0f, 1.0f, 5.0f) / 255.999f;
}



int main()
{

    ofstream out("final.ppm");
    time_t start=time(0);  

    int nx = 600;
    int ny = 600;
    int ns = 10;

    out << "P3\n" << nx << " " << ny << "\n255\n";
    
    vec3 lower_left_corner(-2.0, -1.0, -1.0);
    vec3 horizontal(4.0, 0.0, 0.0);
    vec3 vertical(0.0, 2.0, 0.0);
    vec3 origin(0.0, 0.0, 0.0);
    

    hitable *world;
    camera *cam;
	// ��ʼ������
    cornell_box(&world, &cam, float(nx) / float(ny));

	// ���ڼ��㵱ǰ��ɱ���
    int allpix = nx*ny;
    int fa = 0;
	// ��ǰ�����
	int finishRate = 0;

	//for (int j = (ny - 1) / 2; j >= 0; j--)
    for (int j = ny - 1; j >= 0; j--)
    {
        for (int i = 0; i < nx; i++)
        {
			// ����ͼƬj�е�i�����ص���ɫ
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

			// ��ʾ��ɱ���
            fa++;
			if (fa % 3600 == 0) {
				finishRate++;
				cout << "[";
				int i;
				for (i = 0; i < finishRate / 2; i++)
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