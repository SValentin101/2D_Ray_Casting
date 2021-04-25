#ifndef POINT_H_
#define POINT_H_

#include <SFML/Graphics.hpp>

class PointOnLine
{
public:
    PointOnLine() = default;
    PointOnLine(sf::Vector2f Pos, bool valid_point, std::string origin_father, std::string second_father = "none")
    {
        Position = Pos;
        Valid = valid_point;
        father = origin_father;
        father2 = second_father;
    }

    sf::Vector2f Position;
    bool Valid;
    std::string father;
    std::string father2;
};

class PointOfIntersect
{
public:
    PointOfIntersect(sf::Vector2f A = sf::Vector2f(0,0))
    {
        circle.setRadius(4.5);
        circle.setOrigin(sf::Vector2f(4.5/2, 4.5/2));
        circle.setFillColor(sf::Color::Red);
        circle.setPosition(A);
    }
    sf::CircleShape circle;
};

#endif
