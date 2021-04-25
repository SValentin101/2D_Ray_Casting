#include <SFML/Graphics.hpp>
#include "Figure.h"

void handleEvents(sf::RenderWindow& window, sf::Event& event, std::vector<Figure>& vecOfFigures, bool& lightOn2, std::vector<sf::Text> vecText, sf::RectangleShape& smallRec,
        sf::RectangleShape& smallRec2, bool& colorSamllRec, bool& showLight);
