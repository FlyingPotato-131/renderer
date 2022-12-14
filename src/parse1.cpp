#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
using namespace std;

struct vec3
{
	float x, y, z;
	float &operator[](std::ptrdiff_t const i) noexcept
    {
        return (&x)[i];
    }
};

struct vec2
{
	float x, y;
};

struct triangle
{
	vec3 A, B, C;
};

struct normal
{
	vec3 norm;
	int index;
};

struct indextriangle
{
	int A, B, C;
};

int main()
{
	std::vector<vec3> vertices(1);
    std::vector<normal> normals(1);
    std::vector<vec2> textures(1);
    std::vector<indextriangle> indextriangles(1);
    std::vector<triangle> triangles(1);

	vec3 addvec;
    vec2 addtex;
    normal addnorm;
    indextriangle addind;
    triangle addtri;

	string line;
	int kvec = 0;
    int knor = 0;
    int ktex = 0;
    int kind = 0;
    int ktri = 0;

    fstream F;
	F.open("CedCo - LiAs R17 Venom II Revamp_export.obj");

	while (getline(F, line)){
		istringstream iss(line);
		const char * c = line.c_str();

		if ( 3 == sscanf(c, "v %f %f %f", &vertices[kvec].x, &vertices[kvec].y, &vertices[kvec].z ))
        {
			vertices.push_back(addvec);
			kvec++;
		}
        else if ( 3 == sscanf(c, "vn %f %f %f", &normals[knor].norm.x, &normals[knor].norm.y, &normals[knor].norm.z ))
        {
            normals[knor].index = knor;
            normals.push_back(addnorm);
            knor++;
        }
        else if ( 2 == sscanf(c, "vt %f %f ", &textures[ktex].x, &textures[ktex].y ))
        {
			textures.push_back(addtex);
			ktex++;
		}
        else if ( 3 == sscanf(c, "f %i %*c %*i %*c %*i  %i %*c %*i %*c %*i  %i %*c %*i %*c %*i ", &indextriangles[kind].A, &indextriangles[kind].B, &indextriangles[kind].C))
        {
            indextriangles.push_back(addind);
            triangles.push_back(addtri);
            kind++;
        }
        
	}
    F.close();
   
    for (int i =0; i < indextriangles.size(); i++)
    {   
        {
            F.open("CedCo - LiAs R17 Venom II Revamp_export.obj");
            for (int j = 0; j < indextriangles[i].A - 1; j++)
            {
                getline(F, line);
            }
    
            getline(F, line);
            istringstream iss(line);
            const char * c = line.c_str();
            if ( 3 == sscanf(c, "v %E %E %E", &triangles[ktri].A.x, &triangles[ktri].A.y, &triangles[ktri].A.z ));
            F.close();
       
        }

        {
            F.open("CedCo - LiAs R17 Venom II Revamp_export.obj");
            for (int j = 0; j < indextriangles[i].B - 1; j++)
            {
                getline(F, line);
            }

            getline(F, line);
            istringstream iss(line);
            const char * c = line.c_str();
            if ( 3 == sscanf(c, "v %E %E %E", &triangles[ktri].B.x, &triangles[ktri].B.y, &triangles[ktri].B.z ));
            F.close();
         
        }

        {

            F.open("CedCo - LiAs R17 Venom II Revamp_export.obj");
            for (int j = 0; j < indextriangles[i].C - 1; j++)
            {
                getline(F, line);
            }

            getline(F, line);
            istringstream iss(line);
            const char * c = line.c_str();
            if ( 3 == sscanf(c, "v %E %E %E", &triangles[ktri].C.x, &triangles[ktri].C.y, &triangles[ktri].C.z ));
            F.close();
        
        }

        ktri++;

    }
   
}