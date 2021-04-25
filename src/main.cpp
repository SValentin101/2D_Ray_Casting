#include <SFML/Graphics.hpp>
#include <iostream>
#include <stack>
#include <algorithm>
#include "utilities.h"
#include "Figure.h"
#include "Point.h"
#include "Line.h"
#include "Triangle.h"
#include "handleEvents.h"

int Figure::figureCount = 0;

int main()
{
    const int WINDOW_HEIGHT = 1000;
    const int WINDOW_WIDTH = 1750;
    sf::ContextSettings settings;
    settings.antialiasingLevel = 10;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "2D Ray Casting", sf::Style::Close | sf::Style::Titlebar, settings);

    sf::Vector2f MousePos;
    sf::Vector2f MousePosClickObject;
    std::vector<Line> vecOfOriginalLines;
    std::vector<Line> vecLineasProyecEsquinas;
    std::vector<PointOfIntersect> vecPointsProyected;
    std::vector<Triangle> vecTriangulosLuz;
    std::vector<Line> vecSystemLines;

    std::vector<Figure> vecOfFigures;
    std::vector<sf::Text> vecOfTxt;

    // Reserva esta capacidad para los vectores me asegura que no habrá realloacation cada vez que vayan a aumentar.
    // Proporcionando una buena optimización.
    vecOfOriginalLines.reserve(500);
    vecLineasProyecEsquinas.reserve(500);
    vecPointsProyected.reserve(500);
    vecTriangulosLuz.reserve(500);
    vecOfFigures.reserve(50);

    bool LightOn = false;
    bool LightOn2 = false;
    bool ShowLight = false;
    bool showExtraLines = false;

    vecOfFigures.emplace_back("Cuadrado", sf::Vector2f(350, 450), sf::Vector2f(450, 450), sf::Vector2f(450, 550), sf::Vector2f(350, 550));
    vecOfFigures.emplace_back("Rectangulo", sf::Vector2f(700, 150), sf::Vector2f(900, 150), sf::Vector2f(900, 250), sf::Vector2f(700, 250));
    vecOfFigures.emplace_back("Triangulo", sf::Vector2f(500, 800), sf::Vector2f(750, 800), sf::Vector2f(750, 600));
    vecOfFigures.emplace_back("Trapecio", sf::Vector2f(300, 150), sf::Vector2f(400, 150), sf::Vector2f(500, 250), sf::Vector2f(200, 250));

    // Load txt's and set them accordingly.
    sf::Font font;
    font.loadFromFile("E:\\Programas\\Eclipse Work-space\\Ray_Casting_2\\Debug\\CALLIMBA.ttf");
    createTxt(vecOfTxt, font);

    // To hold the clickeable Recs, the big black and teal rec also.
    std::vector<sf::RectangleShape> vecRecs;
    fillRecsScreenAndClickeable(vecRecs);

    while (window.isOpen())
    {
        vecLineasProyecEsquinas.clear();
        vecPointsProyected.clear();
        vecTriangulosLuz.clear();

        MousePos = getMousePos(window);
        // Dibuja los triangulos de luz solo cuando el ratón esté en el área de las lineas del sistema. Top, bot, left, right.
        if (contains(vecSystemLines, MousePos))
            LightOn = true;
        else
            LightOn = false;

        // Lineas TOP, RIGHT... y el vec del sistema de esas lineas
        fillSystemLines(vecOfOriginalLines, vecSystemLines);

        // Para todas las figuras. Introduce sus lineas en el vecOfOriginalLines y haz el update de sus puntos y lineas de cada figura, ya que se iran alterando.
        for (auto& Figure : vecOfFigures)
        {
            Figure.updateFigure();
            // El insert es una operación lenta para un vector.
            for (auto& linesOfFigure : Figure.vecLinesFigure)
                vecOfOriginalLines.emplace_back(linesOfFigure);
        }

        sf::Event event;
        // Se encarga de los eventos. Cerrar pantalla, girar la figura, ampliar la figura, añadir una nueva figura en caso de hacer click sobre el texto
        if (window.pollEvent(event))
            handleEvents(window, event, vecOfFigures, LightOn2, vecOfTxt, vecRecs[0], vecRecs[1], showExtraLines, ShowLight);

        // Movimiento, spin y resize de las figuras
        handleFigureMovement(vecOfFigures, MousePos);
        handleFigureSpin(vecOfFigures);
        handleFigureResize(vecOfFigures);
        //

//       Creación y añadido de las lineas proyectadas desde las esquinas de la figura. Las lineas cyan.
//       Lo mas curioso es lo de los padres. Line_4 y Line_1 para los padres de la linea proyectada del primer punto, y el resto: Line_1 y Line_2 son sus padres.
        for (auto& Figure : vecOfFigures)
        {
            int num = 0;
            for (auto iter = Figure.vecPointsFigure.begin(); iter != Figure.vecPointsFigure.end(); iter++)
            {
                std::string nameProyectedLine = Figure.nameFigure + "." + "ProyectedLine" + ".";
                std::string numLine = std::to_string(num);
                std::string numLineNext = std::to_string(num + 1);
                std::string numMaxLine = std::to_string(Figure.vecPointsFigure.size());
                if (iter == Figure.vecPointsFigure.begin())
                    vecLineasProyecEsquinas.emplace_back(createLineProyectionFromCornerPoint(*iter, MousePos, nameProyectedLine + numMaxLine, nameProyectedLine + numLineNext));
                else
                    vecLineasProyecEsquinas.emplace_back(createLineProyectionFromCornerPoint(*iter, MousePos, nameProyectedLine + numLine, nameProyectedLine + numLineNext));
                num++;
            }
        }

        // Interseccion de las lineas proyectadas desde las esquinas de las figuras con el resto de lineas de los cuerpos
        for (auto& lineaDesdeEsquina : vecLineasProyecEsquinas)
        {
            sf::Vector2f intersectPos1(0,0);
            std::vector<sf::Vector2f> vecIntersectedPoints;
            vecIntersectedPoints.reserve(25);

            for (auto& lineasOriginales : vecOfOriginalLines)
            {
                    if(lineasOriginales.father != lineaDesdeEsquina.father && lineasOriginales.father != lineaDesdeEsquina.father2)
                    {
                        bool intersect1 = false;
                        intersecSegmentSegment(lineasOriginales, lineaDesdeEsquina, intersect1, intersectPos1);
                        if(intersect1)
                        {
                            PointOnLine IntersectedPoint(intersectPos1, true, lineasOriginales.father);
                            vecIntersectedPoints.push_back(intersectPos1);
                            lineasOriginales.pointsInLine.push_back(IntersectedPoint);
                        }
                    }
             }

            // Para esa linea proyectada teal, encuentra el punto más cercano al ratón, y sólo ese punto añadirás a los puntos rojos de corte
            // que luego vamos a dibujar. Además cambiaremos que el final de las lineas teal sean el primer punto de corte que tengan.
            sf::Vector2f nearestPoint(0,0);
            float dist = 9000;
            for (auto point : vecIntersectedPoints)
            {
                float disToPointFromMouse = module(point - MousePos);
                if (disToPointFromMouse < dist)
                {
                    dist = disToPointFromMouse;
                    nearestPoint = point;
                }
            }

            if (nearestPoint != sf::Vector2f(0,0))
            {
                createdIntersectPointsOnStack(lineaDesdeEsquina, nearestPoint, MousePos);
                lineaDesdeEsquina.updateEndingPointLine(nearestPoint);  // Cambiamos su punto final para que solo vaya desde la esquina de la fig hasta el p de corte.
                vecPointsProyected.push_back(nearestPoint);
            }
           }

            // Creame los triángulos de luz correspondientes y mételos en un vector.
            for (auto& originalLine : vecOfOriginalLines)
            {
                // Reorganizame los puntos del vector de puntos de la linea original en base a su distancia desde el punto A.
                // Debo primero reognaizar, sino, el pointsInLine.back() no será el punto B.
                std::sort(originalLine.pointsInLine.begin(), originalLine.pointsInLine.end(), [originalLine] (const PointOnLine& punto1, const PointOnLine& punto2)
                {return module(punto1.Position - originalLine.pointsInLine.front().Position) <= module(punto2.Position - originalLine.pointsInLine.front().Position); });

                    // Ahora es cuando voy a crear los triángulos de luz con todos los puntos válidos. A, B y todos los intermedios.
                    // Cómo saber qué puntos coger: Un punto intermedio entre esos dos puntos que se consideran para la base del triángulo, si trazo una linea
                    // desde ese punto intermedio al raton...
                    // ¿Corta a alguna linea de las originales (ignorando parentesco)? Si corta entonces esos dos puntos no valen para hacer la base.
                    // Si no corta es que esos dos puntos sí sirven para formar parte de la base del triángulo.
                    for (auto iter = originalLine.pointsInLine.begin(); iter != (originalLine.pointsInLine.end() - 1); iter++)
                    {
                        auto iterforSecondPoint = iter+1;
                        if (iter->Valid && iterforSecondPoint->Valid)
                        {
                            sf::Vector2f middlePoint(0,0);
                            middlePoint.x = (iter->Position.x + iterforSecondPoint->Position.x)/2;
                            middlePoint.y = (iter->Position.y + iterforSecondPoint->Position.y)/2;
                            Line LineaPuntoIntermedioRaton(middlePoint, MousePos, sf::Color::Green, originalLine.father);
                            // La linea al punto intermedio no debe cortar a NINGUNA de las lineas originales. (Excluyendo a la propia en la que está el punto)
                            // La nueva linea del punto intermedio debe excluirse del checkeo de corte con la linea sobre la que está el punto.
                            // Antes con que no cortase a una ya era suficiente, aunque no estuviese mirando a la linea que a mi me importaba. Y
                            // ya creaba el elemento en el vec de luz al no cortar ese linea del punto intermedio a una linea alejada.
                            // Pero no puede cortar a ni una sola, solo entonces serán puntos válidos para formar la base.
                            bool alMenosUnaInterseccion = false;
                            for (auto& lineasOriginales : vecOfOriginalLines)
                            {
                                if (lineasOriginales.father != LineaPuntoIntermedioRaton.father)
                                {
                                    bool intersect1 = false;
                                    sf::Vector2f intersectPos1;
                                    intersecSegmentSegment(lineasOriginales, LineaPuntoIntermedioRaton, intersect1, intersectPos1);
                                    // Ese punto y su siguiente, sí son válidos para formar base de triángulo
                                    if(intersect1 == true)
                                    {
                                        alMenosUnaInterseccion = true;
                                        break;
                                    }
                                }
                            }
                            if(alMenosUnaInterseccion == false)
                                vecTriangulosLuz.emplace_back(iter->Position, MousePos, iterforSecondPoint->Position);
                        }
                }
              }
        window.clear(sf::Color(0, 128, 128));

        for (auto rec : vecRecs)
            window.draw(rec);

        for (auto txt : vecOfTxt)
            window.draw(txt);

        for (auto originalLine : vecOfOriginalLines)
            window.draw(originalLine.vertexLine);

        // Así es como se colorea/dibuja (tbn los puntos individuales), un vector que contiene varios sf::Vertex (es decir puntos)
        for (auto figure : vecOfFigures)
            window.draw(&figure.vecfigureVertexArray[0], figure.vecfigureVertexArray.size(), sf::TriangleFan);

        if(LightOn && LightOn2)
        {
            if (showExtraLines)
            {
                for (auto lineaDesdeEsquina : vecLineasProyecEsquinas)
                {
                    // Dibuja todas las lineas proyectadas desde las esquinas de las figuras
                    window.draw(lineaDesdeEsquina.vertexLine);
                }
                for (auto point : vecPointsProyected)
                    window.draw(point.circle);
            }
            if (ShowLight)
            {
                // Dibujame los triángulos de luz Sólo si el ratón está dentro del cuadrado.
                for (auto triangleLuz : vecTriangulosLuz)
                    window.draw(triangleLuz.MyTriangle);
            }
        }

        window.display();
    }
    return 0;
}
