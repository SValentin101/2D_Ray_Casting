#include "handleEvents.h"
#include "utilities.h"
#include <iostream>

void handleEvents(sf::RenderWindow& window, sf::Event& event, std::vector<Figure>& vecOfFigures, bool& lightOn2,  std::vector<sf::Text> vecText,
        sf::RectangleShape& smallRec, sf::RectangleShape& smallRec2, bool& extraLigth, bool& showLight)
{
    float pi = 3.141592;
    sf::Vector2f MousePos2;

    if (event.type == sf::Event::Closed)
        window.close();
    if (event.type == event.MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        MousePos2 = getMousePos(window);
        // Incluye nuevas figuras en base a si se ha hecho click sobre sus nombres.
        for (auto text : vecText)
            if (text.getGlobalBounds().contains(MousePos2))
            {
                if (text.getString() == "Square")   // Su texto esta en 1050,200
                    vecOfFigures.emplace_back("Cuadrado", sf::Vector2f(1550, 160), sf::Vector2f(1625, 160), sf::Vector2f(1625, 235), sf::Vector2f(1550, 235));
                else if (text.getString() == "Rectangle")
                    vecOfFigures.emplace_back("Rectangulo", sf::Vector2f(1525, 160), sf::Vector2f(1675, 160), sf::Vector2f(1675, 240), sf::Vector2f(1525, 240));
                else if (text.getString() == "Triangle")
                    vecOfFigures.emplace_back("Triangulo", sf::Vector2f(1600, 150), sf::Vector2f(1675, 235), sf::Vector2f(1525, 235));
                else if (text.getString() == "Trapezium")
                    vecOfFigures.emplace_back("Trapecio", sf::Vector2f(1525, 150), sf::Vector2f(1650, 150), sf::Vector2f(1700, 240), sf::Vector2f(1475, 240));
                else if (text.getString() == "Clear All")
                    vecOfFigures.clear();
            }
        if (smallRec.getGlobalBounds().contains(MousePos2) && extraLigth == false)
        {
            smallRec.setFillColor(sf::Color(0, 158, 128));
            extraLigth = true;
        }
        else if (smallRec.getGlobalBounds().contains(MousePos2) && extraLigth == true)
        {
            smallRec.setFillColor(sf::Color::White);
            extraLigth = false;
        }

        if (smallRec2.getGlobalBounds().contains(MousePos2) && showLight == false)
        {
            smallRec2.setFillColor(sf::Color(0, 158, 128));
            showLight = true;
        }
        else if (smallRec2.getGlobalBounds().contains(MousePos2) && showLight == true)
        {
            smallRec2.setFillColor(sf::Color::White);
            showLight = false;
        }

    }

    if (event.type == event.MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
    {
        lightOn2 = false;
        for (auto& Figure : vecOfFigures)
        {
             if (contains(Figure.vecLinesFigure, MousePos2))
             {
                 Figure.clickOverFigure = true;
                 Figure.mousePosOnClick = MousePos2;
             }
        }
    }

    if (event.type == event.MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
    {
        lightOn2 = true;
        for (auto& Figure : vecOfFigures)
        {
            Figure.clickOverFigure = false;
            Figure.spinFigure = false;
            Figure.resizeFigure = false;
            Figure.angleOfSpin = 0;
            Figure.ratioOfResize = 0;
        }
    }

    if (event.type == sf::Event::KeyPressed) // Solo mientras haya click sobre el objeto se permitirá el giro.
    {
        lightOn2 = false;
        for (auto iter = vecOfFigures.begin(); iter != vecOfFigures.end(); iter++)
        {
            if (iter->clickOverFigure)
            {
                iter->spinFigure = true;
                iter->resizeFigure = false;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                    iter->angleOfSpin = -pi/20;
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                    iter->angleOfSpin = +pi/20;
                else
                    iter->angleOfSpin = 0;
            }
        }
    }

    if (event.type == sf::Event::MouseWheelMoved) // Solo mientras haya click sobre el objeto se permitirá el resize
    {
        lightOn2 = false;
        for (auto iter = vecOfFigures.begin(); iter != vecOfFigures.end(); iter++)
        {
            if (iter->clickOverFigure)
            {
                iter->resizeFigure = true;
                iter->spinFigure = false;
                 if (event.mouseWheel.delta == 1)
                     iter->ratioOfResize = 1.20;
                 else if (event.mouseWheel.delta == -1)
                     iter->ratioOfResize = 0.80;
                 else
                     iter->ratioOfResize = 0;
            }
        }
    }
}
