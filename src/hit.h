#pragma once
#include "structs.h"
#include "functions.h"

#include <algorithm>
#include <optional>
#include <vector>

intersection raytree(ray const &ray, std::vector<triangle> triangles)
{
    if(triangles.size() == 0u)
        return intersection{.exists = 0};

    std::vector<basicintersection> intersections;
    for(auto const &s : triangles)
        intersections.push_back(basicintersect(ray, s));
    auto less = [](basicintersection sect1, basicintersection sect2){return sect1.dist < sect2.dist;};
    auto const it = std::min_element(intersections.begin(), intersections.end(), less);
    if(!it->exists)
        return intersection{.exists = 0};

    //float const t = it->tMin;
    auto const i = it - intersections.begin();

    return
    {
        .face = triangles[i],
        .a = intersections[i].a,
        .b = intersections[i].b,
        //.position = ray.point(t),
        //.normal = normalize(ray.point(t) - triangles[i].origin),
    };
}
