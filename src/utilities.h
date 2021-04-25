#include <SFML/Graphics.hpp>
#include "math.h"
#include "Point.h"
#include "Line.h"

sf::Vector2f getMousePos(const sf::RenderWindow& window);
sf::Vector2f normalizeVector(sf::Vector2f vec);
sf::Vector2f proyectedPointOverLine(sf::VertexArray line, sf::Vector2f Point, bool& exists);

float innerProduct(sf::Vector2f vec1, sf::Vector2f vec2);
float module(sf::Vector2f vec);
float crossProduct2d(sf::Vector2f vec1, sf::Vector2f vec2);
float signed2dTriArea(sf::Vector2f PointA, sf::Vector2f PointB, sf::Vector2f PointC);

void intersecSegmentSegment(Line line1, Line line2, bool& intersect, sf::Vector2f& P);
void createLinesToPoint(const std::vector<Line> vecOfLines, std::vector<Line>& vecHoldLinestoMouse, sf::Vector2f Point);
void createdIntersectPointsOnStack(Line& lineaProyectadaMouse, sf::Vector2f intersectPoint, sf::Vector2f MousePos);

void createTxt(std::vector<sf::Text>& vecText, sf::Font& font);
void fillSystemLines(std::vector<Line>& vecOfOriginalLines, std::vector<Line>& systemLines);

Line createLineProyectionFromCornerPoint(sf::Vector2f cornerPoint, sf::Vector2f mousePos, std::string father1, std::string father2);

bool contains(std::vector<Line> vecLineasFigura, sf::Vector2f MousePos);

void fillRecsScreenAndClickeable(std::vector<sf::RectangleShape>& vecRecs);

