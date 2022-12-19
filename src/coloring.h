#pragma once
#include <cmath>
#include <string>
#include "structs.h"
#include "functions.h"
#include "render-tree.h"
//#include "hit.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

color background = {246, 100, 151};
vec3 light = {-10, 10, 10};

struct texture{
	color *map;
	int w, h, n;
};

texture loadtexture(const char *texturefile){
	int w, h, n;
	//ok = stbi_info(texturefile, &w, &h, &n);
	//std::cout << w << " " << h << " " << n << std::endl;
	unsigned char* image = stbi_load(texturefile, &w, &h, &n, 0);
	// for(int i = 0; i < w * h * n; i++){
	// 	std::cout << int(image[i]) << " ";
	// }
	//std::cout << w << " " << h << " " << n << std::endl;
	color *map = static_cast<color *>(std::malloc(w * h * sizeof(color)));
	for(int i = 0; i < w * h; i++){
		map[i] = {
			image[3 * i],
			image[3 * i + 1],
			image[3 * i + 2]
		};
	}
	// for(int i = 0; i < w * h; i++){
	// 	std::cout << int(map[i].r) << " " << int(map[i].g) << " " << int(map[i].b) << std::endl;
	// }
	//std::cout << int(image[0]) << int(image[100000]);
	return{map, w, h, n};
	//return{reinterpret_cast<color *>(image), w, h, n};
}

color getcolor(int w, int h, texture colors){
	return colors.map[h * colors.w + w];
}

color shade(ray r, boxtree box, texture colors){
	intersection sect = raytree(r, box);
	//std::cout << sect.exists;
	if(!sect.exists){
		return background;
	}

	// vec3 AC = sect.face.A.pos - sect.face.C.pos;
	// vec3 BC = sect.face.B.pos - sect.face.C.pos;
	// vec3 a = AC * sect.a;
	// vec3 b = BC * sect.b;
	// float lc = length(a + b);
	// float lb = length(BC - a - b);
	// float la = length(AC - a - b);

	vec3 sectpos = r.orig + r.drct * sect.dist;
	float la = length(sectpos - sect.face.A.pos);
	float lb = length(sectpos - sect.face.B.pos);
	float lc = length(sectpos - sect.face.C.pos);
	//std::cout << sectpos.x << " " << sectpos.y << " " << sectpos.z << std::endl;

	//vec3 norm = (sect.face.A.norm * lb * lc + sect.face.B.norm * la * lc + sect.face.C.norm * la * lb) / (lb * lc + la * lc + la * lb);
	vec3 norm = sect.face.C.norm * (1 - sect.a - sect.b) + sect.face.B.norm * sect.b + sect.face.A.norm * sect.a;
	vec2 tex = sect.face.C.tex * (1 - sect.a - sect.b) + sect.face.B.tex * sect.b + sect.face.A.tex * sect.a;
	color pxcolor = getcolor(round((tex.x) * colors.w), round((1 - tex.y) * colors.h), colors);
	//std::cout << color.r << " ";
	//std::cout << tex.x << " " << tex.y << std::endl;
	//color pxcolor = {255, 255, 255};
	//std::cout << int(pxcolor.r) << " " << int(pxcolor.g) << " " << int(pxcolor.b) << std::endl;
	//std::cout << norm.x << " " << norm.y << " " << norm.z << std::endl;
	ray shadow = {sectpos, normalize(light - sectpos)};

	intersection shadowray = raytree(shadow, box);

	float brightness = shadowray.exists ? 0 : std::max(0.f, dot(normalize(norm), normalize(light - sectpos)));
	//float brightness = std::max(0.f, dot(normalize(norm), -normalize(sectpos - light)));
	//std::cout << brightness << " ";
	color lightcolor = {255, 255, 255};

	//color shade = (pxcolor * (lightcolor * brightness + background * 0.3));
	vec3 shade = {
		float(pxcolor.r) * (float(lightcolor.r) * brightness + float(background.r) * 0.3),
		float(pxcolor.g) * (float(lightcolor.g) * brightness + float(background.g) * 0.3),
		float(pxcolor.b) * (float(lightcolor.b) * brightness + float(background.b) * 0.3),
	};

	//std::cout << shade.x << " " << shade.y << " " << shade.z << std::endl;

	float const exposure = 0.3f;
	float const almost256 = 255.999f;

	return{
		u_char(almost256 * (1.f - std::exp(-exposure * (shade.x) / 256 / 64))),
		u_char(almost256 * (1.f - std::exp(-exposure * (shade.y) / 256 / 64))),
		u_char(almost256 * (1.f - std::exp(-exposure * (shade.z) / 256 / 64))),
	};
}
