#ifndef resource_h
#define resource_h

#include "worldMap.h"
#include <string>

class Resource
{
public:
    std::string name;
    float abundance;
    bool isClustered;
    float clusterFrequency;
    Resource(std::string name, float abundance);
    Resource(std::string name, float abundance, bool isClustered, float clusterFrequency);
    void registerResource(worldMap * map);
};

#endif