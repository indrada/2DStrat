#ifndef WORLDMAP_H
#define WORLDMAP_H

#include <queue>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "tile.h"
#include <memory>

class worldMap;


class mapMode
{
    public:
        virtual sf::Color getTileColor(int x, int y, worldMap toDisplay) = 0;
		void generateVertexArray();
		worldMap * map;
		sf::VertexArray triangles;
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
	void updateTileRender(int x, int y);
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
    void removeDead();

};

class elevationMap : virtual public mapMode
{
public:
    sf::Color getTileColor(int x, int y, worldMap toDisplay) override;
};

class defaultMap : virtual public mapMode
{
public:
    sf::Color getTileColor(int x, int y, worldMap toDisplay) override;

};

class resourceMap : virtual public mapMode
{
public:
    sf::Color getTileColor(int x, int y, worldMap toDisplay) override;
    resourceMap(worldMap * map, int resourceIndex);

private:
    int resourceIndex;
};

#endif