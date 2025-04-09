#pragma once
#ifndef item_h
#define item_h
#include <string>
#include <vector>
class person;
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
        person * assignedPerson;

        inventory();
        inventory(int itemLimit, int weightLimit, person * assignedPerson);
        void addItem(item * toAdd);
        void dropItem(item toDrop, int count);


};
#endif