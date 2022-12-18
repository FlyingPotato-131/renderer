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
    std::vector<normal> normals;
    std::vector<vec2> textures;
    std::vector<triangle> triangles;

	vec3 v;
    vec3 vn;
    vec2 vt;
    

	std::string line;

	int d[3], e[3], f[3];

    //f.open(filename);
    //std::fstream F(f);
    std::ifstream read;
    read.open(filename);
	while (std::getline(read, line, '\n'))
    {
		std::istringstream iss(line);
		const char * c = line.c_str();

		if ( 3 == sscanf(c, "v %f %f %f", &v.x, &v.y, &v.z )){
            vertices.push_back(v);
        }
        else if ( 3 == sscanf(c, "vn %f %f %f", &vn.x, &vn.y, &vn.z ))
            normals.push_back({vn});
        else if ( 2 == sscanf(c, "vt %f %f ", &vt.x, &vt.y ))
			textures.push_back(vt);
        else if ( 9 == sscanf(c, "f %i/%i/%i %i/%i/%i %i/%i/%i", d, d + 1, d + 2, e, e + 1, e + 2, f, f + 1, f + 2))
        {
            vertex A = {vertices[d[0]], textures[d[1]], normals[d[2]].norm};
            vertex B = {vertices[e[0]], textures[e[1]], normals[e[2]].norm};
            vertex C = {vertices[f[0]], textures[f[1]], normals[f[2]].norm};
            triangles.push_back({A, B, C});
        }  
	}
    return triangles;
}

int main(){
    std::vector<triangle> mesh = Structs("../src/CedCo - LiAs R17 Venom II Revamp_export.obj");
    std::cout << mesh[1].A.pos.x << std::endl;
}