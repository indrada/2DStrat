#ifndef WORLDMAP_H
#define WORLDMAP_H

#include <queue>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "tile.h"

class worldMap;


class mapMode
{
    public:
        virtual sf::Color getTileColor(int x, int y, worldMap toDisplay) = 0;
};


class worldMap {
public:
    tile* mapTiles;
    int horizontalSize;
    int verticalSize;
    mapMode* mode;
    std::vector<std::string> resourceNames;
    std::vector<int> shuffledIndices;
	std::vector<person *> allPersons;
    sf::RectangleShape* rectangles;
	sf::RenderWindow *window;
	void updateMapMode(mapMode * mode);
    worldMap(sf::RenderWindow* window, int horizontalSize, int verticalSize, mapMode* mode, float terrainElevation);
    void generateTerrain(float terrainHeight);
    void getRectangles(sf::RenderWindow& win, mapMode& mode);
    void updateRectangle(int xPos, int yPos);
    tile* tileAt(int x, int y);
    float maxElevation();
    float generateResourceQuantity(float abundance, bool isClustered);
	void doTasks(int time);
    float maxResourceValue(int resourceIndex);
	void updateAttributes();
};

class elevationMap : virtual public mapMode
{
public:
    sf::Color getTileColor(int x, int y, worldMap toDisplay) override
    {
        unsigned int colorScale = (unsigned int)(127 * (1 + toDisplay.mapTiles[y * toDisplay.horizontalSize + x].elevation / (1 + toDisplay.maxElevation())));
        return sf::Color(colorScale, colorScale, colorScale);
    }
};

class defaultMap : virtual public mapMode
{
public:
    sf::Color getTileColor(int x, int y, worldMap toDisplay) override
    {
        if (toDisplay.tileAt(x, y)->personHere != NULL) return sf::Color(0, 255, 0);
        if (toDisplay.mapTiles[y * toDisplay.horizontalSize + x].waterDepth() > 0.0f) return sf::Color(62, 164, 240);
        unsigned int colorScale = (unsigned int)(127 * (1 - toDisplay.mapTiles[y * toDisplay.horizontalSize + x].elevation / (1 + toDisplay.maxElevation())));
        return sf::Color(colorScale, colorScale, colorScale);
    }

};

class resourceMap : virtual public mapMode
{
	public:
		int resourceIndex;
		sf::Color getTileColor(int x, int y, worldMap toDisplay) override
		{
			unsigned int colorScale = (unsigned int)(127 * (1 - toDisplay.mapTiles[y * toDisplay.horizontalSize + x].resourceQuantity.at(resourceIndex) / (1 + toDisplay.maxResourceValue(resourceIndex))));
			return sf::Color(colorScale, colorScale, colorScale);
		}
		resourceMap(int resourceIndex)
		{
			this->resourceIndex = resourceIndex;
		}
};

#endif