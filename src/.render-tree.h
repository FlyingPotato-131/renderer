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
	float rx, ry, rz;

	float &operator[](std::ptrdiff_t const i) noexcept
	{
	    return (&rx)[i];
	}
};

float max3(float a, float b, float c){
	return a > b && a > c ? a : b > c ? b : c;
}

float min3(float a, float b, float c){
	return a < b && a < c ? a : b < c ? b : c;
}

boundingbox getbox(triangle t){
	float maxx = max3(t.A.x, t.B.x, t.C.x);
	float minx = min3(t.A.x, t.B.x, t.C.x);

	float maxy = max3(t.A.y, t.B.y, t.C.y);
	float miny = min3(t.A.y, t.B.y, t.C.y);

	float maxz = max3(t.A.z, t.B.z, t.C.z);
	float minz = min3(t.A.z, t.B.z, t.C.z);

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

vec3 center(triangle t){
	return (t.A+t.B+t.C)/3;
}

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

boundingbox getbox(std::span<triangle> triangles){
	boundingbox box = getbox(triangles[0]);
	for(triangle t:triangles){
		box = box + getbox(t);
	}
	return box;
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

bool contains(vec3 point, boundingbox box){
	return box.center.x - box.rx <= point.x && point.x <= box.center.x + box.rx
		&& box.center.y - box.ry <= point.y && point.y <= box.center.y + box.ry
		&& box.center.z - box.rz <= point.z && point.z <= box.center.z + box.rz;
}


int numtri(boundingbox box, std::vector<triangle> triangles){
	int n = 0;
	for(triangle t:triangles){
		if(contains(center(t), box)){
			n++;
		}
	}
	return n;
}

bool lessx(triangle t1, triangle t2){
	return center(t1).x < center(t2).x;
}
bool lessy(triangle t1, triangle t2){
	return center(t1).y < center(t2).y;
}
bool lessz(triangle t1, triangle t2){
	return center(t1).z < center(t2).z;
}

void splitbox(boxnode *box, std::span<triangle> triangles){
	int maxcoord = box->box.rx >= box->box.ry && box->box.rx >= box->box.rz ? 0 : box->box.ry > box->box.rz ? 1 : 2;
//get median triangle
	auto less = [i = maxcoord](vec3 a, vec3 b){return a[i] < b[i];};
	std::nth_element(triangles.begin(), triangles.begin() + triangles.size()/2, triangles.end(), lessx);
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
	boundingbox left = getbox(triangles.first(triangles.size()/2));
	boundingbox right = getbox(triangles.last(triangles.size()/2));
//create nodes
	boxnode *leftnode = static_cast<boxnode *>(std::malloc(sizeof(boxnode)));
	leftnode -> box = left;

	boxnode *rightnode = static_cast<boxnode *>(std::malloc(sizeof(boxnode)));
	rightnode -> box = right;
// add nodes
	box -> next = boxnode::branches{leftnode, rightnode};
}

bool boxray(ray r, boundingbox box){
	if(contains(r.orig, box)){
		return true;
	}
	float x = (box.center.x - box.rx - r.orig.x)/r.drct.x;
	float X = (box.center.x + box.rx - r.orig.x)/r.drct.x;

	float y = (box.center.y - box.ry - r.orig.y)/r.drct.y;
	float Y = (box.center.y + box.ry - r.orig.y)/r.drct.y;

	float z = (box.center.z - box.rz - r.orig.z)/r.drct.z;
	float Z = (box.center.z + box.rz - r.orig.z)/r.drct.z;

	return X >= y && Y >= x
	    && Y >= z && Z >= y
	    && X >= z && Z >= x;
}

struct boxraystate{
	bool exists;
	float x, X, y, Y, z, Z;
};

boxraystate boxrayfull(ray r, boundingbox box){
	boxraystate state;

	float x = (box.center.x - box.rx - r.orig.x)/r.drct.x;
	float X = (box.center.x + box.rx - r.orig.x)/r.drct.x;

	float y = (box.center.y - box.ry - r.orig.y)/r.drct.y;
	float Y = (box.center.y + box.ry - r.orig.y)/r.drct.y;

	float z = (box.center.z - box.rz - r.orig.z)/r.drct.z;
	float Z = (box.center.z + box.rz - r.orig.z)/r.drct.z;

	return {X >= y && Y >= x
	    && Y >= z && Z >= y
	    && X >= z && Z >= x,
	    x, X, y, Y, z, Z};
}

using indexes = std::vector<int>;

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

void continuetree(boxtree box, std::span<triangle> triangles){
	if(triangles.size() == 1){
		box -> next = triangles[0];
	}else{
		splitbox(box, triangles);
		auto len = triangles.size();
		continuetree(std::get<boxnode::branches>(box->next).left, std::span(triangles.begin(), triangles.begin() + len / 2));
		continuetree(std::get<boxnode::branches>(box->next).right, std::span(triangles.begin() + len / 2, triangles.end()));
	}
}

intersection raytree(ray r, boxtree box){
	intersection sect;
	if(!boxray(r, box->box)){
		sect.exists = 0;
	}else if(holds_alternative<triangle>(box->next)){
		basicintersection trysect = basicintersect(r, std::get<triangle>(box->next));
		sect.exists = trysect.exists;
		sect.a = trysect.a;
		sect.b = trysect.b;
		sect.face = std::get<triangle>(box->next);
		sect.vertex = std::get<triangle>(box->next).C;
	}else{
		if(boxray(r, std::get<boxnode::branches>(box->next).left->box) && boxray(r, std::get<boxnode::branches>(box->next).right->box)){
			boxraystate leftstate = boxrayfull(r, std::get<boxnode::branches>(box->next).left->box);
			std::vector<float> leftcoords = {leftstate.x, leftstate.X, leftstate.y, leftstate.Y, leftstate.z, leftstate.Z};
			boxraystate rightstate = boxrayfull(r, std::get<boxnode::branches>(box->next).right->box);
			std::vector<float> rightcoords = {rightstate.x, rightstate.X, rightstate.y, rightstate.Y, rightstate.z, rightstate.Z};
			if(*min_element(leftcoords.begin(), leftcoords.end()) > *min_element(rightcoords.begin(), rightcoords.end())){
				sect = raytree(r, std::get<boxnode::branches>(box->next).left);
				if(!sect.exists){
					sect = raytree(r, std::get<boxnode::branches>(box->next).left);
				}
			}else{
				sect = raytree(r, std::get<boxnode::branches>(box->next).left);
				if(!sect.exists){
					sect = raytree(r, std::get<boxnode::branches>(box->next).right);
				}
			}
		}else if(boxray(r, std::get<boxnode::branches>(box->next).left->box)){
			sect = raytree(r, std::get<boxnode::branches>(box->next).left);
		}else if(boxray(r, std::get<boxnode::branches>(box->next).right->box)){
			sect = raytree(r, std::get<boxnode::branches>(box->next).left);
		}
	}
	return sect;
}

boxtree generatetree(std::vector<triangle> triangles){
	boundingbox mainbox = getmodelbox();
	boxtree boxroot = static_cast<boxtree>(std::malloc(sizeof(boxnode)));
	// boxnode boxroot;
	boxroot->box = mainbox;
	continuetree(boxroot, std::span(triangles.begin(), triangles.end()));
	return boxroot;
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
