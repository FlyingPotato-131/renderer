#include "structs.h"
#include "functions.h"
#include <iostream>

int main(){
	ray ray = {{0, 0, 0}, {0, 1, 0}};
	triangle t = {{0, 1, 1}, {1, 1, -1}, {-1, 1, -1}};
	basicintersection sect = basicintersect(ray, t);
	vec3 point = ray.drct * sect.dist;
	std::cout << point.x << " " << point.y << " " << point.z << " " << sect.exists <<  std::endl;
}
