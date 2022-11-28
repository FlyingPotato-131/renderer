#pragma once
#include <cmath>

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

struct triangle{
	vec3 A, B, C;
};

struct plane{
	vec3 pnt;
	vec3 norm;
};

struct color{
	int r, g, b;
};

struct intersection{
	bool exists;
	int triangle;
	int vertex;
	float a, b;
};

struct normal{
	vec3 norm;
	int index;
};

struct indextriangle{
	int A, B, C;
};
