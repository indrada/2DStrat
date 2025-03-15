#include <string>
#include "worldMap.h"
#include <math.h>
#include "resource.h"
#include "util.h"

Resource::Resource(std::string name, float abundance)
{
    bool isClustered = true;
    float clusterFrequency = 0.002f;
    this->name = name;
    this->abundance = abundance;
    this->isClustered = isClustered;
    this->clusterFrequency = clusterFrequency;
}

Resource::Resource(std::string name, float abundance, bool isClustered = true, float clusterFrequency = 0.002f)
{
    this->name = name;
    this->abundance = abundance;
    this->isClustered = isClustered;
    this->clusterFrequency = clusterFrequency;
}
void Resource::registerResource(worldMap * map)
{
    map->resourceNames.push_back(name);



    int resourceIndex = map->resourceNames.size() - 1;

    for (int i = 0; i < map->verticalSize * map->horizontalSize; i++)
    {
        map->mapTiles[i].resourceQuantity.push_back(0.0f);
    }

    for (int i = 0; i < map->verticalSize * map->horizontalSize; i++)
    {
        if (RandomNumber(0.0f, 1.0f) < clusterFrequency)
        {
            for (int j = 0; j < map->verticalSize * map->horizontalSize; j++)
            {
                if (i != j)
                {
                    (map->mapTiles[j].resourceQuantity)[resourceIndex] += abundance / (abs((i % map->horizontalSize) - (j % map->horizontalSize)) + abs((i / map->horizontalSize) - (j / map->horizontalSize)));
                }
                else
                {
                    (map->mapTiles[j].resourceQuantity)[resourceIndex] += abundance * 2;
                }
            }
        }
    }

}