#include "structs.h"
#include "functions.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
//#include <chrono>
//#include <random>
#include "parse.h"
#include "coloring.h"
#include "render-tree.h"




int main()
{
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

    std::vector <triangle> mesh = Structs("CedCo - LiAs R17 Venom II Revamp_export.obj");
    boxtree object = generatetree(mesh);  

    sf::RenderWindow window(sf::VideoMode(1920, 1080), "My window");
    
    //void sf::Texture::update(const Uint8 * pixels) 	
    //sf::Texture texture;
    //texture.create(w,h) ;
    //texture.update(sf::Uint8* cross,100,100,0,0);


    //sf::Sprite sprite(texture);
    //sprite.setTexture(t_rd);
    //sprite.setPosition(400,400);


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            window.close();
        }

        sf::Uint8* pixels = new sf::Uint8[1920*1080*4];

        sf::Texture image;
        image.create(1920, 1080);
        sf::Sprite sprite(image);

        for (int h = 0; h < 1080; h++)
        {
            for (int w = 0; w < 1920; w++)
            {
                color pxcolor = {u_char(239), u_char(113), u_char(169)};
                color cl = shade(camera.castRay((w - 810) / 1920, (h - 600) / 1080), object, pxcolor);
                pixels[(h * 1920 + w) * 4] = cl.r;
                pixels[(h * 1920 + w) * 4 + 1] = cl.g;
                pixels[(h * 1920 + w) * 4 + 2] = cl.b;
                pixels[(h * 1920 + w) * 4 + 3] = 1;
            }
        }

        image.update(pixels);

        window.clear(sf::Color:: Black);
        //window.draw(sprite);
        window.display();
    }

    //sf::Window window(sf::VideoMode(1920, 1080), "render");
    return 0;
}
