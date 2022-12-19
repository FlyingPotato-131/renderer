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
    for(auto const &s : triangles){
        basicintersection sect = basicintersect(ray, s);
        if(sect.exists){
            intersections.push_back(sect);
        }
    }
    auto less = [](basicintersection sect1, basicintersection sect2){return sect1.dist < sect2.dist;};
    if(intersections.size() == 0)
        return intersection{.exists = 0};
    auto const it = std::min_element(intersections.begin(), intersections.end(), less);
    //std::cout << it->exists;
    if(!it->exists)
        return intersection{.exists = 0};

    //float const t = it->tMin;
    auto const i = it - intersections.begin();

    return
    {
        .exists = 1,
        .face = triangles[i],
        .a = intersections[i].a,
        .b = intersections[i].b,
        .dist = intersections[i].dist,
        //.position = ray.point(t),
        //.normal = normalize(ray.point(t) - triangles[i].origin),
    };
}
