#include "Figure.h"
#include "utilities.h"
#include <iostream>

Figure::Figure(std::string figureName, sf::Vector2f Point1, sf::Vector2f Point2, sf::Vector2f Point3,
            sf::Vector2f Point4, sf::Vector2f Point5, sf::Vector2f Point6)
            : clickOverFigure(false), spinFigure(false), resizeFigure(false), angleOfSpin(0.0f), ratioOfResize(0.0f)
{
        figureCount++;
        nameFigure = figureName + "." + std::to_string(figureCount);
        vecPointsFigure.push_back(Point1);
        vecPointsFigure.push_back(Point2);
        if (Point3 != sf::Vector2f(0,0))
            vecPointsFigure.push_back(Point3);
        if (Point4 != sf::Vector2f(0,0))
            vecPointsFigure.push_back(Point4);
        if (Point5 != sf::Vector2f(0,0))
            vecPointsFigure.push_back(Point5);
        if (Point6 != sf::Vector2f(0,0))
            vecPointsFigure.push_back(Point6);

        updateFigure();
}

void Figure::updateFigure()
{
    createLinesForFigure();
    createCenterPointFigure();
}
void Figure::createLinesForFigure()
{
    // Limpiamos el vectorLinesFigure. Para evitar que crezca y crezca con cada llamada al updateFigure.
    vecLinesFigure.clear();
    // Este vertexarray será el que nos almacene los puntos en sí, aislados, sin ningún tipo de interconexión entre ellos.
    vecfigureVertexArray.clear();

    int numLine = 0;
    for (auto it = vecPointsFigure.cbegin(); it != vecPointsFigure.cend(); it++)
    {
        std::string nameOfLine = nameFigure + ".";
        std::string numOfLine = std::to_string(numLine);
        // No puedo incrementar it++ ya que alterará el bucle, y solo iterara 2 veces en vez de 4
        auto nextPoint = it;
        nextPoint++;

        if (*it != vecPointsFigure.back())
        {
            Line lineFigure(*it, *nextPoint, sf::Color::White, nameOfLine + numOfLine);

            vecLinesFigure.push_back(lineFigure);
        }
        else
        {
            Line lineFigure(*it, vecPointsFigure.front(), sf::Color::White, nameOfLine + numOfLine);
            lineFigure.fatherFigure = nameFigure;
            vecLinesFigure.push_back(lineFigure);
        }
        numLine++;

        // Añado el punto al figureVertexArray. Que será sobre el cual dibuje más tarde y así le doy color al objeto, lo relleno.

        vecfigureVertexArray.push_back(sf::Vertex(*it, sf::Color(179, 0, 89)));
        // sf::Color(102, 26, 255)
    }

    /* Basicamente es hacer esto. Crear las diferentes lineas del objeto.
    Line lineSquare1(SquareA, SquareB, sf::Color::White, "Line_square1");
    Line lineSquare2(SquareB, SquareC, sf::Color::White, "Line_square2");
    Line lineSquare3(SquareC, SquareD, sf::Color::White, "Line_square3");
    Line lineSquare4(SquareD, SquareA, sf::Color::White, "Line_square4");
     */
}

void Figure::createCenterPointFigure()
{
    // Encuentra el .  -> Cuadrado.15, Triangulo.7, quédate sólo con lo que hay hasta el ., es decir, el nombre.
    auto pos = nameFigure.find(".");

    float valueX = 0;
    float valueY = 0;

    if (nameFigure.substr(0, pos) == "Cuadrado" || nameFigure.substr(0, pos) == "Rectangulo"
            || nameFigure.substr(0, pos) == "Trapecio")
    {
        for (auto it = vecPointsFigure.cbegin(); it != vecPointsFigure.cend(); it++)
        {
            valueX += it->x;
            valueY += it->y;
        }
        centerPoint.x = valueX / 4;
        centerPoint.y = valueY / 4;
    }

    if (nameFigure.substr(0, pos) == "Triangulo")
    {
        for (auto it = vecPointsFigure.cbegin(); it != vecPointsFigure.cend(); it++)
        {
            valueX += it->x;
            valueY += it->y;
        }
        centerPoint.x = valueX / 3;
        centerPoint.y = valueY / 3;
    }
}

// Functions for movement/spin/resize

void handleFigureMovement(std::vector<Figure>& vecOfFigures, sf::Vector2f MousePos)
{
    for (auto& Figure : vecOfFigures)
    {
        if(Figure.clickOverFigure)
        {
           // Movimiento del objeto.
           // Calculame el desplazamiento del raton y mueveme todos los puntos
           // de la figura en base a la dif(x2-x1) y dif(y2-y1)
           sf::Vector2f dif = MousePos - Figure.mousePosOnClick;
           for (auto& Point : Figure.vecPointsFigure)
               Point += dif;

           // Ajusta la posición para evitar que el cuerpo se mueva después de soltarlo con una velocidad.
           Figure.mousePosOnClick = Figure.mousePosOnClick + dif;
        }
    }
}

void handleFigureSpin(std::vector<Figure>& vecOfFigures)
{
    for (auto& Figure : vecOfFigures)
    {
     // Spin
       if (Figure.spinFigure)
       {
           // Rotación de los vectores que van hasta cada uno de los puntos desde el centro de la figura.
           for (auto& Point : Figure.vecPointsFigure)
           {
               sf::Vector2f OriginToPointVector(Point - Figure.centerPoint);
               Point.x = std::cos(Figure.angleOfSpin) * OriginToPointVector.x - std::sin(Figure.angleOfSpin) * OriginToPointVector.y;
               Point.y = std::sin(Figure.angleOfSpin) * OriginToPointVector.x + std::cos(Figure.angleOfSpin) * OriginToPointVector.y;

               Point = Point + Figure.centerPoint;
           }
           Figure.spinFigure = false;
           Figure.resizeFigure = false;
       }
    }
}

void handleFigureResize(std::vector<Figure>& vecOfFigures)
{
    for (auto& Figure : vecOfFigures)
    {
       // Resize
        if (Figure.resizeFigure)
        {
           // Aumentamos o disminuimos los vectores que van hasta los puntos de la figura desde el centro de la figura.
            for (auto& Point : Figure.vecPointsFigure)
            {
                sf::Vector2f OriginToPointVector(Point - Figure.centerPoint);
                Point.x = Figure.ratioOfResize*OriginToPointVector.x;
                Point.y = Figure.ratioOfResize*OriginToPointVector.y;

               Point = Point + Figure.centerPoint;
            }
            Figure.spinFigure = false;
            Figure.resizeFigure = false;
        }
    }
}
