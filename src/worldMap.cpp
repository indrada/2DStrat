#include <map>
#include <vector>
#include <deque>
#include <string>
#include <math.h>
#include <random>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include "worldMap.h"
#include "util.h"
#include "person.h"
#include "context.h"
extern globalContext context;

template <class T, class A, class Predicate>
void erase_if(std::vector<T, A>& c, Predicate pred) {
    c.erase(remove_if(c.begin(), c.end(), pred), c.end());
}

worldMap::worldMap(sf::RenderWindow* window, int horizontalSize, int verticalSize,  mapMode* mode,float terrainElevation = 5.0f)
{
	this->window = window;
    this->mode = mode;
    this->mapTiles = (tile*)malloc(horizontalSize * verticalSize * sizeof(tile));
    for (int i = 0; i < verticalSize * horizontalSize; i++)
    {
        new (mapTiles + i) tile(i%horizontalSize,i/horizontalSize);
    }
    this->horizontalSize = horizontalSize;
    this->verticalSize = verticalSize;
	(this->mode->map) = this;
	this->mode->triangles = sf::VertexArray(sf::PrimitiveType::Triangles, 6*verticalSize*horizontalSize);
    generateTerrain(terrainElevation);
    resourceNames = {};
    for (int i = 0; i < verticalSize * horizontalSize; i++)
    {
        mapTiles[i].waterLevel = mapTiles[i].elevation;
        shuffledIndices.push_back(i);
    }
    getRectangles(*window, *mode);
	auto cmp = [](person * left, person * right) { if(left->tasks.empty()) return true; if(right->tasks.empty()) return false; return (left->tasks.front()->timeToComplete) > (right->tasks.front()->timeToComplete); };
    std::priority_queue<person *, std::vector<person *>, decltype(cmp)> allPersons(cmp);
}

void worldMap::getRectangles(sf::RenderWindow& win, mapMode& mode) {
    auto size = win.getView().getSize();
    float rowH = size.y / verticalSize;
    float colW = size.y / horizontalSize;
    rectangles = (sf::RectangleShape*)malloc(horizontalSize * verticalSize * sizeof(sf::RectangleShape));
    for (int i = 0; i < verticalSize; i++)
    {
        for (int j = 0; j < horizontalSize; j++)
        {
            new (rectangles + i * horizontalSize + j) sf::RectangleShape({ colW,rowH });
            (*(rectangles + i * horizontalSize + j)).setPosition({ colW * j,rowH * i });
            (*(rectangles + i * horizontalSize + j)).setFillColor(mode.getTileColor(j, i, *this));
        }
    }

}

void worldMap::updateRectangle(int xPos, int yPos)
{
    int index = xPos + yPos * horizontalSize;
    rectangles[index].setFillColor(mode->getTileColor(xPos, yPos, *this));
}



void worldMap::generateTerrain(float terrainHeight = 5.0f)
{
    float* gradients = (float*)malloc(sizeof(float) * (verticalSize - 1) * (horizontalSize - 1) * 2);
    gradients[0] = RandomNumber(-terrainHeight, terrainHeight);
    gradients[1] = RandomNumber(-terrainHeight, terrainHeight);
    mapTiles[0].elevation = 0.0f;
    for (int i = 1; i < horizontalSize - 1; i++)
    {
        gradients[2 * i] = gradients[2 * (i - 1)] + RandomNumber(-terrainHeight, terrainHeight);
        gradients[2 * i + 1] = gradients[2 * (i - 1) + 1] + RandomNumber(-terrainHeight, terrainHeight);
        mapTiles[i].elevation = gradients[2 * (i - 1) + 1] + mapTiles[i - 1].elevation;
    }
    for (int j = 1; j < verticalSize - 1; j++)
    {
        gradients[2 * j * (horizontalSize - 1)] = gradients[2 * (j - 1) * (horizontalSize - 1)] + RandomNumber(-terrainHeight, terrainHeight);
        gradients[2 * j * (horizontalSize - 1) + 1] = gradients[2 * (j - 1) * (horizontalSize - 1) + 1] + RandomNumber(-terrainHeight, terrainHeight);
        mapTiles[j * horizontalSize].elevation = gradients[2 * (j - 1) * (horizontalSize - 1)] + mapTiles[(j - 1) * horizontalSize].elevation;

    }
    for (int i = 1; i < horizontalSize - 1; i++)
    {
        for (int j = 1; j < verticalSize - 1; j++)
        {

            gradients[2 * (j * (horizontalSize - 1) + i)] = RandomNumber(-terrainHeight, terrainHeight) + (gradients[2 * ((j - 1) * (horizontalSize - 1) + i)] + gradients[2 * (j * (horizontalSize - 1) + i - 1)]) / 2;

            gradients[2 * (j * (horizontalSize - 1) + i) + 1] = RandomNumber(-terrainHeight, terrainHeight) + (gradients[2 * ((j - 1) * (horizontalSize - 1) + i) + 1] + gradients[2 * (j * (horizontalSize - 1) + i - 1) + 1]) / 2;

            mapTiles[j * horizontalSize + i].elevation = (mapTiles[(j - 1) * horizontalSize + i].elevation + mapTiles[j * horizontalSize + i - 1].elevation + gradients[2 * ((j - 1) * (horizontalSize - 1) + i)] + gradients[2 * (j * (horizontalSize - 1) + i - 1) + 1]) / 2;

        }

        mapTiles[(verticalSize - 1) * horizontalSize + i].elevation = mapTiles[(verticalSize - 2) * horizontalSize + i].elevation + gradients[2 * ((verticalSize - 2) * (horizontalSize - 1) + i) + 1];

    }
    for (int j = 1; j < verticalSize - 1; j++)
    {

        mapTiles[((horizontalSize - 1) + horizontalSize * j)].elevation = mapTiles[((horizontalSize - 2) + horizontalSize * j)].elevation + gradients[2 * ((horizontalSize - 1) * j + horizontalSize - 2) + 1];
    }
    mapTiles[horizontalSize * verticalSize - 1].elevation = (mapTiles[horizontalSize * verticalSize - 2].elevation + mapTiles[horizontalSize * (verticalSize - 1) - 1].elevation) / 2;
    free(gradients);
}





tile* worldMap::tileAt(int x, int y)
{
    if(y >= verticalSize || x >= horizontalSize || x < 0 || y < 0) return NULL;
    return mapTiles + (y * horizontalSize + x);
}

float worldMap::maxElevation()
{
    float retval = 0.0f;
    for (int i = 0; i < horizontalSize * verticalSize; i++)
    {
        if (abs(mapTiles[i].elevation) > retval) retval = abs(mapTiles[i].elevation);
    }
    return retval;
}

float worldMap::maxResourceValue(int resourceIndex)
{
    if (resourceIndex >= resourceNames.size())
    {
        return 0;
    }
    float retval = 0.0f;
    for (int i = 0; i < horizontalSize * verticalSize; i++)
    {
        if (mapTiles[i].resourceQuantity.at(resourceIndex) > retval)
        {
            retval = mapTiles[i].resourceQuantity.at(resourceIndex);
        }
    }
    return retval;
}

void worldMap::doTasks(int time)
{
	int nextTaskLength;
    printf("\nasd123\n");
	bool taskDone;
	while(time>0)
	{
		if(allPersons.empty()) return;
		nextTaskLength = time;
        printf("\nabc123\n");
		for(person * toDo : allPersons)
		{
			if(!((toDo->tasks).empty())&&toDo->tasks.front()->timeToComplete < nextTaskLength)
			{
				nextTaskLength = toDo->tasks.front()->timeToComplete;
			}
		}
        printf("\na123\n");
		taskDone = false;
		for(person * toDo : allPersons)
		{
			if(toDo->doTasks(nextTaskLength)) taskDone = true;			
		}
        if(!taskDone) return;
		time-=nextTaskLength;
        printf("\naa123\n");
        removeDead();        
        printf("\naaa123\n");
		sf::sleep(sf::milliseconds(250));
        context.scene->renderFrame();
        printf("\naab123\n");
	}
}

void worldMap::updateAttributes()
{
	for(person * toUpdate : allPersons)
	{
		toUpdate->updateAttributes();
	}
}

void worldMap::updateMapMode(mapMode * mode)
{
	this->mode = mode;
	mode->generateVertexArray();
}

void mapMode::generateVertexArray()
{
	
    auto size = map->window->getView().getSize();
    float rowH = size.y / map->verticalSize;
    float colW = size.y / map->horizontalSize;
	for(int i = 0; i < map->verticalSize; i++)
	{
		for(int j = 0; j < map->horizontalSize; j++)
		{
			triangles[6*(map->horizontalSize*i+j)].position=sf::Vector2f({colW*j,rowH*i});
			triangles[6*(map->horizontalSize*i+j)+1].position=sf::Vector2f({colW*j+colW,rowH*i});
			triangles[6*(map->horizontalSize*i+j)+2].position=sf::Vector2f({colW*j,rowH*i+rowH});
			triangles[6*(map->horizontalSize*i+j)+3].position=sf::Vector2f({colW*j+colW,rowH*i});
			triangles[6*(map->horizontalSize*i+j)+4].position=sf::Vector2f({colW*j,rowH*i+rowH});
			triangles[6*(map->horizontalSize*i+j)+5].position=sf::Vector2f({colW*j+colW,rowH*i+rowH});
			triangles[6*(map->horizontalSize*i+j)].color=triangles[6*(map->horizontalSize*i+j)+1].color=triangles[6*(map->horizontalSize*i+j)+2].color=triangles[6*(map->horizontalSize*i+j)+3].color=triangles[6*(map->horizontalSize*i+j)+4].color=triangles[6*(map->horizontalSize*i+j)+5].color = getTileColor(j,i,*map);
		}
	}
}

void worldMap::updateTileRender(int j, int i)
{
	mode->triangles[6*(horizontalSize*i+j)].color=mode->triangles[6*(horizontalSize*i+j)+1].color=mode->triangles[6*(horizontalSize*i+j)+2].color=mode->triangles[6*(horizontalSize*i+j)+3].color=mode->triangles[6*(horizontalSize*i+j)+4].color=mode->triangles[6*(horizontalSize*i+j)+5].color = mode->getTileColor(j,i,*this);
}

void worldMap::removeDead()
{
    int i = 0;
    for(person * toRemove : allPersons)
    {
        if(!toRemove->isAlive())
        {
            if(tileAt(toRemove->xPos,toRemove->yPos)->personHere == toRemove)
            {
                tileAt(toRemove->xPos,toRemove->yPos)->personHere = NULL;
            }
            updateTileRender(toRemove->xPos,toRemove->yPos);
        }
    }
    erase_if(allPersons,[](person * x){return !x->isAlive();});
}

sf::Color elevationMap::getTileColor(int x, int y, worldMap toDisplay)
{
    unsigned int colorScale = (unsigned int)(127 * (1 + toDisplay.mapTiles[y * toDisplay.horizontalSize + x].elevation / (1 + toDisplay.maxElevation())));
    return sf::Color(colorScale, colorScale, colorScale);
}

sf::Color defaultMap::getTileColor(int x, int y, worldMap toDisplay)
{
    if(toDisplay.tileAt(x, y)-> structureHere!=NULL) return toDisplay.tileAt(x, y)->structureHere->color;
    if (toDisplay.tileAt(x, y)->personHere != NULL)
    {
        if (toDisplay.tileAt(x, y)->personHere->getIsFriendly())
            return sf::Color(0, 255, 0);
        else
            return sf::Color(255, 0, 0);
    }
    if (toDisplay.mapTiles[y * toDisplay.horizontalSize + x].waterDepth() > 0.0f)
        return sf::Color(62, 164, 240);
    unsigned int colorScale = (unsigned int)(127 * (1 - toDisplay.mapTiles[y * toDisplay.horizontalSize + x].elevation / (1 + toDisplay.maxElevation())));
    return sf::Color(colorScale, colorScale, colorScale);
}

sf::Color resourceMap::getTileColor(int x, int y, worldMap toDisplay)
{
    unsigned int colorScale = (unsigned int)(127 * (1 - toDisplay.mapTiles[y * toDisplay.horizontalSize + x].resourceQuantity.at(resourceIndex) / (1 + toDisplay.maxResourceValue(resourceIndex))));
    return sf::Color(colorScale, colorScale, colorScale);
}

resourceMap::resourceMap(worldMap *map, int resourceIndex)
{
    this->resourceIndex = resourceIndex;
    this->map = map;
    triangles = sf::VertexArray(sf::PrimitiveType::Triangles, 6 * map->verticalSize * map->horizontalSize);
}

void worldMap::addStructure(structure * toAdd, int x, int y)
{
    tileAt(x,y)->structureHere = toAdd;
}

void worldMap::moveEnemiesRandom()
{
    int xTemp, yTemp, randX, randY;
    for (int i = 0; i < this->allPersons.size(); ++i)
    {
        if (this->allPersons[i]->getIsFriendly()) continue;
        this->allPersons[i]->moveTo(context.mainCityPos.x,context.mainCityPos.y);
    }
}


