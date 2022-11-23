#pragma once
#include <cmath>

struct vec3{
	float x, y, z;
};

struct vec2{
	float x, y;
};

struct ray{
	vec3 orig;
	vec3 drct;

	vec3 point(float const t) const {return origin + direction * t;}
};

struct triangle{
	vec3 a, b, c;
};

struct plane{
	vec3 pnt;
	vec3 norm;
};

struct color{
	int r, g, b;
};

struct intersection{
	vec3 point;
	int index;
};

struct normal{
	vec3 norm;
	int index;
};

struct indextriangle{
	int a, b, c;
};

struct Camera
{
    vec3 position;
    vec3 at;
    vec3 up;
    float aspectRatio;
    float verticalFOV;

    // u, v ∈ [-1;1]
    ray castRay(float const u, float const v) const
    {
        vec3 const z = normalize(position - at);
        vec3 const x = normalize(cross(up, z));
        vec3 const y = cross(z, x);

        vec3 const d = x * (u * verticalFOV * aspectRatio)
                     + y * (v * verticalFOV)
                     - z;
        return
        {
            .origin = position,
            .direction = normalize(d),
        };
    }
};
