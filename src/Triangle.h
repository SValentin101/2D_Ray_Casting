#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include <SFML/Graphics.hpp>

class Triangle
{
public:
    Triangle(sf::Vector2f A, sf::Vector2f B, sf::Vector2f C)
    {
        sf::VertexArray TriangleCons(sf::Triangles, 3);
        TriangleCons[0].position = A;
        TriangleCons[1].position = B;
        TriangleCons[2].position = C;

        sf::Color color = sf::Color::White;

        TriangleCons[0].color = color;
        TriangleCons[1].color = color;
        TriangleCons[2].color = color;

        MyTriangle = TriangleCons;
    }
    sf::VertexArray MyTriangle;
};



#endif
