#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include "structs.h"

std::vector<triangle> Structs(const char * filename)
{
    
	std::vector<vec3> vertices;
	std::vector<vec3> normals;
	std::vector<vec2> textures;
	std::vector<triangle> triangles;

	vec3 v;
	vec3 vn;
	vec2 vt;
    

	std::string line;

	int d[3], e[3], f[3];


	std::fstream F(filename);
	while (std::getline(F, line))
	{
		//std::istringstream iss(line);
		const char * c = line.c_str();

		if (3 == sscanf(c, "v %f %f %f", &v.x, &v.y, &v.z ))
            		vertices.push_back(v);
		else if (3 == sscanf(c, "vn %f %f %f", &vn.x, &vn.y, &vn.z ))
			normals.push_back(vn);
		else if (2 == sscanf(c, "vt %f %f", &vt.x, &vt.y ))
			textures.push_back(vt);
		else if (9 == sscanf(c, "f %i/%i/%i %i/%i/%i %i/%i/%i", d, d + 1, d + 2, e, e + 1, e + 2, f, f + 1, f + 2))
		{
			vertex A = {vertices[d[0] - 1], textures[d[1] - 1], normals[d[2] - 1]};
			vertex B = {vertices[e[0] - 1], textures[e[1] - 1], normals[e[2] - 1]};
			vertex C = {vertices[f[0] - 1], textures[f[1] - 1], normals[f[2] - 1]};
            //std::cout << vertices[d[0] - 1].x << " " << vertices[e[0] - 1].x << " " << vertices[f[0] - 1].x << std::endl;
			triangles.push_back({A, B, C});
		}
	}
	return triangles;
}
