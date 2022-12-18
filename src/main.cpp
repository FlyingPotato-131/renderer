#pragma once
#include "structs.h"
#include "functions.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
//#include <chrono>
//#include <random>
#include "parse.cpp"
#include "coloring.h"
#include "render-tree.h"




int main()
{
<<<<<<< HEAD
  //  std::string line;
  //  std::ifstream in("Home/renderer/src/CedCo - LiAs R17 Venom II Revamp_export.obj");
    Camera camera = 
    {
        .position = {0.f, 0.8f, -2.f},
        .at = {0.f, 0.f, 0.f},
        .up = {0.f, 1.f, 0.f},
        .aspectRatio = 1920 / 1200,
        .verticalFOV = 0.55f,
    };

    Structs("CedCo - LiAs R17 Venom II Revamp_export.obj");
    boxtree object = generatetree();

    for (int w = 0; w < 1920; w++)
    {
        for (int h = 0; h < 1200; h++)
        {
            intersection cross = raytree(camera.castRay((w-810)/1920, (h-600)/1200));
            color cl = color(cross);
            image.setPixel(w,h,sf::Color(cl.r, cl.g, cl.b, 255));
        }
    }

    

    sf::RenderWindow window(sf::VideoMode(1920, 1200), "My window");
    
 
    void sf::Texture::update(const Uint8 * pixels) 	
    sf::Texture texture;
    texture.create(w,h) ;
    texture.update(sf::Uint8* cross,100,100,0,0);


    sf::Sprite sprite(texture);
    //sprite.setTexture(t_rd);
    sprite.setPosition(400,400);


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            window.close();
        }

        window.clear(sf::Color:: Black);
        window.draw(sprite);
        window.display();
    }

=======
    sf::Window window(sf::VideoMode(1920, 1080), "render");
    
>>>>>>> 66f1079b299afac912ea15a8cf60f0d48576b90a
    return 0;
}
