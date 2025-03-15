#include "person.h"
#include"tile.h"
#include <vector>


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
}

tile::tile(int x, int y, float elevation = 0.0f, float waterLevel = 0.0f, person* personHere = NULL)
{
    xPos = x;
    yPos = y;
    this->elevation = elevation;
    this->personHere = personHere;
    this->waterLevel = waterLevel;
}