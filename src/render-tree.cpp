//#pragma once
#include "structs.h"
#include "functions.h"
#include <vector>
#include <variant>

std::vector<vec3> vertices;
std::vector<indextriangle> triangles;

struct boundingbox{
	vec3 center;
	float rx, ry, rz;

	float &operator[](std::ptrdiff_t const i) noexcept
	{
	    return (&rx)[i];
	}
};

boundingbox getbox(triangle t){
	float maxx = std::max(t.A.x, t.B.x, t.C.x);
	float minx = std::min(t.A.x, t.B.x, t.C.x);

	float maxy = std::max(t.A.y, t.B.y, t.C.y);
	float miny = std::min(t.A.y, t.B.y, t.C.y);

	float maxz = std::max(t.A.z, t.B.z, t.C.z);
	float minz = std::min(t.A.z, t.B.z, t.C.z);

	vec3 center = {(maxx + minx)/2, (maxy + miny)/2, (maxz + minz)/2};

	return {center, (maxx - minx)/2, (maxy - miny)/2, (maxz - minz)/2};
}

float max(std::vector<vec3> vct, int axis){
	unsigned int max = 0;
	
	for(unsigned int i = 1; i < vct.size(); i++){
		if(vct[i][axis] > vct[max][axis]){
			max = i;
		}
	}
	return vct[max][axis];
}

float min(std::vector<vec3> vct, int axis){
	unsigned int min = 0;
	
	for(unsigned int i = 1; i < vct.size(); i++){
		if(vct[i][axis] < vct[min][axis]){
			min = i;
		}
	}
	return vct[min][axis];
}

boundingbox getmodelbox(){
	float maxx = max(vertices, 0);
	float minx = min(vertices, 0);

	float maxy = max(vertices, 1);
	float miny = min(vertices, 1);

	float maxz = max(vertices, 2);
	float minz = min(vertices, 2);

	vec3 center = {(maxx + minx)/2, (maxy + miny)/2, (maxz + minz)/2};

	return {center, (maxx - minx)/2, (maxy - miny)/2, (maxz - minz)/2};
}

struct boxnode{
	struct branches{
		boxnode *left;
		boxnode *right;
	};
	boundingbox box;
	std::variant<branches, triangle> next;
};

using boxtree = boxnode *;

void splitbox(boxnode *box){
	int maxcoord = box->box.rx >= box->box.ry && box->box.rx >= box->box.rz ? 0 : box->box.ry > box->box.rz ? 1 : 2;
	//left box
	vec3 leftc = box->box.center;
	leftc[maxcoord] = box->box.center[maxcoord] - box->box[maxcoord] / 2;
	boundingbox left = box->box;
	left[maxcoord] = box->box[maxcoord] / 2;
	left.center = leftc;
	//right box
	vec3 rightc = box->box.center;
	rightc[maxcoord] = box->box.center[maxcoord] + box->box[maxcoord] / 2;
	boundingbox right = box->box;
	right[maxcoord] = box->box[maxcoord] / 2;
	right.center = rightc;
	//create nodes
	boxnode *leftnode = static_cast<boxnode *>(std::malloc(sizeof(boxnode)));
	leftnode -> box = left;

	boxnode *rightnode = static_cast<boxnode *>(std::malloc(sizeof(boxnode)));
	rightnode -> box = right;
	//add nodes
	box->next = branches{leftnode, rightnode};
}

bool contains(vec3 point, boundingbox box){
	return box.center.x - box.rx <= point.x && point.x <= box.center.x + box.rx
		&& box.center.y - box.ry <= point.y && point.y <= box.center.y + box.ry
		&& box.center.z - box.rz <= point.z && point.z <= box.center.z + box.rz;
}

intersection intersect(ray r){
	boundingbox box = getmodelbox();
	//triangle centers
	std::vector<vec3> centers;
	for(unsigned int i = 0; i < triangles.size(); i++){
		centers.push_back((vertices[triangles[i].A] + vertices[triangles[i].B] + vertices[triangles[i].C]) / 3);
	}
	
}
