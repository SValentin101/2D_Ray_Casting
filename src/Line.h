#ifndef LINE_H_
#define LINE_H_

#include <SFML/Graphics.hpp>
#include <vector>
#include <stack>
#include "Point.h"

class Line
{
public:
    Line(sf::Vector2f A, sf::Vector2f B, sf::Color color, std::string fath = "none", std::string fath2 = "none", std::string fatherVar = "none") : Point_A(A, true, fath), Point_B(B, true, fath)
    {
        sf::VertexArray Line2(sf::Lines, 2);
        Line2[0].position = A;
        Line2[1].position = B;
        Line2[0].color = color;
        Line2[1].color = color;
        vertexLine = Line2;
        father = fath;
        father2 = fath2;
        fatherFigure = fatherVar;

        pointsInLine.push_back(Point_A);
        pointsInLine.push_back(Point_B);
    }

    sf::VertexArray vertexLine;
    std::string father;
    std::string father2;
    std::string fatherFigure;

    PointOnLine Point_A;
    PointOnLine Point_B;

    std::vector<PointOnLine> pointsInLine;
    std::stack<sf::Vector2f> stackOfIntersectPoints;

    void updateEndingPointLine(sf::Vector2f point)
    {
        vertexLine[1].position = point;
    }
};

#endif
