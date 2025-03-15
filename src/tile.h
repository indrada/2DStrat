#ifndef TILE_H
#define TILE_H

#include <vector>

class person;  // Forward declaration

class tile {
public:
    int xPos;
    int yPos;
    float elevation;
    float waterLevel;
    person* personHere;
    std::vector<float> resourceQuantity;

    tile();
    tile(int x, int y);
    tile(int x, int y, float elevation, float waterLevel, person* personHere);
    float waterDepth();
};

#endif