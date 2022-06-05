#include <iostream>
#include <fstream>
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"
#include "material.h"
using namespace std;


hitable* random_scene() {
	int n = 500;
	hitable **list = new hitable*[n + 1];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			double choose_mat = random();
			vec3 center(a + 0.9 * random(), 0.2, b + 0.9 * random());
			if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
				if (choose_mat < 0.8) {
					list[i++] = new sphere(center, 0.2, new lambertian(vec3(random() * random(), random() * random(), random() * random())));
				}
				else if (choose_mat < 0.95) {
					list[i++] = new sphere(center, 0.2, new metal(vec3(0.5 * (1 + random()), 0.5 * (1 + random()), 0.5 * (1 + random())), 0.5*random()));
				}
				else {
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}

	list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
	list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));
	return new hitable_list(list, i);
}
vec3 color(const ray& r, hitable *world, int depth) {
	hit_record rec;
	if (world->hit(r, 0.0001, FLT_MAX, rec)) {
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			return attenuation * color(scattered, world, depth + 1);
		}
		else {
			return vec3(0, 0, 0);
		}
	}
	else {
		vec3 unit_direction = unit_vector(r.direction());
		double t = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}

}
//writes a ppm file that outputs a rainbow of colors
int main() {
	ofstream file;
	string directory = R"(C:\Users\Elijah Jefferson\Documents\output.ppm)";
	file.open(directory);
	int nx = 200;
	int ny = 100;
	int ns = 100;
	if (file.is_open()) {
		file << "P3\n" << nx << " " << ny << "\n255\n";

		hitable* world = random_scene();
		vec3 lookfrom(13, 2, 3);
		vec3 lookat(0, 0, 0);
		//double dist_to_focus = (lookfrom - lookat).length();
		double dist_to_focus = 10.0;
		double aperture = 0.1;
		camera cam(lookfrom, lookat, vec3(0,1,0), 20, double(nx)/double(ny), aperture, dist_to_focus);
		for (int j = ny - 1; j >= 0; j--) {
			for (int i = 0; i < nx; i++) {
				vec3 col(0, 0, 0);
				for (int s = 0; s < ns; s++) {
					double u = double(i + random()) / double(nx);
					double v = double(j + random()) / double(ny);
					ray r = cam.get_ray(u, v);
					vec3 p = r.point_at_parameter(2.0);
					col += color(r, world, 0);
				}
				col /= double(ns);
				col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
				int ir = int(255.99 * col[0]);
				int ig = int(255.99 * col[1]);
				int ib = int(255.99 * col[2]);
				file << ir << " " << ig << " " << ib << "\n";
			}
		}
		file.close();
	}
	return 0;
}