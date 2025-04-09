#pragma once
#ifndef item_h
#define item_h
#include <string>
#include <vector>
class tile;
class person;
class locatable;
class item
{
    public:
        
        item();
        ~item();
        int count;
        int weight;
        int value;
        std::string name;

        item(int weight, int value, int count, std::string name);

        bool operator==(const item& other);
};

class inventory
{
    public:
        std::vector<item *> items;
        int itemLimit;
        int weightLimit;
        int numItems;
        int currentWeight;
        locatable * attached;
        bool hasItem(item toCheck);

        inventory();
        inventory(int itemLimit, int weightLimit, locatable * attached);
        void addItem(item * toAdd);
        void dropItem(item toDrop, int count, tile * location);


};
#endif