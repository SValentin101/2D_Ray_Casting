#ifndef FIGURE_H_
#define FIGURE_H_

#include <SFML/Graphics.hpp>
#include <initializer_list>
#include "Line.h"

class Figure
{
public:
    Figure(std::string figureName, sf::Vector2f Point1, sf::Vector2f Point2, sf::Vector2f Point3 = sf::Vector2f(0,0),
            sf::Vector2f Point4 = sf::Vector2f(0,0), sf::Vector2f Point5 = sf::Vector2f(0,0), sf::Vector2f Point6 = sf::Vector2f(0,0));

    std::string nameFigure;
    std::vector<sf::Vector2f> vecPointsFigure;
    std::vector<Line> vecLinesFigure;
    std::vector<sf::Vertex> vecfigureVertexArray;
    sf::Vector2f centerPoint;
    sf::Vector2f mousePosOnClick;

    static int figureCount;

    bool clickOverFigure;
    bool spinFigure;
    bool resizeFigure;

    float angleOfSpin;
    float ratioOfResize;

    void createLinesForFigure();
    void createCenterPointFigure();

    void updateFigure();
};

void handleFigureMovement(std::vector<Figure>& vecOfFigures, sf::Vector2f MousePos);
void handleFigureSpin(std::vector<Figure>& vecOfFigures);
void handleFigureResize(std::vector<Figure>& vecOfFigures);

#endif
