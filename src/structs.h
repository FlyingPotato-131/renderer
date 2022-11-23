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

inline vec3 operator+(vec3 const &v1, vec3 const &v2)
{
    return
    {
        v1.x + v2.x,
        v1.y + v2.y,
        v1.z + v2.z,
    };
}
inline vec3 operator-(vec3 const &v1, vec3 const &v2)
{
    return
    {
        v1.x - v2.x,
        v1.y - v2.y,
        v1.z - v2.z,
    };
}
inline vec3 operator-(vec3 const &v)
{
    return {-v.x, -v.y, -v.z};
}
inline vec3 operator*(vec3 const &v, float const f)
{
    return
    {
        v.x * f,
        v.y * f,
        v.z * f,
    };
}
inline vec3 operator/(vec3 const &v, float const f)
{
    return
    {
        v.x / f,
        v.y / f,
        v.z / f,
    };
}
inline float dot(vec3 const &v1, vec3 const &v2)
{
    return v1.x * v2.x
         + v1.y * v2.y
         + v1.z * v2.z;
}
inline vec3 cross(vec3 const &v1, vec3 const &v2)
{
    return
    {
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x,
    };
}
inline float length(vec3 const &v)
{
    return std::sqrt(dot(v, v));
}
inline vec3 normalize(vec3 const &v)
{
    return v / length(v);
}
