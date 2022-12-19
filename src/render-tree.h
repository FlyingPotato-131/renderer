#pragma once
#include "structs.h"
#include "functions.h"
#include <vector>
#include <variant>
#include <algorithm>
#include <span>

std::vector<vec3> vertices;
std::vector<triangle> triangles;

struct boundingbox{
	vec3 center;
	float rx, ry, rz; // box by center and half sides

	float &operator[](std::ptrdiff_t const i) noexcept
	{
	    return (&rx)[i]; // get side on axis i (1, 2, 3) -> (x, y, z)
	}
};

//min and max of 3 floats
float max3(float a, float b, float c){
	return a > b && a > c ? a : b > c ? b : c;
}

float min3(float a, float b, float c){
	return a < b && a < c ? a : b < c ? b : c;
}

//bounding box of a triangle
boundingbox getbox(triangle t){
	float maxx = max3(t.A.pos.x, t.B.pos.x, t.C.pos.x);
	float minx = min3(t.A.pos.x, t.B.pos.x, t.C.pos.x);

	float maxy = max3(t.A.pos.y, t.B.pos.y, t.C.pos.y);
	float miny = min3(t.A.pos.y, t.B.pos.y, t.C.pos.y);

	float maxz = max3(t.A.pos.z, t.B.pos.z, t.C.pos.z);
	float minz = min3(t.A.pos.z, t.B.pos.z, t.C.pos.z);

	vec3 center = {(maxx + minx)/2, (maxy + miny)/2, (maxz + minz)/2};

	return {center, (maxx - minx)/2, (maxy - miny)/2, (maxz - minz)/2};
}

//min and max of std::vector
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

//get bounding box from global vertices std::vector
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

//get bounding box from std::vector of vertices
/*boundingbox getbox(std::vector<vec3> vertices){
	float maxx = max(vertices, 0);
	float minx = min(vertices, 0);

	float maxy = max(vertices, 1);
	float miny = min(vertices, 1);

	float maxz = max(vertices, 2);
	float minz = min(vertices, 2);

	vec3 center = {(maxx + minx)/2, (maxy + miny)/2, (maxz + minz)/2};

	return {center, (maxx - minx)/2, (maxy - miny)/2, (maxz - minz)/2};
}*/

//get triangle center
vec3 center(triangle t){
	return (t.A.pos+t.B.pos+t.C.pos)/3;
}

//add bounding boxes
boundingbox operator+(boundingbox b1, boundingbox b2){
	float maxx = std::max(b1.center.x + b1.rx, b2.center.x + b2.rx);
	float minx = std::min(b1.center.x - b1.rx, b2.center.x - b2.rx);

	float maxy = std::max(b1.center.y + b1.ry, b2.center.y + b2.ry);
	float miny = std::min(b1.center.y - b1.ry, b2.center.y - b2.ry);

	float maxz = std::max(b1.center.z + b1.rz, b2.center.z + b2.rz);
	float minz = std::min(b1.center.z - b1.rz, b2.center.z - b2.rz);

	vec3 center = {(maxx + minx)/2, (maxy + miny)/2, (maxz + minz)/2};

	return {center, (maxx - minx)/2, (maxy - miny)/2, (maxz - minz)/2};
}

//get box from std::span of triangles
boundingbox getbox(std::span<triangle> triangles){
	boundingbox box = getbox(triangles[0]);
	for(int i = 1; i < triangles.size(); i++){
		box = box + getbox(triangles[i]);
		//std::cout << box.center.x << " " << box.center.y << " " << box.center.z << std::endl;
	}
	//std::cout << box.center.x << " " << box.center.y << " " << box.center.z << std::endl;
	return box;
}

struct boxnode{
	struct branches{	//branches in boxnode for breaking recursion
		boxnode *left;
		boxnode *right;
	};
	boundingbox box;
	std::variant<branches, triangle> next; //child either pointers to 2 other boxnodes or triangle
};

using boxtree = boxnode *;
/*
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
	// add nodes
	box -> next = boxnode::branches{leftnode, rightnode};
}*/

//if vec3 is in box
bool contains(vec3 point, boundingbox box){
	return box.center.x - box.rx <= point.x && point.x <= box.center.x + box.rx
		&& box.center.y - box.ry <= point.y && point.y <= box.center.y + box.ry
		&& box.center.z - box.rz <= point.z && point.z <= box.center.z + box.rz;
}

//number of triangles from vector in box
int numtri(boundingbox box, std::vector<triangle> triangles){
	int n = 0;
	for(triangle t:triangles){
		if(contains(center(t), box)){
			n++;
		}
	}
	return n;
}

//compare triangles by center coords (deprecated)
bool lessx(triangle t1, triangle t2){
	return center(t1).x < center(t2).x;
}
bool lessy(triangle t1, triangle t2){
	return center(t1).y < center(t2).y;
}
bool lessz(triangle t1, triangle t2){
	return center(t1).z < center(t2).z;
}

//create boxnode * children with equal amount of triangles from boxnode * and span of triangles
void splitbox(boxnode *box, std::span<triangle> triangles){
	int maxcoord = box->box.rx >= box->box.ry && box->box.rx >= box->box.rz ? 0 : box->box.ry > box->box.rz ? 1 : 2;
//get median triangle
	auto less = [i = maxcoord](triangle a, triangle b){return center(a)[i] < center(b)[i];};
	std::nth_element(triangles.begin(), triangles.begin() + triangles.size()/2, triangles.end(), less);
//	float split = center(*(triangles.begin() + triangles.size()/2))[maxcoord];
//left box
/*	vec3 leftc = box->box.center;
	leftc[maxcoord] = (split + box->box.center[maxcoord] - box->box[maxcoord]) / 2;
	boundingbox left = box->box;
	left[maxcoord] = leftc[maxcoord] - box->box.center[maxcoord] + box->box[maxcoord];
	left.center = leftc;
//right box
	vec3 rightc = box->box.center;
	rightc[maxcoord] = (split + box->box.center[maxcoord] - box->box[maxcoord]) / 2;
	boundingbox right = box->box;
	right[maxcoord] = rightc[maxcoord] - box->box.center[maxcoord] + box->box[maxcoord];
	right.center = rightc;*/
//boxes
	boundingbox left = getbox(std::span(triangles.begin(), triangles.begin() + triangles.size()/2));
	boundingbox right = getbox(std::span(triangles.begin() + triangles.size()/2, triangles.end()));
//create nodes
	boxnode *leftnode = static_cast<boxnode *>(std::malloc(sizeof(boxnode)));
	leftnode->box = left;

	boxnode *rightnode = static_cast<boxnode *>(std::malloc(sizeof(boxnode)));
	rightnode->box = right;
// add nodes
	box->next = boxnode::branches{leftnode, rightnode};
}

//if ray intersects box
bool boxray(ray r, boundingbox box){
	if(contains(r.orig, box)){
		return true;
	}
	float x = (box.center.x - box.rx - r.orig.x)/r.drct.x;
	float X = (box.center.x + box.rx - r.orig.x)/r.drct.x;
	float xmin = std::min(x, X);
	float xmax = std::max(x, X);

	float y = (box.center.y - box.ry - r.orig.y)/r.drct.y;
	float Y = (box.center.y + box.ry - r.orig.y)/r.drct.y;
	float ymin = std::min(y, Y);
	float ymax = std::max(y, Y);

	float z = (box.center.z - box.rz - r.orig.z)/r.drct.z;
	float Z = (box.center.z + box.rz - r.orig.z)/r.drct.z;
	float zmin = std::min(z, Z);
	float zmax = std::max(z, Z);

	return xmax >= ymin && ymax >= xmin
	    && ymax >= zmin && zmax >= ymin
	    && xmax >= zmin && zmax >= xmin;
}

struct boxraystate{
	bool exists;
	float x, X, y, Y, z, Z;	//min and max values of t from coords if ray is r = r0 + at
};

//same as boxray but more info
boxraystate boxrayfull(ray r, boundingbox box){
	boxraystate state;

	float x = (box.center.x - box.rx - r.orig.x)/r.drct.x;
	float X = (box.center.x + box.rx - r.orig.x)/r.drct.x;
	float xmin = std::min(x, X);
	float xmax = std::max(x, X);

	float y = (box.center.y - box.ry - r.orig.y)/r.drct.y;
	float Y = (box.center.y + box.ry - r.orig.y)/r.drct.y;
	float ymin = std::min(y, Y);
	float ymax = std::max(y, Y);

	float z = (box.center.z - box.rz - r.orig.z)/r.drct.z;
	float Z = (box.center.z + box.rz - r.orig.z)/r.drct.z;
	float zmin = std::min(z, Z);
	float zmax = std::max(z, Z);

	return {xmax >= ymin && ymax >= xmin
	    && ymax >= zmin && zmax >= ymin
	    && xmax >= zmin && zmax >= xmin,
	    x, X, y, Y, z, Z};
}

//using indexes = std::vector<int>;

/*void createtree(boxtree box, indexes tri, std::vector<vec3> centers){
	if(tri.size() == 0){
		box -> next = boxtree::branches{nullptr, nullptr};
	}else if(tri.size() == 1){
		box -> next = centers[tri[0]];
	}else{
		splitbox(box);

		indexes leftind;
		indexes rightind;

		for(int i:tri){
			if(contains(centers[i], box->next.left->box)){
				leftind.push_back(i);
			}else{
				rightind.push_back(i);
			}
		}

		quickselect(triangles, mid, compare);

		createtree(box->next.left, leftind, centers);
		createtree(box->next.right, rightind, centers);
	}
}*/

//create children of boxnode * and span of triangles in said boxnode
void continuetree(boxtree box, std::span<triangle> triangles){
	if(triangles.size() == 1){
		box->next = triangles[0];
	}else{
		splitbox(box, triangles);
		auto len = triangles.size();
		continuetree(std::get<boxnode::branches>(box->next).left, std::span(triangles.begin(), triangles.begin() + len / 2));
		continuetree(std::get<boxnode::branches>(box->next).right, std::span(triangles.begin() + len / 2, triangles.end()));
	}
}

//intersect ray with boxtree
intersection raytree(ray r, boxtree box){
	intersection sect;
	if(!boxray(r, box->box)){	//if intersection doesn't exist
		sect.exists = 0;
	}else if(holds_alternative<triangle>(box->next)){	//if child is triangle
		basicintersection trysect = basicintersect(r, std::get<triangle>(box->next));
		sect.exists = trysect.exists;
		sect.a = trysect.a;
		sect.b = trysect.b;
		sect.face = std::get<triangle>(box->next);
		sect.vertex = std::get<triangle>(box->next).C.pos;
		sect.dist = trysect.dist;
	}else{	//if child is not triangle (obviously)
		if(boxray(r, std::get<boxnode::branches>(box->next).left->box) && boxray(r, std::get<boxnode::branches>(box->next).right->box)){	//if ray intersects both children
			// boxraystate leftstate = boxrayfull(r, std::get<boxnode::branches>(box->next).left->box);
			// float leftdist = max3(std::min(leftstate.x, leftstate.X), std::min(leftstate.y, leftstate.Y), std::min(leftstate.z, leftstate.Z));
			// //std::vector<float> leftcoords = {leftstate.x, leftstate.X, leftstate.y, leftstate.Y, leftstate.z, leftstate.Z};
			// boxraystate rightstate = boxrayfull(r, std::get<boxnode::branches>(box->next).right->box);
			// float rightdist = max3(std::min(rightstate.x, rightstate.X), std::min(rightstate.y, rightstate.Y), std::min(rightstate.z, rightstate.Z));
			// //std::vector<float> rightcoords = {rightstate.x, rightstate.X, rightstate.y, rightstate.Y, rightstate.z, rightstate.Z};

			// if(leftdist < rightdist){	//pick closest box for inspecting first
			// 	intersection sectleft = raytree(r, std::get<boxnode::branches>(box->next).left);
			// 	if(sectleft.exists){
			// 		sect = sectleft;
			// 	}else{
			// 		sect = raytree(r, std::get<boxnode::branches>(box->next).left);
			// 	}
			// }else{	//same code but reversed (if left is first)
			// 	intersection sectright = raytree(r, std::get<boxnode::branches>(box->next).right);
			// 	if(sectright.exists){
			// 		sect = sectright;
			// 	}else{
			// 		sect = raytree(r, std::get<boxnode::branches>(box->next).left);
			// 	}
			// }
			intersection sectleft = raytree(r, std::get<boxnode::branches>(box->next).left);
			intersection sectright = raytree(r, std::get<boxnode::branches>(box->next).right);
			if(sectleft.exists && sectright.exists){
				if(sectleft.dist < sectright.dist){
					sect = sectleft;
				}else{
					sect = sectright;
				}
			}else if(sectleft.exists){
				sect = sectleft;
			}else if(sectright.exists){
				sect = sectright;
			}else{
				sect.exists = 0;
			}
		}else if(boxray(r, std::get<boxnode::branches>(box->next).left->box)){	//if ray intersects only one child
			sect = raytree(r, std::get<boxnode::branches>(box->next).left);
			// if(!sect.exists){
			// 	sect = raytree(r, std::get<boxnode::branches>(box->next).right);
			// }
		}else if(boxray(r, std::get<boxnode::branches>(box->next).right->box)){
			sect = raytree(r, std::get<boxnode::branches>(box->next).right);
			// if(!sect.exists){
			// 	sect = raytree(r, std::get<boxnode::branches>(box->next).right);
			// }
		}else{
			sect.exists = 0;
		}
	}
	return sect;
}

//create tree using continuetree
boxtree generatetree(std::vector<triangle> triangles){
	std::cout << "generating tree" << std::endl;
	boundingbox mainbox = getbox(std::span(triangles.begin(), triangles.end()));
	boxtree boxroot = static_cast<boxtree>(std::malloc(sizeof(boxnode)));
	// boxnode boxroot;
	boxroot->box = mainbox;
	continuetree(boxroot, std::span(triangles.begin(), triangles.end()));
	return boxroot;
}

//delete tree
void deletetree(boxtree box){
	if(holds_alternative<boxnode::branches>(box->next)){
		deletetree(std::get<boxnode::branches>(box->next).right);
		deletetree(std::get<boxnode::branches>(box->next).left);
	}
	std::free(box);
}

/*int gettriangle(ray r, boxtree box, indexes tri){
	if(tri.size() == 1){
		return tri[0];
	}

}*/
/*
intersection intersect(ray r){
	boundingbox box = getmodelbox();
	//triangle centers
	std::vector<vec3> centers;
	for(unsigned int i = 0; i < triangles.size(); i++){
		centers.push_back((vertices[triangles[i].A] + vertices[triangles[i].B] + vertices[triangles[i].C]) / 3);
	}

}*/
