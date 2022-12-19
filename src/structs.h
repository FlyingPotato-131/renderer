#pragma once
#include <iostream>

struct vec3{
	float x, y, z;
	float &operator[](std::ptrdiff_t const i) noexcept
    {
        return (&x)[i];
    }
};

struct vec2{
	float x, y;
};

struct ray{
	vec3 orig;
	vec3 drct;
};

struct vertex{
	vec3 pos;
	vec2 tex;
	vec3 norm;
};

struct triangle{
	vertex A, B, C;
};

struct plane{
	vec3 pnt;
	vec3 norm;
};

// struct color{
// 	unsigned char r, g, b;
// };

struct color{
	int r, g, b;
};

struct intersection{
	bool exists;
	triangle face;
	vec3 vertex;
	float a, b;
	float dist;
};

struct normal{
	vec3 norm;
	int index;
};

struct indextriangle{
	int A, B, C;
};
