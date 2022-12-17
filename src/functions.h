#pragma once
#include "structs.h"
#include <cmath>
#include <iostream>

//3D vectors

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

//2D vectors

inline vec2 operator+(vec2 const &v1, vec3 const &v2)
{
    return
    {
        v1.x + v2.x,
        v1.y + v2.y
    };
}
inline vec2 operator-(vec2 const &v1, vec3 const &v2)
{
    return
    {
        v1.x - v2.x,
        v1.y - v2.y
    };
}
inline vec2 operator-(vec2 const &v)
{
    return {-v.x, -v.y};
}
inline vec2 operator*(vec2 const &v, float const f)
{
    return
    {
        v.x * f,
        v.y * f
    };
}
inline vec2 operator/(vec2 const &v, float const f)
{
    return
    {
        v.x / f,
        v.y / f
    };
}
inline float dot(vec2 const &v1, vec2 const &v2)
{
    return v1.x * v2.x
         + v1.y * v2.y;
}

inline float length(vec2 const &v)
{
    return std::sqrt(dot(v, v));
}

inline vec2 normalize(vec2 const &v)
{
    return v / length(v);
}

//colors

inline color operator*(color c, float f){
    return {u_char(c.r * f), u_char(c.g * f), u_char(c.b * f)};
}

//intersections

struct basicintersection{
	bool exists;
	float a, b, dist;
};

inline basicintersection basicintersect(ray const &ray, triangle const &triangle)
{
    vec3 const b = triangle.A.pos - triangle.C.pos;
    vec3 const a = triangle.B.pos - triangle.C.pos;
    vec3 const c = ray.orig - triangle.C.pos;
    vec3 const d = ray.drct;
    
    float const det0 = dot( d, cross(a, b));
    float const det1 = dot(-c, cross(a, b));
    float const deta = dot(-d, cross(c, a));
    float const detb = dot(-d, cross(b, c));

	float alpha = deta / det0;
	float beta = detb / det0;
	float dist = det1 / det0;

    bool const exists = alpha >= 0.f && beta >= 0.f && alpha + beta <= 1.f;
    return {exists, alpha, beta, dist};
}

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
            .orig = position,
            .drct = normalize(d),
        };
    }
};
