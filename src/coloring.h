#pragma once
#include <string>
#include "structs.h"
#include "functions.h"
#include "render-tree.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

color background;
vec3 light;

struct texture{
	color *map;
	int w, h, n;
};

texture loadtexture(const char *texturefile){
	int ok, w, h, n;
	ok = stbi_info(texturefile, &w, &h, &n);
	unsigned char* image = stbi_load(texturefile, &w, &h, &n, STBI_rgb_alpha);
	return{reinterpret_cast<color *>(image), w, h, n};
}

color getcolor(int w, int h, texture colors){
	return colors.map[w * colors.w + h];
}

color shade(ray r, boxtree box, color pxcolor){
	intersection sect = raytree(r, box);
	if(!sect.exists){
		return background;
	}

	vec3 AC = sect.face.A.pos - sect.face.C.pos;
	vec3 BC = sect.face.B.pos - sect.face.C.pos;
	vec3 a = AC * sect.a;
	vec3 b = BC * sect.b;
	float lc = length(a + b);
	float lb = length(BC - a - b);
	float la = length(AC - a - b);

	vec3 sectpos = sect.vertex + a + b;

	vec3 norm = (sect.face.A.norm * la + sect.face.B.norm * lb + sect.face.C.norm * lc) / (la + lb + lc);
	ray shadow = {sectpos, normalize(light - sectpos)};

	intersection shadowray = raytree(shadow, box);

	float brightness = shadowray.exists ? 0 : std::max(0.f, dot(norm, sectpos - light));
	color lightcolor = {1, 1, 1};

	return(pxcolor * (lightcolor * brightness + background * 0.3));
}
