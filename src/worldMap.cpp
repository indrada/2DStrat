#include <map>
#include <vector>
#include <deque>
#include <string>
#include <math.h>
#include <random>
#include <algorithm>
#include <SFML/graphics.hpp>
#include "worldMap.h"
#include "util.h"
#include "person.h"

worldMap::worldMap(sf::RenderWindow& window, int horizontalSize, int verticalSize,  mapMode* mode,float terrainElevation = 5.0f)
{
    this->mode = mode;
    this->mapTiles = (tile*)malloc(horizontalSize * verticalSize * sizeof(tile));
    for (int i = 0; i < verticalSize * horizontalSize; i++)
    {
        new (mapTiles + i) tile(i%horizontalSize,i/horizontalSize);
    }
    this->horizontalSize = horizontalSize;
    this->verticalSize = verticalSize;
    generateTerrain(terrainElevation);
    resourceNames = {};
    for (int i = 0; i < verticalSize * horizontalSize; i++)
    {
        mapTiles[i].waterLevel = mapTiles[i].elevation;
        shuffledIndices.push_back(i);
    }
    getRectangles(window, *mode);
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
	
	while(time>0)
	{
		if(allPersons.empty()) return;
		nextTaskLength = time;
		for(person * toDo : allPersons)
		{
			if(!((toDo->tasks).empty())&&toDo->tasks.front()->timeToComplete < nextTaskLength)
			{
				nextTaskLength = toDo->tasks.front()->timeToComplete;
			}
		}
		
		for(person * toDo : allPersons)
		{
			toDo->doTasks(nextTaskLength);			
		}
		time-=nextTaskLength;
	}
}

void worldMap::updateAttributes()
{
	for(person * toUpdate : allPersons)
	{
		toUpdate->updateAttributes();
	}
}
