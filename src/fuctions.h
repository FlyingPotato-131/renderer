#pragma once
#include "structs.h"

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
    return {-v.x, -v.y, -v.z};
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

//intersections

struct rayTriangleIntersection
{
    triangle triangle;
    bool flag;
};

inline bool happened(RayTriangleIntersection const &i)
{
    return i.p >= 0.f
        && i.q >= 0.f
        && i.p + i.q <= 1.f;
}

inline RayTriangleIntersection rayTriangleIntersection(Ray const &ray, Triangle const &triangle)
{
    vec3 const a = triangle.r1 - triangle.r0;
    vec3 const b = triangle.r2 - triangle.r0;
    vec3 const c = ray.origin - triangle.r0;
    vec3 const d = ray.direction;
    float const det0 = dot(-d, cross(a, b));
    float const det1 = dot( c, cross(a, b));
    float const det2 = dot(-d, cross(c, b));
    float const det3 = dot(-d, cross(a, c));
    return 0;
}
