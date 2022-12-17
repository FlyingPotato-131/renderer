#pragma once
#include "structs.h"
#include "funcs.h"

color background;
vec3 light;
color objectcolor;

color color(ray r, boxtree box){
	intersection sect = raytree(r, box);
	if(!sect.exists){
		return background;
	}

	vec3 sectpos = sect.vertex + a + b;

	vec3 AC = sect.face.A - sect.face.C;
	vec3 BC = sect.face.B - sect.face.C;
	vec3 a = AC * sect.a;
	vec3 b = AB * sect.b;
	float lc = lenght(a + b);
	float lb = lenght(BC - a - b);
	float la = lenght(AC - a - b);

	vec3 norm = (sect.face.A.norm * la + sect.face.B.norm * lb + sect.face.C.norm * lc) / (la + lb + lc);
	ray shadow = {sectpos, normalize(sectpos - light)};

	intersection shadow = raytree(lightray, box);

	float brightness = shadow.exists ? 0 : std::max(0, dot(norm, sectpos - light));

	return(objectcolor * brightness);
}
