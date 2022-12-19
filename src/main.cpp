#include "structs.h"
#include <vector>
#include <span>
#include "functions.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
//#include "Window.hpp"
//#include "Graphics.hpp"
//#include "System.hpp"
//#include <chrono>
//#include <random>
#include "parse.h"
#include "coloring.h"
//#include "render-tree.h"
//#include "hit.h"

void printtree(boxtree box){
    std::cout << box->box.center.x << " " << box->box.center.y << " " << box->box.center.z << std::endl;
    if(std::holds_alternative<triangle>(box->next)){
        triangle t = std::get<triangle>(box->next);
        std::cout << center(t).x << " " << center(t).y << " " << center(t).z << std::endl;
    }else{
        printtree(std::get<boxnode::branches>(box->next).left);
        printtree(std::get<boxnode::branches>(box->next).right);
    }
}

int main()
{
  //  std::string line;
  //  std::ifstream in("Home/renderer/src/CedCo - LiAs R17 Venom II Revamp_export.obj");
    Camera camera = 
    {
        .position = {1.5f, 0.5f, 1.5f},
        .at = {0.f, 0.f, 0.f},
        .up = {0.f, 1.f, 0.f},
        .aspectRatio = 1.6,
        .verticalFOV = 0.55f
    };

    std::vector<triangle> mesh = Structs("../test-models/prop-plane/plane2.obj");
    texture colors = loadtexture("../test-models/prop-plane/texture.png");
    
    // std::vector<triangle> mesh = {
    //     {{{1, 1, 1}, {}, {}}, {{1, -1, 1}, {}, {}}, {{-1, -1, -1}, {}, {}}},
    //     {{{1, 2, 1}, {}, {}}, {{1, 0, 1}, {}, {}}, {{-1, 0, 0}, {}, {}}},

    // };
    //std::cout << mesh[35].A.pos.x << " " << mesh[35].B.pos.x << " " << mesh[35].C.pos.x << std::endl;
    //boundingbox box = getbox(std::span(mesh.begin(), mesh.end()));
    boxtree object = generatetree(mesh);
    std::cout << "generated tree" << std::endl;
    //printtree(object);


    sf::RenderWindow window(sf::VideoMode(1920, 1080), "My window");
    
    //void sf::Texture::update(const Uint8 * pixels) 	
    //sf::Texture texture;
    //texture.create(w,h) ;
    //texture.update(sf::Uint8* cross,100,100,0,0);


    //sf::Sprite sprite(texture);
    //sprite.setTexture(t_rd);
    //sprite.setPosition(400,400);

    sf::Uint8* pixels = new sf::Uint8[1920*1080*4];

    sf::Texture image;
    image.create(1920, 1080);
    //window.draw(image);
    //sf::Sprite sprite(image);

    sf::Sprite sprite(image);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed){
                window.close();
                //deletetree(object);
            }
        }

        for (int h = 0; h < 1080; h++)
        {
            for (int w = 0; w < 1920; w++)
            {
                //color pxcolor = {u_char(239), u_char(113), u_char(169)};
                //vec3 norm = sect.face.C.norm * (1 - sect.a - sect.b) + sect.face.B.norm * sect.b + sect.face.A.norm * sect.a;
                float x = (float(w) - 960) / 960;
                float y = -(float(h) - 540) / 540;
                float dx = 0.5 / 1920.f;
                float dy = 0.5 / 1080.f;
                color cl = (
                    shade(camera.castRay(x - dx, y - dy), object, colors) * 0.25f
                  + shade(camera.castRay(x + dx, y - dy), object, colors) * 0.25f
                  + shade(camera.castRay(x - dx, y + dy), object, colors) * 0.25f
                  + shade(camera.castRay(x + dx, y + dy), object, colors) * 0.25f
                );
                //color cl = (shade(camera.castRay(x, y), object, colors));
                pixels[(h * 1920 + w) * 4] = cl.r;
                pixels[(h * 1920 + w) * 4 + 1] = cl.g;
                pixels[(h * 1920 + w) * 4 + 2] = cl.b;
                pixels[(h * 1920 + w) * 4 + 3] = 255;
            }
        }
        image.update(pixels);
        //std::cout << "frame" << std::endl;


        window.clear(sf::Color:: Black);
        window.draw(sprite);
        window.display();
    }

    //sf::Window window(sf::VideoMode(1920, 1080), "render");
    //return 0;
}
