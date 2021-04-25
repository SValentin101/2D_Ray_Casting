#include "utilities.h"

sf::Vector2f getMousePos(const sf::RenderWindow& window)
{
    sf::Vector2f PosMouse;
    PosMouse.x = static_cast<float>(sf::Mouse::getPosition(window).x);
    PosMouse.y = static_cast<float>(sf::Mouse::getPosition(window).y);
    return PosMouse;
}

float module(sf::Vector2f vec)
{
    return std::sqrt(std::pow(vec.x, 2) + std::pow(vec.y, 2));
}
sf::Vector2f normalizeVector(sf::Vector2f vec)
{
    if(module(vec) != 0)
        return sf::Vector2f(vec.x/module(vec), vec.y/module(vec));
    else
        return sf::Vector2f(0,0);
}
float innerProduct(sf::Vector2f vec1, sf::Vector2f vec2)
{
    return (vec1.x * vec2.x + vec1.y + vec2.y);
}

float crossProduct2d(sf::Vector2f vec1, sf::Vector2f vec2)
{
    return (vec1.x*vec2.y - vec1.y*vec2.x);
}

sf::Vector2f proyectedPointOverLine(sf::VertexArray line, sf::Vector2f Point, bool& exists)
{
    // Un SEGMENTO lo puedo representar con: S = P + v*t, donde 0<= t <= 1. Un rayo sería con 0<=t y una línea con -inf<=t<=inf
    sf::Vector2f vecABLinea(line[1].position.x - line[0].position.x, line[1].position.y - line[0].position.y);
    sf::Vector2f vecProyectado(Point.x - line[0].position.x, Point.y - line[0].position.y);

    float t = innerProduct(vecProyectado, vecABLinea) / innerProduct(vecABLinea, vecABLinea);

    if( t < 0.0f) t = 0.0f;
    if( t > 1.0f) t = 1.0f;

    // Dame la posición
    sf::Vector2f PosPuntoProyectado(line[0].position.x + t * vecABLinea.x, line[0].position.y + t * vecABLinea.y);
    if(t < 0.0)
    {
        exists = false;
        return sf::Vector2f(0,0);
    }
    else
    {
        exists = true;
        return PosPuntoProyectado;
    }
}

float signed2dTriArea(sf::Vector2f PointA, sf::Vector2f PointB, sf::Vector2f PointC)
{
    return (PointA.x - PointC.x) * (PointB.y - PointC.y) - (PointA.y - PointC.y) * (PointB.x - PointC.x);
}

void intersecSegmentSegment(Line line1, Line line2, bool& intersect, sf::Vector2f& P)
{
    float a1 = signed2dTriArea(line1.vertexLine[0].position, line1.vertexLine[1].position, line2.vertexLine[1].position);
    float a2 = signed2dTriArea(line1.vertexLine[0].position, line1.vertexLine[1].position, line2.vertexLine[0].position);

    if(a1 != 0.0f && a2 != 0.0f && a1*a2 < 0.0f)
    {
        float a3 = signed2dTriArea(line2.vertexLine[0].position, line2.vertexLine[1].position, line1.vertexLine[0].position);
        float a4 = a3 + a2 - a1;

        if(a3 * a4 < 0.0f)
        {
            // Segments intersect!
            float t = a3 / (a3 - a4);
            intersect = true;
            P.x = line1.vertexLine[0].position.x + t * (line1.vertexLine[1].position.x - line1.vertexLine[0].position.x);
            P.y = line1.vertexLine[0].position.y + t * (line1.vertexLine[1].position.y - line1.vertexLine[0].position.y);
        }
        else
        {
            // Segments do not intersect!
            intersect = false;
            P.x = 0; P.y = 0;
        }
    }
}

void createLinesToPoint(const std::vector<Line> vecOfLines, std::vector<Line>& vecHoldLinestoMouse, sf::Vector2f Point)
{
    for (auto elem : vecOfLines)
    {
        for (int i = 0; i <= 1; i++)
        {
            Line newLine(elem.vertexLine[i].position, Point, sf::Color::Red, elem.father);
            vecHoldLinestoMouse.push_back(newLine);
        }
    }
}

void createdIntersectPointsOnStack(Line& lineaProyectadaMouse, sf::Vector2f intersectPoint, sf::Vector2f MousePos)
{
    lineaProyectadaMouse.stackOfIntersectPoints.push(intersectPoint);
}

bool contains(std::vector<Line> vecLineasFigura, sf::Vector2f MousePos)
{
    std::vector<Line> vecLinesFromCorners;
    float minX = 3000, minY = 3000;
    float maxX = 0, maxY = 0;
    for (unsigned int i = 0; i < vecLineasFigura.size(); i++)
    {
        vecLinesFromCorners.emplace_back(vecLineasFigura[i].Point_A.Position, MousePos, sf::Color::White);
        // Hallame tbn el mín X y el min Y de entre todos los puntos que conforman la figura
        if (vecLineasFigura[i].Point_A.Position.x < minX)
            minX = vecLineasFigura[i].Point_A.Position.x;
        if(vecLineasFigura[i].Point_A.Position.y < minY)
            minY = vecLineasFigura[i].Point_A.Position.y;
        // Tbn los máx X y max Y ya que estás
        if(vecLineasFigura[i].Point_A.Position.x > maxX)
            maxX = vecLineasFigura[i].Point_A.Position.x;
        if(vecLineasFigura[i].Point_A.Position.y > maxY)
            maxY = vecLineasFigura[i].Point_A.Position.y;
    }
    // Si estás mas o menos cerca del cuerpo, viene a ser el contain de los rectangles.
    if ((MousePos.x >= minX && MousePos.x < maxX && MousePos.y >= minY && MousePos.y < maxY))
    {
        int numCortes = 0;
        for (auto lineaFigura : vecLineasFigura)
        {
            for (auto lineaEsquinas : vecLinesFromCorners)
            {
                 sf::Vector2f intersectPos;
                 bool intersect = false;
                 intersecSegmentSegment(lineaEsquinas, lineaFigura, intersect, intersectPos);
                 if (intersect)
                     numCortes++;
            }
        }
        // Si no hay cortes desde las lineas de las esquinas con las lineas que conforman el cuadrado...
        // Entonces el ratón está dentro del objeto
        if (numCortes == 0)
            return true;
        else
            return false;
    }
    // No estás ni cerca del cuerpo como para compararlo en más detalle, contains = false
    return false;
}

Line createLineProyectionFromCornerPoint(sf::Vector2f cornerPoint, sf::Vector2f mousePos, std::string father1, std::string father2)
{
    sf::VertexArray createdLine(sf::Lines, 2);

    createdLine[0].position = cornerPoint;
    sf::Vector2f AB = cornerPoint - mousePos;
    sf::Vector2f Position(cornerPoint.x + AB.x * 1000, cornerPoint.y + AB.y * 1000);
    createdLine[1].position = Position;

    Line proyectedLine(createdLine[0].position, createdLine[1].position, sf::Color::Cyan, father1, father2);

    return proyectedLine;
}

void createTxt(std::vector<sf::Text>& vecText, sf::Font& font)
{
    sf::Text txt;
    txt.setFont(font);
    txt.setCharacterSize(24);
    txt.setFillColor(sf::Color::White);
    txt.setOutlineThickness(1);
    txt.setOutlineColor(sf::Color::Black);

    for (auto i = 0; i != 7; i++)
    {
        if (i == 0)
        {
            txt.setString("Square");
            txt.setPosition(1250, 120);
        }
        else if (i == 1)
        {
            txt.setString("Rectangle");
            txt.setPosition(1250, 160);
        }
        else if (i == 2)
        {
            txt.setString("Triangle");
            txt.setPosition(1250, 200);
        }
        else if (i == 3)
        {
            txt.setString("Trapezium");
            txt.setPosition(1250, 240);
        }
        else if (i == 4)
        {
            txt.setString("Clear All");
            txt.setPosition(1250, 315);
        }
        else if (i == 5)
        {
            txt.setCharacterSize(18);
            txt.setString("Show lines and points");
            txt.setPosition(1250, 40);
        }
        else if (i == 6)
        {
            txt.setCharacterSize(18);
            txt.setString("Show Light");
            txt.setPosition(1250, 70);
        }
        vecText.push_back(txt);
    }
}

void fillSystemLines(std::vector<Line>& vecOfOriginalLines, std::vector<Line>& systemLines)
{
    // Antes de nada debo limpiar los dos vectores para que no se dispare la cantidad de elemtos que almacenan.
    vecOfOriginalLines.clear();
    systemLines.clear();
    //

    vecOfOriginalLines.emplace_back(sf::Vector2f(10, 10), sf::Vector2f(1200, 10), sf::Color::White, "Line_TOP");
    vecOfOriginalLines.emplace_back(sf::Vector2f(1200, 10), sf::Vector2f(1200, 990), sf::Color::White, "Line_RIGTH");
    vecOfOriginalLines.emplace_back(sf::Vector2f(10, 990), sf::Vector2f(1200, 990), sf::Color::White, "Line_BOT");
    vecOfOriginalLines.emplace_back(sf::Vector2f(10, 10), sf::Vector2f(10, 990), sf::Color::White, "Line_LEFT");

    for (auto originalLine : vecOfOriginalLines)
        systemLines.emplace_back(originalLine);
}

void fillRecsScreenAndClickeable(std::vector<sf::RectangleShape>& vecRecs)
{
    for (auto i = 0; i != 4; i++)
    {
        sf::RectangleShape rec;
        if (i == 0)
        {
            rec.setFillColor(sf::Color::White);
            rec.setOutlineThickness(2);
            rec.setOutlineColor(sf::Color::Black);
            rec.setPosition(sf::Vector2f(1490,42.5));
            rec.setSize(sf::Vector2f(15,15));
            vecRecs.push_back(rec);
        }
        else if (i == 1)
        {
            rec.setFillColor(sf::Color::White);
            rec.setOutlineThickness(2);
            rec.setOutlineColor(sf::Color::Black);
            rec.setPosition(sf::Vector2f(1385, 73));
            rec.setSize(sf::Vector2f(15,15));
            vecRecs.push_back(rec);
        }
        else if (i == 2)
        {
            rec.setFillColor(sf::Color::Black);
            rec.setPosition(sf::Vector2f(10,10));
            rec.setSize(sf::Vector2f(1190,980));
            vecRecs.push_back(rec);
        }
        else if (i == 3)
        {
            rec.setFillColor(sf::Color(0, 158, 128));
            rec.setOutlineThickness(3);
            rec.setOutlineColor(sf::Color::White);
            rec.setPosition(sf::Vector2f(1450,130));
            rec.setSize(sf::Vector2f(275,140));
            vecRecs.push_back(rec);
        }
    }
}
