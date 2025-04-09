#include "person.h"
#include"tile.h"
#include <vector>

tile* locatable::getTile()
{
    return attachedMap->tileAt(xPos, yPos);
}

float tile::waterDepth()
{
    return waterLevel - elevation;
}

tile::tile(){

}

tile::tile(int x, int y)
{
    float elevation = 0.0f;
    float waterLevel = 0.0f;
    person* personHere = NULL;
    xPos = x;
    yPos = y;
    this->elevation = elevation;
    this->personHere = personHere;
    this->waterLevel = waterLevel;
    this->structureHere=NULL;
}

tile::tile(int x, int y, float elevation = 0.0f, float waterLevel = 0.0f, person* personHere = NULL)
{
    xPos = x;
    yPos = y;
    this->elevation = elevation;
    this->personHere = personHere;
    this->waterLevel = waterLevel;
    this->structureHere = NULL;
}

structure::structure(sf::Color color)
{
    this->color = color;
}

void mainCity::processTurn()
{
    return;
}

mainCity::mainCity(sf::Color color)
{
    this->color = color;
}

structure::structure(){}