#pragma once
#include "ray.h"
#include "random.h"
#define M_PI 3.14159265358979323846

vec3 random_in_unit_disk() {
	vec3 p;
	do {
		p = 2.0 * vec3(random(), random(), 0) - vec3(1, 0, 0);
	} while (dot(p, p) >= 1.0);
	return p;
}
class camera{
public:
	camera(vec3 lookfrom, vec3 lookat, vec3 vup, double vfov, double aspect, double aperture, double focus_dist) {
		lens_radius = aperture / 2;
		double theta = vfov * M_PI / 180;
		double half_height = tan(theta / 2);
		double half_width = aspect * half_height;
		origin = lookfrom;
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);
		lower_left_corner = origin - focus_dist * (half_width*u + half_height*v + w);
		horizontal = 2*half_width*u * focus_dist;
		vertical = 2*half_height*v * focus_dist;
	}
	ray get_ray(double s, double t) { 
		vec3 rd = lens_radius * random_in_unit_disk();
		vec3 offset = u * rd.x() + v * rd.y();
		return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset); 
	}
	vec3 origin;
	vec3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
	vec3 u, v, w;
	double lens_radius;
};

